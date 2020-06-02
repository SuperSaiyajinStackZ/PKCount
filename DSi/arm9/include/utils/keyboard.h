#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <string>

namespace Input {
	std::string getLine(std::string inputText);
	std::string getLine(uint maxLength, std::string inputText);
}

#endif