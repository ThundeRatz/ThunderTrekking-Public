#include <exception>

#include "TooManyInstances.hh"

const char* TooManyInstances::what() const throw() {
	return "Too many instances";
}
