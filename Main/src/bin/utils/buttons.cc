#include <iostream>

#include "Button.hh"

using namespace Trekking;

int main() {
	Button button;
	char keycode;

	while ((keycode = button.blocking_update(10000)))
		std::cout << "Botão " << keycode << " (" << +keycode << ")" << std::endl;

	return 0;
}
