#include <cstring>
#include <errno.h>

#include "errno_string.hh"

errno_string::errno_string() {
	error_string = strerror_r(errno, error_string_buffer, sizeof(error_string_buffer));
}

errno_string::errno_string(int error) {
	error_string = strerror_r(error, error_string_buffer, sizeof(error_string_buffer));
}

errno_string::operator char* () {
	return error_string;
}
