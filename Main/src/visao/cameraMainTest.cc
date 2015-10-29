#include "camera.hh"
#include "ThreadMotors.hh"
#include "ThreadSpawn.hh"
#include "sleep.hh"
#include "errno_string.hh"
int main(){
	thread_spawn(motors_thread);
	Camera camera;
	int i;
	while(1) {
		camera.showBiggest();
		i = camera.getBiggest();
		printf("%d\n", camera.getXContour(i));
		camera.followContour(i);
	}
	return 0;
}
