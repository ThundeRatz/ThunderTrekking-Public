#pragma once

#include <string>

namespace Trekking {
	class Leds {
	public:
		Leds(const std::string& name);
		~Leds();
		void operator=(int brightness);
		void set_trigger(const std::string& trigger);
		void set_property(const std::string& name, const std::string& value);

	private:
		const std::string path;
		int brightness_fd;
	};
}
