#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <vector>

class Keyboard
{
public:
	Keyboard();

//private:
	std::vector<std::vector<char>> letters;
};

Keyboard::Keyboard()
{
	letters.push_back(std::vector<char>());
	letters[0].push_back('1');
	letters[0].push_back('2');
	letters[0].push_back('3');
	letters[0].push_back('4');
	letters[0].push_back('5');
	letters[0].push_back('6');
	letters[0].push_back('7');
	letters[0].push_back('8');
	letters[0].push_back('9');
	letters[0].push_back('0');
}

#endif 