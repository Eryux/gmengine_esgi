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


		/*sf::Font font;
		if (!font.loadFromFile("arial.ttf"))
		{
			// erreur...
		}

		sf::Text text;

		// choix de la police à utiliser
		text.setFont(font); // font est un sf::Font

							// choix de la chaîne de caractères à afficher
		text.setString("Hello world");

		// choix de la taille des caractères
		text.setCharacterSize(24); // exprimée en pixels, pas en points !

								   // choix de la couleur du texte
		text.setColor(sf::Color::Red);

		// choix du style du texte
		text.setStyle(sf::Text::Bold | sf::Text::Underlined);

		...

			// puis, dans la boucle de dessin, entre window.clear() et window.display()
			window.draw(text);*/

	};

}

#endif