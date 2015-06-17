#pragma once

class TooManyInstances : public std::exception {
	virtual const char* what() const throw();
};
