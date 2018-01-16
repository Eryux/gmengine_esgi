#include "Engine.h"
#include "Renderer.h"
#include "Dummy.h"
#include "Input.h"
#include "MoveCamera_Component.h"
#include "Texture.h"
#include "Skybox.h"

#include "GUI_FPSCounter.h"

#include <SFML/System/Time.hpp>

#include <iostream>

#include <stdio.h>
#include <direct.h>
#define GetCurrentDir _getcwd

std::string GetCurrentWorkingDir(void) {
	char buff[FILENAME_MAX];
	GetCurrentDir(buff, FILENAME_MAX);
	std::string current_working_dir(buff);
	return current_working_dir;
}

using namespace Engine;

int g_LAMBER_SHADER = -1;

void Core::AddRenderer(Renderer *r) 
{
	m_renderers.push_back(r);
}

void Core::RemoveRenderer(Renderer *r)
{
	int number_of_erased = 0;
	for (unsigned int i = 0; i < m_renderers.size(); i++) {
		if (m_renderers[i] == r) {
			m_renderers[i] = nullptr;
		}

		if (m_renderers[i] == nullptr) {
			number_of_erased++;
		}
	}

	// If number of erased renderer is > 100 we re-create clean vector
	if (number_of_erased > 100) {
		std::vector<Renderer*> new_renderers;
		new_renderers.reserve(m_renderers.size() - number_of_erased + 16);
		for (unsigned int i = 0, j = 0; i < m_renderers.size(); i++) {
			if (m_renderers[i] != nullptr) {
				new_renderers[j] = m_renderers[i];
				j++;
			}
		}
		m_renderers = new_renderers;
	}
}

void Core::InitScene()
{
	// Camera ---------------------------------
	
	GameObject * main_camera = new GameObject();
	m_gameObjects.push_back(main_camera);

	m_camera = new Camera();
	m_camera->m_fov = 70.0f;
	m_camera->m_near_plane = 0.1f;
	m_camera->m_far_plane = 10000.f;
	m_camera->m_ratio = 800.0f / 600.0f;
	main_camera->addComponent(m_camera);

	MoveCamera_Component * camera_move_cp = new MoveCamera_Component();
	camera_move_cp->m_speed = 3.f;
	main_camera->addComponent(camera_move_cp);

	main_camera->getTransform()->setLocalPosition(glm::vec3(0.0f, 5.0f, -5.0f));

	// 3D Object ---------------------------

	// Dragon
	GameObject * dragon_obj = new GameObject();
	m_gameObjects.push_back(dragon_obj);

	Renderer * dragon_renderer = new Renderer("..\\Ressources\\Models\\alduin\\alduin-dragon.obj", "..\\Ressources\\Models\\alduin\\");
	dragon_renderer->SetShader(m_shaders.GetShader(g_LAMBER_SHADER));
	dragon_renderer->SetMaterial(0);
	dragon_obj->addComponent(dragon_renderer);
	dragon_obj->getTransform()->setLocalPosition(glm::vec3(1.2f, 3.5f, 0.0f));
	dragon_obj->getTransform()->setLocalSize(glm::vec3(0.005f, 0.005f, 0.005f));
	dragon_obj->getTransform()->setLocalRotation(glm::rotate(dragon_obj->getTransform()->getLocalRotation(), glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f)));

	int dragon_texture = Texture::LoadTexture("..\\Ressources\\Models\\alduin\\alduin.jpg");
	dragon_renderer->SetTexture(dragon_texture);

	// Floor
	GameObject * floor_obj = new GameObject();
	m_gameObjects.push_back(floor_obj);

	Renderer * floor_renderer = new Renderer("..\\Ressources\\Models\\pcube.obj", "..\\Ressources\\Models\\");
	floor_renderer->SetShader(m_shaders.GetShader(g_LAMBER_SHADER));
	floor_renderer->SetMaterial(0);
	floor_obj->addComponent(floor_renderer);
	floor_obj->getTransform()->setLocalPosition(glm::vec3(0.0f, -0.05f, 0.00f));
	floor_obj->getTransform()->setLocalSize(glm::vec3(100.0f, 0.1f, 100.0f));

	// Table
	GameObject * table_obj = new GameObject();
	m_gameObjects.push_back(table_obj);

	Renderer * table_renderer = new Renderer("..\\Ressources\\Models\\pcube.obj", "..\\Ressources\\Models\\");
	table_renderer->SetShader(m_shaders.GetShader(g_LAMBER_SHADER));
	table_renderer->SetMaterial(0);
	table_obj->addComponent(table_renderer);
	table_obj->getTransform()->setLocalPosition(glm::vec3(0.0f, 1.2f, 0.0f));
	table_obj->getTransform()->setLocalSize(glm::vec3(1.0f, 1.2f, 1.0f));
}

