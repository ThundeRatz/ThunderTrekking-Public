#include "Bumper.hh"
#include <ctime.h >
int main(){
	Bumper bumper;
	printf("Bumper\n");
	while(!(bumper.pressed()));
	while(1)
		if(bumper.pressed())
			printf("Estado: 1\n");
		else {
		        printf("Estado: 0\n");
		}
	return 0;
}
