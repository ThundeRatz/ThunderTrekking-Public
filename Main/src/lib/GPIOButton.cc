/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 ThundeRatz

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

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
			while (gpio_button->button.poll(gpio_button->switch_debounce)) ;
			gpio_button->value = gpio_button->button;
		}
	}

	GPIOButton::GPIOButton(int gpio, int switch_debounce) : button(gpio), thread(this) {
		button.direction(GPIO::IN);
		button.edge(GPIO::EDGE_BOTH);
		this->switch_debounce = switch_debounce;
	}

	GPIOButton::operator bool() const {
		return value;
	}

	GPIOButton::~GPIOButton() {
		running = false;
		thread.join();
	}
}
