#include "GPIOButton.hh"

namespace Trekking {
	// Could be optimized to run one thread and poll() on a fd set

	GPIOButtonThread::GPIOButtonThread(GPIOButton* gpio_button) : thread(GPIOButtonThread::run, gpio_button){}

	void GPIOButtonThread::join() {
		thread.join();
	}

	/// This is a static method
	void GPIOButtonThread::run(GPIOButton* gpio_button) {
		while (gpio_button->running) {
			gpio_button->button.poll();
			while (gpio_button->button.poll(gpio_button->debounce)) ;
			gpio_button->value = gpio_button->button;
		}
	}

	GPIOButton::GPIOButton(int gpio, int switch_debounce) : button(gpio), thread(this) {
		button.direction(GPIO::IN);
		button.edge(GPIO::EDGE_BOTH);
		this->debounce = switch_debounce;
	}

	GPIOButton::operator bool() const {
		return value;
	}

	GPIOButton::~GPIOButton() {
		running = false;
		thread.join();
	}
}
