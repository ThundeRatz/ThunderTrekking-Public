/* Tentativa de melhorar o erro do GPS n1
 * Assumindo que o erro se mantem constante
 * 
 * Muda as coordenadas dos eventos, baseado na diferenca
 * entre a coordenada medida na posicao inicial (eventos[0]) em algum momento
 * e a coordenada medida na mesma posicao no momento atual.
 *
 * As posicoes dos cones devem ter sido medidas no mesmo momento da posicao 
 * inicial.
 *
 * Testar na gaiola em posicoes aleatorias
 */

#include <stdio.h>
#include <time.h>

#include "ports.h"
#include "gps_coord.h"
#include "compass.h"
#include "thread_spawn.h"
#include "thread_hmc5883l.h"
#include "thread_gps.h"

#define MS	1000000

static gps_coord_t soma, media, dist;
static int n;
static const struct timespec gps_wait = {.tv_sec = 1, .tv_nsec = 0 * MS};

int main()
{
	thread_spawn(gps_thread, NULL);
	thread_spawn(hmc5883l_thread, NULL);
	
	// Media das medidas do GPS na posicao atual
	soma.latitude = soma.longitude = 0;
	for (n = 1; n <= 15; n++) {
		soma.latitude += gps_get()->latitude;
		soma.longitude += gps_get()->longitude;
		nanosleep(&gps_wait, NULL);
		printf("Posicao atual: %f %f\n", gps_get()->latitude, gps_get()->longitude);
	}
	media.latitude = soma.latitude/n;
	media.longitude = soma.longitude/n;
	printf("Posicao atual media: %f %f\n", media.latitude, media.longitude);
	
	// Ver se so fazer isso e valido
	// Se nao, ver como usar o angulo da bussola e outras funcoes para melhorar o erro
	dist.latitude = eventos[0].pos.latitude - media.latitude;
	dist.longitude = eventos[0].pos.longitude - media.longitude;
	printf("Diferenca calculada: %f %f\n", dist.latitude, dist.longitude);
	
	// OBS.: criar novas variaveis para guardar as novas posicoes para deixar as medidas
	// iniciais constantes, como eventos_novo ou algo assim, e usar essas variaveis na 
	// main, ou entao criar eventos_inicial pra gravar as medicoes iniciais e continuar
	// usando eventos na main
	for (i = 0; i < len(eventos); i++) { // Desloca todas as coordenadas
		eventos[i].pos.latitude -= dist.latitude;
		eventos[i].pos.longitude -= dist.longitude;
	}
	
	// Mostra as novas distancias, ver se se aproxima de 0
	// Ver se o angulo para as outras coordenadas e valido
	// Ver se a distancia se aproxima de 0 perto das outras coordenadas
	for (;;) {
		printf("\nPosicao atual: %f %f\n", gps_get()->latitude, gps_get()->longitude);
		for (i = 0; i < len(eventos); i++) {
			printf("--- Posicao %d\n", i);
			printf("Coordenadas: %f %f\n", eventos[i].pos.latitude, eventos[i].pos.longitude);
			printf("Azimuth: %f\n", azimuth(gps_get(), &eventos[i].pos));
			printf("Direção atual: %f\n", direcao_atual);
			printf("Diff %lf\n", compass_diff(azimuth(gps_get(), &eventos[i].pos), direcao_atual));
		}
		// Possivel colocar um nanosleep aqui pra facilitar a leitura dos dados
	}
}
