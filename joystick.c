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
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>

#ifdef JOYSTICK_NONBLOCK
#include <errno.h>
#endif

/* struct js_event:
{
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

inline static int version_check(int fd, int *version) {
	if (ioctl(fd, JSIOCGVERSION, version) < 0) {
		fprintf(stderr, "Joystick - versão do driver < 1.0, abortando\n");
		return -1;
	}
	return 0;
}

/*
 * 
 * int joystick_open()
 * 
 * Abre joystick e testa fd por versão compatível do driver. Retorna fd.
 * 
*/

int joystick_open(char *dev) {
	int version, fd;
#ifdef JOYSTICK_NONBLOCK
	fd = open(dev, O_RDONLY | O_NONBLOCK);
#else	
	fd = open(dev, O_RDONLY);
#endif
	if (fd == -1) {
		fprintf(stderr, "Erro na abertura de %s:\t", dev);
		perror("open");
		return -1;
	}
	
	if (version_check(fd, &version))
		return -1;
	return fd;
}

void joystick_dump(int fd) {
	int version;
	char name[128];
	uint8_t axes, buttons;
	if (version_check(fd, &version))
		return;
	printf("Versão 0x%x\n", version);
	
	if (ioctl(fd, JSIOCGNAME(sizeof(name)), name) < 0)
		printf("Falha na leitura do nome\n");
	else
		printf("Nome: %s\n", name);
	
	if (ioctl(fd, JSIOCGAXES, &axes) < 0)
		printf("Falha na leitura do número de eixos\n");
	else
		printf("%d eixos\n", axes);
	
	if (ioctl(fd, JSIOCGBUTTONS, &buttons) < 0)
		printf("Falha na leitura do número de botões\n");
	else
		printf("%d botões\n", buttons);
}

/*
 * 
 * int joystick_read(int fd, struct js_event *event)
 * 
 * Lê de fd para event. Retorna -1 para erro e escreve na saída de erro o motivo,
 * 1 se há eventos e 0 se não há (apenas se JOYSTICK_NONBLOCK estiver definido)
 * 
*/

int joystick_read(int fd, struct js_event *event) {
	switch (read(fd, event, sizeof(struct js_event))) {
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
		case sizeof(struct js_event):
#ifdef JOYSTICK_NONBLOCK
		return 1;	// há eventos
#else
		return 0;
#endif
		default:
		fprintf(stderr, __FILE__ " - joystick invalid packet\n");
		return -1;
	}
}
