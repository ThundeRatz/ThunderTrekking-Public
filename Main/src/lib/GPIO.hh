#pragma once

#include <string>

#include <poll.h>

namespace Trekking {
	class GPIO {
		public:
			static const std::string IN, OUT_LOW, OUT_HIGH;
			static const std::string EDGE_NONE, EDGE_RISE, EDGE_FALL,
				EDGE_BOTH;
			GPIO(int gpio);
			~GPIO();
			void export_gpio();
			int poll(int timeout);
			void poll();
			void direction(const std::string &direction);
			void active_low();
			void edge(const std::string &edge_type);
			void operator=(int value);
			operator bool() const;

		private:
			// value_fd é salvo por ser o mais acessado e para não abrir e
			// fechar toda a hora
			int fd_value;
			int gpio;
			struct pollfd poll_targets;
			void write_to_file(const std::string& name, const std::string& value);
			bool exported();
	};
}
