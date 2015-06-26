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
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>
#include <errno.h>

#include <poll.h>
#include <errno.h>

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
	fd = open(dev, O_RDONLY);
	if (fd == -1) {
		char error[30];
		fprintf(stderr, "Erro na abertura de %s:\t%s\n", dev, strerror_r(errno, error, sizeof(error)));
		return -1;
	}
	
	if (version_check(fd, &version))
		return -1;
	return fd;
}

int joystick_open_nb(char *dev) {
	int version, fd;
	fd = open(dev, O_RDONLY | O_NONBLOCK);
	if (fd == -1) {
		char error[30];
		fprintf(stderr, "Erro na abertura de %s:\t%s\n", dev, strerror_r(errno, error, sizeof(error)));
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
		perror(__FILE__ " - read");
		return -1;
		
		case 0:
		fprintf(stderr, __FILE__ " - unexpected EOF at joystick device\n");
		return -1;
		
		case sizeof(struct js_event):
		return 0;
		
		default:
		fprintf(stderr, __FILE__ " - joystick invalid packet\n");
		return -1;
	}
}

int joystick_read_nb(int fd, struct js_event *event) {
	struct pollfd joystick_poll = {.fd = fd, .events = POLLIN};
	switch (poll(&joystick_poll, 1, 0)) {
		case 0:
		return 0;
		
		case -1:
		return -1;
	}
	switch (read(fd, event, sizeof(struct js_event))) {
		case -1:
		if (errno == EAGAIN)
			return 0;
		perror(__FILE__ " - read");
		return -1;
		
		case 0:
		fprintf(stderr, __FILE__ " - unexpected EOF at joystick device\n");
		return -1;
		
		case sizeof(struct js_event):
		return 1;	// há eventos
		
		default:
		fprintf(stderr, __FILE__ " - joystick invalid packet\n");
		return -1;
	}
}
