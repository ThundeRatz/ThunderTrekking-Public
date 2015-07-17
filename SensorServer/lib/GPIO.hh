#pragma once

#include <string>

#include <poll.h>

namespace Trekking {
	using namespace std;

	class GPIO {
		public:
			typedef enum {
				GPIO_IN,
				GPIO_OUT_LOW,
				GPIO_OUT_HIGH
			} Direction;
			typedef enum {
				GPIO_EDGE_NONE,
				GPIO_EDGE_RISE,
				GPIO_EDGE_FALL,
				GPIO_EDGE_BOTH
			} Edge;
			GPIO(int gpio);
			~GPIO();
			void export_gpio();
			int poll(int timeout);
			void poll();
			void direction(Direction direction);
			void active_low();
			void edge(Edge edge_type);
			void operator=(int value);
			int get();

		private:
			// value_fd é salvo por ser o mais acessado e para não abrir e
			// fechar toda a hora
			int fd_value;
			int gpio;
			struct pollfd poll_targets;
			void write_to_file(string name, string value);
			bool exported();
	};
}
