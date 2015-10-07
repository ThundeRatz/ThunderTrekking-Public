#include "Bumper.hh"
#include <ctime.h >
int main(){
	Bumper bumper;
	timespec timer;
	timer.tv_sec = 0;
	timer.tv_nsec = 300000000;
	printf("Bumper\n");
	while(1)
		if(bumper.pressed())
			printf("Estado: 1\n");
		else {
			int i;
			nanosleep(timer, NULL);
		        printf("Estado: 0\n");
		}
	return 0;
}
