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

	/*int * mouseAxis = Input::mouseAxis();
	float * mouseDelta = Input::mouseDelta();
	int * mouseWheel = Input::mouseWheelAxis();
	float * mouseWheelDelta = Input::mouseWheelDelta();

	std::cout << "Mouse position : " << "x " << mouseAxis[0] << " y " << mouseAxis[1] << std::endl;
	std::cout << "Mouse delta : " << "x " << mouseDelta[0] << " y " << mouseDelta[1] << std::endl;
	std::cout << "Mouse wheel position : " << "x " << mouseWheel[0] << " y " << mouseWheel[1] << std::endl;
	std::cout << "Mouse wheel delta : " << "x " << mouseWheelDelta[0] << "y " << mouseWheelDelta[1] << std::endl;*/
}