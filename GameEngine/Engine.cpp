#include "Engine.h"
#include "Renderer.h"
#include "Input.h"
#include "Texture.h"
#include "Skybox.h"
#include "SceneLoader.h"
#include "Physic.h"

#include "MoveCamera_Component.h"
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

void Core::Init()
{
	std::cout << "Current working directory : " << GetCurrentWorkingDir() << std::endl;

	// OpenGL Context -------------------
	m_window = new sf::RenderWindow(sf::VideoMode(1600,900), "D4MN Engine", sf::Style::Default, sf::ContextSettings(32));
	m_window->setVerticalSyncEnabled(true);
	m_window->setFramerateLimit(60);

	glewInit();

	// Load Shaders ----------------------
	g_LAMBER_SHADER = m_shaders.LoadShader("..\\Ressources\\Shaders\\lambert.vs", "..\\Ressources\\Shaders\\lambert.fs");
	int skybox_shader = m_shaders.LoadShader("..\\Ressources\\Shaders\\skybox.vs", "..\\Ressources\\Shaders\\skybox.fs");

	// Create skybox
	m_skybox = new Skybox();
	m_skybox->setShader(m_shaders.GetProgram(skybox_shader));

	// Physx init ------------------------
	Physic::Init();

	// Scene creations -------------------
	SceneLoader::Init();

	InitScene();
	SceneLoader::LoadScene("..\\Ressources\\Scenes\\dragon.json");
}

void Core::LoadScene(std::string path)
{
	m_scene_file = path;
	m_waitForLoadScene = true;
}

void Core::InitScene()
{
	// Init physx scene
	Physic::InitScene();
	Physic::s_scene->setGravity(physx::PxVec3(0.f, -9.81f, 0.f));

	// Load main scene
	//SceneLoader::Init();
	//SceneLoader::LoadScene("..\\Ressources\\Scenes\\dragon.json");

	// ADDITIONAL SCENE OBJECT (DEBUG ONLY) ---------------------------------

	// FPS Counter
	/*GameObject * fpsc_obj = new GameObject("fpscounter");
	FPSCounter * fpsc_component = new FPSCounter();
	fpsc_component->SetFont("..\\Ressources\\Fonts\\arial.ttf");
	fpsc_obj->addComponent(fpsc_component);
	m_gameObjects.push_back(fpsc_obj);*/
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
	deltaTimeClock.restart();

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
		// Create / Delete pending object -----------------------
		for (int i = 0; i < m_gameObjects.size(); i++) {
			if (m_gameObjects[i]->getState() == GameObjectState::ENABLE) 
			{
				std::vector<Component*> components = m_gameObjects[i]->getComponents();
				for (int j = 0; j < components.size(); j++) {
					Component * c = components[j];
					if (c->getState() == ComponentState::INIT) {
						c->start();
						c->setActive(true);
					}
					else if (c->getState() == ComponentState::PENDING_REMOVE) {
						m_gameObjects[i]->removeComponent(j);
					}
				}
			}
			else if (m_gameObjects[i]->getState() == GameObjectState::PENDING_DESTROY) {
				m_gameObjects[i]->destroy();
			}
		}

		if (m_waitForLoadScene) {
			ClearScene();
			InitScene();
			SceneLoader::LoadScene(m_scene_file);
			m_waitForLoadScene = false;
			continue; // Jump render for current frame to let new object start before
		}

		Input::refresh();

		// Events --------------------------------------------
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
				m_window->setSize(sf::Vector2u(event.size.width, event.size.height));
				m_camera->m_ratio = (float) event.size.width / (float) event.size.height;
			}
			else if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) 
			{
				if (event.key.code == sf::Keyboard::Key::Escape && event.type == sf::Event::KeyPressed) {
					running = false;
				}
				if (event.key.code == sf::Keyboard::Key::F11 && event.type == sf::Event::KeyPressed) {
					if (wireframe)
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					else
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					wireframe = !wireframe;
				}
				if (event.key.code == sf::Keyboard::Key::F1 && event.type == sf::Event::KeyPressed) {
					LoadScene("..\\Ressources\\Scenes\\main.json");
				}
				if (event.key.code == sf::Keyboard::Key::F2 && event.type == sf::Event::KeyPressed) {
					LoadScene("..\\Ressources\\Scenes\\dragon.json");
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

		// Update --------------------------------------------
		int size_objects = m_gameObjects.size();
		for (int i = 0; i < size_objects; i++) {
			if (m_gameObjects[i]->getState() == GameObjectState::ENABLE)
			{
				std::vector<Component*> components = m_gameObjects[i]->getComponents();
				int size_components = components.size();
				for (int j = 0; j < size_components; j++) {
					if (components[j]->getState() == ComponentState::ENABLED) {
						components[j]->update();
					}
				}
			}
		}

		m_window->clear();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// DRAW --------------------------------------------

		// Draw skybox
		m_skybox->draw();

		// Draw 3D
		int size_r = m_renderers.size();
		for (int i = 0; i < size_r; i++) {
			if (m_renderers[i]->getState() == ComponentState::ENABLED)
				m_renderers[i]->draw();
		}

		// Simulate Physic
		Physic::Run(m_deltaTime);

		// Draw GUI
		m_window->pushGLStates();
		size_objects = m_gameObjects.size();
		for (int i = 0; i < size_objects; i++) {
			if (m_gameObjects[i]->getState() == GameObjectState::ENABLE) {
				std::vector<Component*> components = m_gameObjects[i]->getComponents();
				int size_components = components.size();
				for (int j = 0; j < size_components; j++) {
					if (components[j]->getState() == ComponentState::ENABLED) {
						components[j]->gui_draw(m_window);
					}
				}
			}
		}
		m_window->popGLStates();

		// FPS lock --------------------------------------------
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
}

void Core::ClearScene()
{
	for (int i = 0; i < m_vbo.size(); i++) {
		glDeleteBuffers(1, &m_vbo[i]);
		glDeleteBuffers(1, &m_ibo[i]);
	}
	m_ibo.clear();
	m_vbo.clear();

	Renderer::FreeModelAll();
	m_renderers.clear();

	for (int i = 0; i < m_gameObjects.size(); i++) {
		m_gameObjects[i]->destroy();
		delete m_gameObjects[i];
	}
	m_gameObjects.clear();

	Transform::freeActor();
	Physic::FreeScene();
}

void Core::Free()
{
	ClearScene();
	delete m_skybox;

	m_shaders.Free();
	delete m_window;

	Physic::Free();
}