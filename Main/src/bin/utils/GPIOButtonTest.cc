#include <iostream>

#include "GPIOButton.hh"

int main() {
	Trekking::GPIOButton bumper166(166);
	Trekking::GPIOButton bumper165(165);
	int b165, b166, backup165 = 0, backup166 = 0;
	for (;;){
		b165 = bumper165;
		if(b165 != backup165) {
			printf("Bumper 165: %d\n", b165); 
			backup165 = b165;
		}
		b166 = bumper166;
		if(b166 != backup166) {
			printf("Bumper 166: %d\n", b166); 
			backup166 = b166;
		}
	}
	return 0;
}