void Core::Init()
{
	std::cout << "Current working directory : " << GetCurrentWorkingDir() << std::endl;

	// OpenGL Context -------------------
	m_window = new sf::RenderWindow(sf::VideoMode(800, 600), "D4MN Engine", sf::Style::Default, sf::ContextSettings(32));
	m_window->setVerticalSyncEnabled(true);
	m_window->setFramerateLimit(60);

	glewInit();

	// Load Shaders ----------------------
	g_LAMBER_SHADER = m_shaders.LoadShader("..\\Ressources\\Shaders\\lambert.vs", "..\\Ressources\\Shaders\\lambert.fs");
	int skybox_shader = m_shaders.LoadShader("..\\Ressources\\Shaders\\skybox.vs", "..\\Ressources\\Shaders\\skybox.fs");

	// Create skybox
	m_skybox = new Skybox();
	m_skybox->loadTexture("..\\Ressources\\Textures\\skybox");
	m_skybox->setShader(m_shaders.GetProgram(skybox_shader));

	// Scene creations -------------------
	InitScene();

	// Scene init -----------------------
	for (int i = 0; i < m_gameObjects.size(); i++) {
		std::vector<Component*> components = m_gameObjects[i]->getComponents();
		for (int j = 0; j < components.size(); j++) {
			components[j]->start();
		}
		m_gameObjects[i]->setActive(true);
	}
}

int Core::CreateGLBuffer(GLfloat * vertices, GLuint * indexes, unsigned int size_v, unsigned int size_i)
{
	GLuint VBO, IBO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size_v * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_i * sizeof(GLuint), indexes, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_vbo.push_back(VBO);
	m_ibo.push_back(IBO);

	return (m_vbo.size() - 1);
}

void Core::Run()
{
	sf::Clock timeClock;
	timeClock.restart();

	sf::Clock deltaTimeClock;

	Engine_GUI::FPSCounter * gui_fpsCounter = new Engine_GUI::FPSCounter();
	gui_fpsCounter->SetFont("..\\Ressources\\Fonts\\arial.ttf");
	float last_FPSrefresh = 0.0f;

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_TEXTURE_2D);

	bool wireframe = false;

	bool running = true;
	while (running)
	{
		Input::refresh();

		// Refresh FPS counter only every sec.
		if (last_FPSrefresh + 1.f < m_Time) {
			gui_fpsCounter->SetFPS(1.f / m_deltaTime);
			last_FPSrefresh = m_Time;
		}

		// gestion des évènements
		sf::Event event;
		while (m_window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				running = false;
			}
			else if (event.type == sf::Event::Resized)
			{
				glViewport(0, 0, event.size.width, event.size.height);
				m_camera->m_ratio = (float) event.size.width / (float) event.size.height;
			}
			else if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) 
			{
				if (event.key.code == sf::Keyboard::Key::Escape && event.type == sf::Event::KeyPressed) {
					running = false;
				}
				if (event.key.code == sf::Keyboard::Key::F11 && event.type == sf::Event::KeyPressed) {
					if (wireframe) {
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					}
					else {
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					}
					wireframe = !wireframe;
				}

				Input::updateKeyState(event.key.code, event.type);
			}
			else if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased) 
			{
				Input::updateButtonState(event.mouseButton.button, event.type);
			}
			else if (event.type == sf::Event::MouseMoved) {
				Input::updateMouseAxisState(event.mouseMove.x, event.mouseMove.y);
			}
			else if (event.type == sf::Event::MouseWheelMoved) {
				Input::updateMouseWheelState(event.mouseWheel.x, event.mouseWheel.y);
			}
		}

		// Update
		int size_objects = m_gameObjects.size();
		for (int i = 0; i < size_objects; i++) {
			std::vector<Component*> components = m_gameObjects[i]->getComponents();
			int size_components = components.size();
			for (int j = 0; j < size_components; j++) {
				components[j]->update();
			}
		}

		// effacement les tampons de couleur/profondeur
		m_window->clear();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw skybox
		m_skybox->draw();

		// Draw 3D
		int size_r = m_renderers.size();
		for (int i = 0; i < size_r; i++) {
			m_renderers[i]->draw();
		}

		// Draw GUI
		m_window->pushGLStates();
		gui_fpsCounter->Draw(m_window);
		m_window->popGLStates();

		// termine la trame courante (en interne, échange les deux tampons de rendu)
		m_window->display();

		// Update time
		m_deltaTime = deltaTimeClock.restart().asSeconds();
		m_Time = timeClock.getElapsedTime().asSeconds();
	}

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDisable(GL_POLYGON_SMOOTH);
	glDisable(GL_TEXTURE_2D);

	delete gui_fpsCounter;
}

void Core::Free()
{
	for (int i = 0; i < m_vbo.size(); i++) {
		glDeleteBuffers(1, &m_vbo[i]);
		glDeleteBuffers(1, &m_ibo[i]);
	}

	m_shaders.Free();

	for (int i = 0; i < m_gameObjects.size(); i++) {
		m_gameObjects[i]->destroy();
		delete m_gameObjects[i];
	}

	delete m_window;
}