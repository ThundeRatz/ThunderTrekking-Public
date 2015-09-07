#pragma once

class errno_string {
public:
	errno_string();
	errno_string(int error);
	operator char* ();

private:
	char error_string_buffer[20];
	char *error_string;
};
