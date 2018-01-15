#include "Input.h"

using namespace Engine;

mouseAxisState_t Input::m_mouseAxis;
std::map<sf::Keyboard::Key, keyState_t> Input::m_bind_keys;
std::map<sf::Mouse::Button, mouseButtonState_t> Input::m_bind_buttons;

bool Input::keyDown(sf::Keyboard::Key key) {
	return Input::m_bind_keys[key].down;
}

bool Input::keyUp(sf::Keyboard::Key key) {
	return Input::m_bind_keys[key].up;
}

bool Input::keyPress(sf::Keyboard::Key key) {
	return Input::m_bind_keys[key].pressed;
}

bool Input::mouseDown(sf::Mouse::Button button) {
	return Input::m_bind_buttons[button].down;
}

bool Input::mouseUp(sf::Mouse::Button button) {
	return Input::m_bind_buttons[button].up;
}

bool Input::mousePress(sf::Mouse::Button button) {
	return Input::m_bind_buttons[button].pressed;
}

int * Input::mouseAxis() {
	return Input::m_mouseAxis.axis;
}

int * Input::mouseWheelAxis() {
	return Input::m_mouseAxis.wheel;
}

float * Input::mouseDelta() {
	return Input::m_mouseAxis.delta;
}

float * Input::mouseWheelDelta() {
	return Input::m_mouseAxis.wheel_delta;
}

void Input::bindKeyboard(sf::Keyboard::Key k) {
	keyState_t s;
	Input::m_bind_keys[k] = s;
}

void Input::bindMouse(sf::Mouse::Button b) {
	mouseButtonState_t s;
	Input::m_bind_buttons[b] = s;
}

void Input::unbindKeyboard(sf::Keyboard::Key k) {
	Input::m_bind_keys.erase(k);
}

void Input::unbindMouse(sf::Mouse::Button b) {
	Input::m_bind_buttons.erase(b);
}

void Input::updateKeyState(sf::Keyboard::Key k, sf::Event::EventType t) {
	keyState_t * state = &(Input::m_bind_keys[k]);

	if (t == sf::Event::EventType::KeyPressed) {
		state->down = state->pressed == false;
		state->pressed = true;
		state->up = false;
	}
	else if (t == sf::Event::EventType::KeyReleased) {
		state->up = state->pressed == true;
		state->pressed = false;
		state->down = false;
	}
}

void Input::updateButtonState(sf::Mouse::Button b, sf::Event::EventType t) {
	mouseButtonState_t * state = &(Input::m_bind_buttons[b]);

	if (t == sf::Event::EventType::MouseButtonPressed) {
		state->down = state->pressed == false;
		state->pressed = true;
		state->up = false;
	}
	else if (t == sf::Event::EventType::MouseButtonReleased) {
		state->up = state->pressed == true;
		state->pressed = false;
		state->down = false;
	}
}

void Input::refresh() 
{
	for (auto it = Input::m_bind_keys.begin(); it != Input::m_bind_keys.end(); ++it) {
		it->second.down = false;
		it->second.up = false;
	}

	for (auto it = Input::m_bind_buttons.begin(); it != Input::m_bind_buttons.end(); ++it) {
		it->second.down = false;
		it->second.up = false;
	}
}