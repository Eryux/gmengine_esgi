#pragma once

#ifndef ENGINE_GUI_FPS_COUNTER_H
#define ENGINE_GUI_FPS_COUNTER_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

namespace Engine_GUI {

	class FPSCounter {

	private:
		std::string m_font_file = "..\\Ressources\\Fonts\\arial.ttf";

		sf::Font m_font;
		
		sf::Text m_text;

	public:
		FPSCounter();

		void SetFPS(float fps);

		void SetFont(std::string font_path);

		void Draw(sf::RenderWindow * w);

	};

}

#endif