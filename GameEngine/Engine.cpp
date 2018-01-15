#include "Engine.h"
#include "Renderer.h"
#include "Dummy.h"
#include "Input.h"
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
	m_window = new sf::RenderWindow(sf::VideoMode(800, 600), "D4MN Engine", sf::Style::Default, sf::ContextSettings(32));
	m_window->setVerticalSyncEnabled(true);
	m_window->setFramerateLimit(60);

	glewInit();

	// Load Shaders ----------------------
	g_LAMBER_SHADER = m_shaders.LoadShader("..\\Ressources\\Shaders\\lambert.vs", "..\\Ressources\\Shaders\\lambert.fs");

	// Scene creations -------------------
	GameObject * main_camera = new GameObject();
	m_camera = new Camera();
	m_camera->m_fov = 70.0f;
	m_camera->m_near_plane = 0.1f;
	m_camera->m_far_plane = 10000.f;
	m_camera->m_ratio = 800.0f / 600.0f;
	main_camera->addComponent(m_camera);
	main_camera->getTransform()->setLocalPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	m_gameObjects.push_back(main_camera);

	GameObject * simple_3D_object = new GameObject();
	Renderer * simple_3D_object_renderer = new Renderer("..\\Ressources\\Models\\pcube.obj", "..\\Ressources\\Models\\");
	//Renderer * simple_3D_object_renderer = new Renderer("..\\Ressources\\Models\\pplane.obj", "..\\Ressources\\Models");
	//Renderer * simple_3D_object_renderer = new Renderer("..\\Ressources\\Models\\alduin\\alduin-dragon.obj", "..\\Ressources\\Models\\alduin\\");
	//Renderer * simple_3D_object_renderer = new Renderer("..\\Ressources\\Models\\charizard\\charizard-pokemon-go.obj", "..\\Ressources\\Models\\charizard");
	simple_3D_object_renderer->SetShader(m_shaders.GetShader(g_LAMBER_SHADER));
	simple_3D_object_renderer->SetMaterial(0);

	simple_3D_object->addComponent(simple_3D_object_renderer);
	simple_3D_object->getTransform()->setLocalPosition(glm::vec3(0.0f, 0.0f, 5.0f));
	simple_3D_object->getTransform()->setLocalSize(glm::vec3(1.0f, 2.0f, 1.0f));
	m_gameObjects.push_back(simple_3D_object);

	simple_3D_object = new GameObject();
	simple_3D_object_renderer = new Renderer("..\\Ressources\\Models\\pcube.obj", "..\\Ressources\\Models\\");
	simple_3D_object_renderer->SetShader(m_shaders.GetShader(g_LAMBER_SHADER));
	simple_3D_object_renderer->SetMaterial(0);
	simple_3D_object->addComponent(simple_3D_object_renderer);
	simple_3D_object->getTransform()->setLocalPosition(glm::vec3(5.0f, 0.0f, 5.0f));
	m_gameObjects.push_back(simple_3D_object);

	simple_3D_object = new GameObject();
	simple_3D_object_renderer = new Renderer("..\\Ressources\\Models\\pcube.obj", "..\\Ressources\\Models\\");
	simple_3D_object_renderer->SetShader(m_shaders.GetShader(g_LAMBER_SHADER));
	simple_3D_object_renderer->SetMaterial(0);
	simple_3D_object->addComponent(simple_3D_object_renderer);
	simple_3D_object->getTransform()->setLocalPosition(glm::vec3(-5.0f, 0.0f, 5.0f));
	m_gameObjects.push_back(simple_3D_object);

	GameObject * test_object = new GameObject();
	//Dummy * test_object_component = new Dummy();
	//test_object->addComponent(test_object_component);
	MoveCamera_Component * camera_move_cp = new MoveCamera_Component();
	camera_move_cp->m_speed = 3.f;
	test_object->addComponent(camera_move_cp);
	m_gameObjects.push_back(test_object);

	// Scene init -----------------------
	for (int i = 0; i < m_gameObjects.size(); i++) {
		std::vector<Component*> components = m_gameObjects[i]->getComponents();
		for (int j = 0; j < components.size(); j++) {
			components[j]->start();
		}
		m_gameObjects[i]->setActive(true);
	}

	/*model_t * simple_3D_model = simple_3D_object_renderer->GetModel();
	for (int i = 0; i < simple_3D_model->shapes.size(); i++) {
		std::cout << "Model " << i << " name : " << simple_3D_model->shapes[i].name << std::endl;

		std::cout << "Model " << i << " vertices : " << simple_3D_model->data->vertices.size() << std::endl;
		std::cout << "Model " << i << " normals : " << simple_3D_model->data->normals.size() << std::endl;
		std::cout << "Model " << i << " uvs : " << simple_3D_model->data->texcoords.size() << std::endl;

		std::cout << "Mesh ID size : " << simple_3D_model->shapes[i].mesh.indices.size() << std::endl;
		std::cout << "----------------------------" << std::endl;
		for (int i = 0; i < simple_3D_model->data->vertices.size(); i += 3) {
			std::cout << "x " << simple_3D_model->data->vertices[i] << " y " << simple_3D_model->data->vertices[i + 1] << " z " << simple_3D_model->data->vertices[i + 2] << std::endl;
		}
		std::cout << "----------------------------" << std::endl;
		for (int i = 0; i < simple_3D_model->shapes[0].mesh.indices.size(); i++) {
			//std::cout << simple_3D_model->shapes[0].mesh.indices[i].vertex_index << std::endl;
			std::cout << "x " << simple_3D_model->vertices[simple_3D_model->shapes[0].mesh.indices[i].vertex_index * 3] << " y " << simple_3D_model->vertices[simple_3D_model->shapes[0].mesh.indices[i].vertex_index * 3 + 1] << " z " << simple_3D_model->vertices[simple_3D_model->shapes[0].mesh.indices[i].vertex_index * 3 + 2] << std::endl;
		}
		std::cout << "----------------------------" << std::endl;
		for (int i = 0; i < simple_3D_model->data->vertices.size(); i++) {
			std::cout << "r " << simple_3D_model->data->vertices[i] << std::endl;
		}
		std::cout << "----------------------------" << std::endl;
		std::cout << simple_3D_model->vertices_size / 8.0f << std::endl;
		for (int i = 0; i < simple_3D_model->vertices_size; i += 8) {
			std::cout << i <<  " - v " << simple_3D_model->vertices[i] << " " << simple_3D_model->vertices[i + 1] << " " << simple_3D_model->vertices[i + 2]
				<< " n " << simple_3D_model->vertices[i + 3] << " " << simple_3D_model->vertices[i + 4] << " " << simple_3D_model->vertices[i + 5]
				<< " t " << simple_3D_model->vertices[i + 6] << " " << simple_3D_model->vertices[i + 7] << std::endl;
		}
		std::cout << "----------------------------" << std::endl;
		for (int i = 0; i < simple_3D_model->indexes_size; i++) {
			std::cout << simple_3D_model->indexes[i] << " - ";
		}
		std::cout << std::endl;
	}*/

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
				Input::updateKeyState(event.key.code, event.type);
			}
			else if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased) {
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
		glEnable(GL_DEPTH_TEST);

		// Draw 3D
		int size_r = m_renderers.size();
		for (int i = 0; i < size_r; i++) {
			m_renderers[i]->draw();
		}

		glDisable(GL_DEPTH_TEST);

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