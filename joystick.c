/*******************************************************************************
 * 
 * Joystick para Linux
 * Interface por eventos requer driver 1.0+ (qualquer kernel recente é
 * suficiente).
 * 
 * Cheque https://www.kernel.org/doc/Documentation/input/joystick-api.txt para a
 * API e https://www.kernel.org/doc/Documentation/input/joystick.txt e
 * https://wiki.archlinux.org/index.php/joystick para configuração.
 * 
 * Se JOYSTICK_NONBLOCK estiver definido, abre o dispositivo com O_NONBLOCK.
 * 
 * Tiago Shibata
 * 
*******************************************************************************/

#include <stdint.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>

typedef struct js_event js_event_t;
/*{
	uint32_t time;	// timestamp em milisegundos.
					// Detalhe: não guarda o timestamp do sistema, mas um timestamp
					// a partir de um valor arbitrário do driver:
	//
	//	The time an event was generated is stored in ``js_event.time''. It's a time
	//	in milliseconds since ... well, since sometime in the past.  This eases the
	//	task of detecting double clicks, figuring out if movement of axis and button
	//	presses happened at the same time, and similar.
	int16_t value;	// para botão, 0 ou 1. Para eixo, -32767 a +32767.
					// Usar jscal caso esteja descalibrado.
	uint8_t type;
	uint8_t number;
}
*/

/*
 * 
 * int joystick_open()
 * 
 * Abre joystick, testa fd por versão compatível do driver e escreve na saída padrão
 * dados do controle. Retorna fd.
 * 
*/

int joystick_open(char *dev) {
	int version, fd;
	char name[128], axes, buttons;
	
#ifdef JOYSTICK_NONBLOCK
	#warning __FILE__ in NONBLOCK mode
	fd = open(dev, O_RDONLY | O_NONBLOCK);
#else
	fd = open(dev, O_RDONLY);
#endif
	
	if (fd == -1) {
		fprintf(stderr, "Erro na abertura de %s:\t", dev);
		perror("open");
		return -1;
	}
	
	if (ioctl(fd, JSIOCGVERSION, &version) < 0) {
		fprintf(stderr, "Versão do driver < 1.0, abortando\n");
		return -1;
	}
	
	fprintf(stderr, "Versão 0x%x\n", version);
	
	if (ioctl(fd, JSIOCGNAME(sizeof(name)), name) < 0) {
		fprintf(stderr, "Falha na leitura do nome\n");
		return -1;
	}
	fprintf(stderr, "Nome: %s\n", name);
	
	if (ioctl(fd, JSIOCGAXES, &axes) < 0) {
		fprintf(stderr, "Falha na leitura do número de eixos\n");
		return -1;
	}
	fprintf(stderr, "%d eixos\n", axes);
	
	if (ioctl(fd, JSIOCGBUTTONS, &buttons) < 0) {
		fprintf(stderr, "Falha na leitura do número de botões\n");
		return -1;
	}
	fprintf(stderr, "%d botões\n", buttons);
	return fd;
}

/*
 * 
 * int joystick_read(int fd, struct js_event *event)
 * 
 * Lê de fd para event. Retorna -1 para erro e escreve na saída de erro o motivo,
 * 1 se há eventos e 0 se não há (apenas se JOYSTICK_NONBLOCK estiver definido)
 * 
*/

int joystick_read(int fd, js_event_t *event) {
	switch (read(fd, event, sizeof(js_event_t))) {
		case -1:
#ifdef JOYSTICK_NONBLOCK
		if (errno == EAGAIN)
			return 0;
#endif
		perror(__FILE__ " - read");
		return -1;
		case 0:
		fprintf(stderr, __FILE__ " - unexpected EOF at joystick device\n");
		return -1;
		case sizeof(js_event_t):
		return 1;	// há eventos
		default:
		fprintf(stderr, __FILE__ " - joystick invalid packet\n");
		return -1;
	}
}
