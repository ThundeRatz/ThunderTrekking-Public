#pragma once

#include <string>

#include <poll.h>

namespace Trekking {
	class GPIO {
		public:
			static const std::string GPIO_IN, GPIO_OUT_LOW, GPIO_OUT_HIGH;
			static const std::string GPIO_EDGE_NONE, GPIO_EDGE_RISE, GPIO_EDGE_FALL,
				GPIO_EDGE_BOTH;
			GPIO(int gpio);
			~GPIO();
			void export_gpio();
			int poll(int timeout);
			void poll();
			void direction(const std::string &direction);
			void active_low();
			void edge(const std::string &edge_type);
			void operator=(int value);
			operator bool const();

		private:
			// value_fd é salvo por ser o mais acessado e para não abrir e
			// fechar toda a hora
			int fd_value;
			int gpio;
			struct pollfd poll_targets;
			void write_to_file(const std::string& name, const std::string& value);
			bool exported();
	};
	const std::string GPIO::GPIO_IN = "in", GPIO::GPIO_OUT_LOW = "low",
		GPIO::GPIO_OUT_HIGH = "high";
	const std::string GPIO_EDGE_NONE = "none", GPIO_EDGE_RISE = "rise",
		GPIO_EDGE_FALL = "fall", GPIO_EDGE_BOTH = "both";
}
