#include "GUI_FPSCounter.h"

#include <iomanip>
#include <sstream>
#include <iostream>

using namespace Engine;

FPSCounter::FPSCounter() {
	bool r_load = m_font.loadFromFile(m_font_file);
	if (r_load) {
		m_text.setFont(m_font);
	}

	m_text.setString("00.0 FPS");
	m_text.setColor(sf::Color::Red);
	m_text.setStyle(sf::Text::Bold);
	m_text.setCharacterSize(12);
	m_text.setPosition(sf::Vector2f(10.f, 10.f));
}

void FPSCounter::SetFPS(float fps) {
	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << fps << " FPS";
	m_text.setString(stream.str());
}

void FPSCounter::SetFont(std::string font_path) {
	bool r_load = m_font.loadFromFile(font_path);
	if (r_load) {
		m_text.setFont(m_font);
	}
}

void FPSCounter::gui_draw(sf::RenderWindow * window) {
	window->draw(m_text);
}

void FPSCounter::start() {
	Component::start();
	m_last_refresh = 0.0f;
	m_core = Core::Get();
}

void FPSCounter::update() {
	if (m_last_refresh + 1.f < m_core->m_Time) {
		SetFPS(1.f / m_core->m_deltaTime);
		m_last_refresh = m_core->m_Time;
	}
}