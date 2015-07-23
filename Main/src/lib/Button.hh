#pragma once

#include <mutex>

#include <unistd.h>
#include <termios.h>

namespace Trekking {
	class Button {
		class LowLevelButtonSetup {
		private:
			struct termios default_terminal;
		public:
			LowLevelButtonSetup();
			~LowLevelButtonSetup();
		};
	private:
		static LowLevelButtonSetup button_initializer;
		static std::mutex mutex_button;
	public:
		Button();
		~Button();
		void free();
		char update();
		char blocking_update();
		char blocking_update(int timeout);
	};
}
