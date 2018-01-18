#pragma once

#ifndef ENGINE_GUI_FPS_COUNTER_H
#define ENGINE_GUI_FPS_COUNTER_H

#include "Engine.h"
#include "Component.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

namespace Engine {

	class FPSCounter : public Component {

	private:
		std::string m_font_file = "..\\Ressources\\Fonts\\arial.ttf";

		sf::Font m_font;
		
		sf::Text m_text;

		float m_last_refresh = 0.0f;

		Core * m_core;

	public:
		FPSCounter();

		void SetFPS(float fps);

		void SetFont(std::string font_path);

		void start();

		void update();

		void gui_draw(sf::RenderWindow * w);

	};

}

#endif