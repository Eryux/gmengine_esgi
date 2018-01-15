#pragma once

#ifndef ENGINE_INPUT_H
#define ENGINE_INPUT_H

#include <vector>
#include <map>

#include <SFML\Window\Event.hpp>
#include <SFML\Window\Keyboard.hpp>
#include <SFML\Window\Joystick.hpp>
#include <SFML\Window\Mouse.hpp>

namespace Engine {

	typedef struct {
		bool up = false;
		bool down = false;
		bool pressed = false;
	} keyState_t;

	typedef struct {
		bool up = false;
		bool down = false;
		bool pressed = false;
	} mouseButtonState_t;

	typedef struct {
		int axis[2] = { 0, 0 };
		int wheel[2] = { 0, 0 };;
		float delta[2] = { 0.f, 0.f };
		float wheel_delta[2] = { 0.f, 0.f };
	} mouseAxisState_t;

	class Input {

	private:
		static mouseAxisState_t m_mouseAxis;
		static std::map<sf::Keyboard::Key, keyState_t> m_bind_keys;
		static std::map<sf::Mouse::Button, mouseButtonState_t> m_bind_buttons;

	public:
		static bool keyDown(sf::Keyboard::Key key);
		static bool keyUp(sf::Keyboard::Key key);
		static bool keyPress(sf::Keyboard::Key key);

		static bool mouseDown(sf::Mouse::Button button);
		static bool mouseUp(sf::Mouse::Button button);
		static bool mousePress(sf::Mouse::Button button);

		static int * mouseAxis();
		static int * mouseWheelAxis();
		static float * mouseDelta();
		static float * mouseWheelDelta();

		static void bindKeyboard(sf::Keyboard::Key k);
		static void bindMouse(sf::Mouse::Button b);
		static void unbindKeyboard(sf::Keyboard::Key k);
		static void unbindMouse(sf::Mouse::Button b);

		static void updateKeyState(sf::Keyboard::Key k, sf::Event::EventType t);
		static void updateButtonState(sf::Mouse::Button b, sf::Event::EventType t);
		static void updateMouseAxisState(int x, int y);
		static void updateMouseWheelState(int x, int y);

		static void refresh();

	};

}

#endif