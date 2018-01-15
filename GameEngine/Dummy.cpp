#include "Dummy.h"

#include <iostream>
#include "Input.h"

using namespace Engine;

void Dummy::start() {
	std::cout << "Test class started !" << std::endl;
	Input::bindKeyboard(sf::Keyboard::Key::Z);
	Input::bindMouse(sf::Mouse::Button::Left);
}

void Dummy::update()
{
	if (Input::keyDown(sf::Keyboard::Key::Z)) {
		std::cout << "Z is down !" << std::endl;
	}
	if (Input::keyUp(sf::Keyboard::Key::Z)) {
		std::cout << "Z is up !" << std::endl;
	}
	if (Input::keyPress(sf::Keyboard::Key::Z)) {
		std::cout << "Z is pressed !" << std::endl;
	}

	if (Input::mouseDown(sf::Mouse::Button::Left)) {
		std::cout << "Click down !" << std::endl;
	}
	if (Input::mouseUp(sf::Mouse::Button::Left)) {
		std::cout << "Click up !" << std::endl;
	}
	if (Input::mousePress(sf::Mouse::Button::Left)) {
		std::cout << "Click pressed !" << std::endl;
	}
}