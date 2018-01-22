#include "AudioPlayer.h"
#include "SceneLoader.h"

using namespace Engine;

std::unordered_map<std::string, sf::SoundBuffer*> AudioPlayer::s_sound_buffers;

AudioPlayer::AudioPlayer()
{
	SceneLoader::BindComponent(".PAVAudioPlayer@Engine@@");
	SceneLoader::BindParam(".PAVAudioPlayer@Engine@@", "m_sound_file", &m_sound_file, ".STRING");
	SceneLoader::BindParam(".PAVAudioPlayer@Engine@@", "m_sound_type", &m_sound_type, ".I");
	SceneLoader::BindParam(".PAVAudioPlayer@Engine@@", "m_volume", &m_volume, ".M");
	SceneLoader::BindParam(".PAVAudioPlayer@Engine@@", "m_loop", &m_loop, "._N");
	SceneLoader::BindParam(".PAVAudioPlayer@Engine@@", "m_autoplay", &m_autoplay, "._N");

	m_sound = new sf::Sound();
	m_music = new sf::Music();
}

void AudioPlayer::remove()
{
	m_sound->stop();
	m_sound->resetBuffer();
	m_music->stop();
	delete m_sound;
	delete m_music;
}

void AudioPlayer::start()
{
	if (m_sound_type == 0)
	{
		sf::SoundBuffer * ptr_s;
		auto it = s_sound_buffers.find(m_sound_file);
		if (it == s_sound_buffers.end())
		{
			ptr_s = new sf::SoundBuffer();
			ptr_s->loadFromFile(m_sound_file);
		}
		else 
		{
			ptr_s = it->second;
		}

		m_sound->setBuffer(*ptr_s);
		m_sound->setLoop(m_loop);
		m_sound->setVolume(m_volume);
	}
	else
	{
		m_music->openFromFile(m_sound_file);
		m_music->setLoop(m_loop);
		m_music->setVolume(m_volume);
	}

	if (m_autoplay) Play();
}

void AudioPlayer::Play()
{
	if (m_sound_type == 0)
		m_sound->play();
	else
		m_music->play();
}

void AudioPlayer::Stop()
{
	if (m_sound_type == 0)
		m_sound->stop();
	else
		m_music->stop();
}

void AudioPlayer::Pause()
{
	if (m_sound_type == 0)
		m_sound->pause();
	else
		m_music->pause();
}

void AudioPlayer::PlayAt(float offset)
{
	if (m_sound_type == 0)
		m_sound->setPlayingOffset(sf::seconds(offset));
	else
		m_music->setPlayingOffset(sf::seconds(offset));
}

void AudioPlayer::SetVolume(float v)
{
	m_volume = v;
	if (m_sound_type == 0)
		m_sound->setVolume(m_volume);
	else
		m_sound->setVolume(m_volume);
}

float AudioPlayer::getVolume()
{
	return m_volume;
}

void AudioPlayer::Free()
{
	for (auto it = s_sound_buffers.begin(); it != s_sound_buffers.end(); ++it)
	{
		delete it->second;
	}

	s_sound_buffers.clear();
}