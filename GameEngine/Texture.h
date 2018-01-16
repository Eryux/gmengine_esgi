#pragma once

#ifndef ENGINE_TEXTURE_H
#define ENGINE_TEXTURE_H

#include <SFML/Graphics/Image.hpp>
#include "Engine.h"

namespace Engine {

	class Texture {

	public:
		
		static int LoadTexture(std::string file) {
			sf::Image * text = new sf::Image();

			if (!text->loadFromFile(file)) {
				return -1;
			}

			GLuint buffer_index;
			glGenTextures(1, &buffer_index);
			glBindTexture(GL_TEXTURE_2D, buffer_index);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, text->getSize().x, text->getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, text->getPixelsPtr());
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);

			Core * core = Core::Get();
			core->m_textures.push_back(buffer_index);
			delete text;
			return core->m_textures.size() - 1;
		}

	};

}

#endif // !ENGINE_TEXTURE_H
