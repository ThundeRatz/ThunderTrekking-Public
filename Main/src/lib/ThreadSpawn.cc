#include <system_error>
#include <iostream>
#include <thread>

void thread_spawn(void new_thread()) {
	try {
		std::thread t(new_thread);
		if(t.joinable())
			t.detach();
	} catch (std::system_error& e) {
		std::cerr << "Thread error: " << e.what() << std::endl;
		exit(-1);
	}
}
