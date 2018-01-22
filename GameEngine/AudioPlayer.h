#pragma once

#ifndef ENGINE_AUDIOPLAYER_H
#define ENGINE_AUDIOPLAYER_H

#include "Component.h"

#include <unordered_map>
#include <SFML/Audio.hpp>

namespace Engine
{

	class AudioPlayer : public Component {

	private:

		static std::unordered_map<std::string, sf::SoundBuffer*> s_sound_buffers;

		sf::Sound * m_sound;

		sf::Music * m_music;

		float m_volume = 50;

		bool m_loop = false;

		bool m_autoplay = false;

	public:
		unsigned int m_sound_type = 0; // 0 = sound, 1 = music

		std::string m_sound_file;

		AudioPlayer();

		void remove();

		void start();

		void Play();

		void Stop();

		void Pause();

		void PlayAt(float offset);

		void SetVolume(float v);

		float getVolume();

		static void Free();

	};

}

#endif