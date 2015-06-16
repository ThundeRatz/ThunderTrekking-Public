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
#define len(array)	((&array)[1] - array)

static gps_coord_t soma, media, dist;
static int n;
static const struct timespec gps_wait = {.tv_sec = 1, .tv_nsec = 0 * MS};

gps_coord_t testes[] = {
	//{.latitude = -23.55392375, .longitude = -46.72891395},  //Mesa elétrica
	{.latitude = -23.55387477, .longitude = -46.72899905},  //Interruptor
	{.latitude = -23.55344336, .longitude = -46.72915281},  //Hidrante
	//{.latitude = -23.64701287, .longitude = -46.57263634},
};

int main()
{
	int i;
	thread_spawn(gps_thread, NULL);
	thread_spawn(hmc5883l_thread, NULL);
	
	// Media das medidas do GPS na posicao atual
	nanosleep(&gps_wait, NULL);
	nanosleep(&gps_wait, NULL);
	nanosleep(&gps_wait, NULL);
//	nanosleep(&gps_wait, NULL);
//	nanosleep(&gps_wait, NULL);
//	nanosleep(&gps_wait, NULL);
	soma.latitude = soma.longitude = 0;
	for (n = 1; n <= 15; n++) {
		soma.latitude += gps_get()->latitude;
		soma.longitude += gps_get()->longitude;
		nanosleep(&gps_wait, NULL);
		printf("Posicao atual: %.8f %.8f\n", gps_get()->latitude, gps_get()->longitude);
	}
	media.latitude = soma.latitude/(n-1);
	media.longitude = soma.longitude/(n-1);
	printf("Posicao atual media: %.8f %.8f\n", media.latitude, media.longitude);
/*	soma.latitude = soma.longitude = 0;
	for (n = 1; n <= 15; n++) {
		soma.latitude += gps_get()->latitude;
		soma.longitude += gps_get()->longitude;
		nanosleep(&gps_wait, NULL);
		printf("Posicao atual: %.8f %.8f\n", gps_get()->latitude, gps_get()->longitude);
	}
	media.latitude = soma.latitude/(n-1);
	media.longitude = soma.longitude/(n-1);
	printf("Posicao atual media: %.8f %.8f\n", media.latitude, media.longitude);
	*/
	
	// Ver se so fazer isso e valido
	// Se nao, ver como usar o angulo da bussola e outras funcoes para melhorar o erro
	dist.latitude = testes[0].latitude - media.latitude; //eventos[0].pos.latitude
	dist.longitude = testes[0].longitude - media.longitude; //eventos[0].pos.longitude
	printf("Diferenca calculada: %f %f\n", dist.latitude, dist.longitude);
	
	// OBS.: criar novas variaveis para guardar as novas posicoes para deixar as medidas
	// iniciais constantes, como eventos_novo ou algo assim, e usar essas variaveis na 
	// main, ou entao criar eventos_inicial pra gravar as medicoes iniciais e continuar
	// usando eventos na main
	for (i = 0; i < len(testes); i++) { // Desloca todas as coordenadas
		testes[i].latitude -= dist.latitude; //eventos[i].pos.latitude
		testes[i].longitude -= dist.longitude; //eventos[i].pos.longitude 
	}
	
	nanosleep(&gps_wait, NULL);
	nanosleep(&gps_wait, NULL);

	// Mostra as novas distancias, ver se se aproxima de 0
	// Ver se o angulo para as outras coordenadas e valido
	// Ver se a distancia se aproxima de 0 perto das outras coordenadas
	for (;;) {
		printf("\nPosicao atual: %.8f %.8f\n", gps_get()->latitude, gps_get()->longitude);
		for (i = 0; i < len(testes); i++) {
			printf("--- Posicao %d\n", i);
			printf("Coordenadas: %.8f %.8f\n", testes[i].latitude, testes[i].longitude); //eventos[i].pos.latitude, eventos[i].pos.longitude
			printf("Azimuth: %f\n", azimuth(gps_get(), &testes[i])); //&eventos[i].pos)
			printf("Distancia: %f\n", gps_distance(gps_get(), &testes[i]));
			printf("Direção atual: %f\n", direcao_atual);
			printf("Diff %lf\n", compass_diff(azimuth(gps_get(), &testes[i]), direcao_atual)); //&eventos[i].pos)
		}
//		nanosleep(&gps_wait, NULL);
//		nanosleep(&gps_wait, NULL);
//		nanosleep(&gps_wait, NULL);
//		nanosleep(&gps_wait, NULL);
		// Possivel colocar um nanosleep aqui pra facilitar a leitura dos dados
	}
}
