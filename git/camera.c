#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>

#include <linux/videodev2.h>

#define v4l2_ioctl(fd, cmd, arg)	if (ioctl(fd, cmd, arg) == -1) {perror(#cmd); close(fd); abort();}

static void lista_menu(int fd, struct v4l2_queryctrl *queryctrl) {
	struct v4l2_querymenu querymenu;
	//printf("\tMenu itens:\n");
	memset(&querymenu, 0, sizeof(querymenu));
	querymenu.id = queryctrl->id;
	for (querymenu.index = queryctrl->minimum; querymenu.index <= queryctrl->maximum; querymenu.index++) {
		if (ioctl(fd, VIDIOC_QUERYMENU, &querymenu)) {
			if (errno != EINVAL)
				perror("VIDIOC_QUERYMENU");
			//abort();
			return;
		}
		printf("\t-%s\n", querymenu.name);
	}
}

void cam_init(char *dev) {
	int fd = open(dev, O_RDONLY), index;
	struct v4l2_input input;
	struct v4l2_standard standard;
	struct v4l2_queryctrl queryctrl;
	struct v4l2_capability cap;
	
	if (fd == -1) {
		perror("open");
		abort();
	}
	
	v4l2_ioctl(fd, VIDIOC_G_INPUT, &index);
	
	memset(&input, 0, sizeof(input));
	v4l2_ioctl(fd, VIDIOC_ENUMINPUT, &input);
	printf("Entrada de vídeo %s atual (%d): %s\n", dev, index, input.name);
	
	v4l2_ioctl(fd, VIDIOC_QUERYCAP, &cap);
	if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) || !(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
		fprintf(stderr, "%s não suporta captura de vídeo\n", dev);
		abort();
	}
		
	
	// para múltiplas câmeras no mesmo dispositivo, usar aqui VIDIOC_S_INPUT para escolher entrada
	
	printf("Standards:\n");
	// lista modos de entrada e saída. Geralmente usados em dispositivos usando standards para sinais analógicos
	// (NTSC, PAL), mas deixado aqui para deixar o código completo e listar caso alguém conecte uma câmera com
	// vários modos de operação
	memset(&standard, 0, sizeof(standard));
	standard.index = 0;
	while (!ioctl(fd, VIDIOC_ENUMSTD, &standard)) {
		if (standard.id & input.std)
			printf("%s\n", standard.name);
	}
	
	printf("Controles:\n");
	memset(&queryctrl, 0, sizeof(queryctrl));
	for (queryctrl.id = V4L2_CID_BASE; queryctrl.id < V4L2_CID_LASTP1; queryctrl.id++) {
		if (ioctl(fd, VIDIOC_QUERYCTRL, &queryctrl)) {
			if (errno == EINVAL)
				continue;	// câmera não suporta esse controle
			perror("VIDIOC_QUERYCTRL");
			break;
			//abort();
		} else {
			if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED)
				continue;
			printf("Controle %s (%d) [%d, %d]\n", queryctrl.name, queryctrl.id, queryctrl.minimum, queryctrl.maximum);
			lista_menu(fd, &queryctrl);
		}
	}
	
	for (queryctrl.id = V4L2_CID_PRIVATE_BASE;; queryctrl.id++) {
		if (ioctl(fd, VIDIOC_QUERYCTRL, &queryctrl)) {
			if (errno == EINVAL)
				break;
			perror("VIDIOC_QUERYCTRL");
			break;
			//abort();
		} else {
			if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED)
				continue;
			printf ("Controle %s (%d) [%d, %d]\n", queryctrl.name, queryctrl.id, queryctrl.minimum, queryctrl.maximum);
			if (queryctrl.type == V4L2_CTRL_TYPE_MENU)
				lista_menu(fd, &queryctrl);
		}
	}
}

void cam_setcap(int fd, int32_t cap, int32_t value) {
	struct v4l2_control control;
	
	memset(&control, 0, sizeof(control));
	control.id = cap;
	control.value = value;
	if (ioctl(fd, VIDIOC_S_CTRL, &control) == -1) {
		fprintf(stderr, "cam_setcap %d, %d\n", cap, value);
		perror("VIDIOC_S_CTRL");
	}
}
#warning ver formatos
#warning ver scaling/cropping
#warning streaming params

int main() {
	char dev[50];
	printf("Dispositivo: ");
	scanf("%s", dev);
	cam_init(dev);
	return -1;
}
