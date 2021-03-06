#include "Printm.h"

std::string Printm(char _type, std::string _message, int lvl, bool visible) {
	std::stringstream buffer;
	if (_type == 'e') {
		buffer << std::setw(lvl) << "ERROR!!\t" << _message << std::endl;
		std::cout << buffer.str();
		exit(1);
	}
	else if (_type == 'i') buffer << std::setw(lvl * 10) << "INFO:\t" << _message << std::endl;
	else if (_type == 'w') buffer << std::setw(lvl) << "WARNING!\t" << _message << std::endl;
	else if (_type == 'r') buffer << std::setw(lvl) << "\t" <<_message << std::endl;
	else if (_type == 'c') buffer << "\033[2J\033[1;1H";
	if (visible) std::cout << buffer.str();
	return buffer.str();
}