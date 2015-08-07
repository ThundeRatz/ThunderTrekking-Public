#pragma once

#include <thread>

#include "GPIO.hh"

namespace Trekking {
	class GPIOButton;

	class GPIOButtonThread {
	public:
		GPIOButtonThread(GPIOButton* gpio_button);
		void join();

	private:
		std::thread thread;
		static void run(GPIOButton* gpio_button);
	};

	class GPIOButton {
		friend GPIOButtonThread;
	public:
		GPIOButton(int gpio, int switch_debounce = 50);
		~GPIOButton();
		operator bool() const;

	private:
		GPIO button;
		GPIOButtonThread thread;
		bool running = true, value = false;
		int debounce;
	};
}
