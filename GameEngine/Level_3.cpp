#include "Level_3.h"

#include "SceneLoader.h"
#include "RigidBody.h"

using namespace Engine;

Level_3::Level_3()
{
	SceneLoader::BindComponent(".PAVLevel_3@Engine@@");
	m_core = Core::Get();
}

void Level_3::start()
{

}

void Level_3::update()
{
	if (m_timer + 1.f < m_core->m_Time)
	{
		m_timer = m_core->m_Time;

		GameObject * g = new GameObject("pull_object");

		Renderer * r;
		if (i % 2 == 0)
			r = new Renderer("..\\Ressources\\Models\\pcube.obj", "..\\Ressources\\Models\\");
		else
			r = new Renderer("..\\Ressources\\Models\\psphere.obj", "..\\Ressources\\Models\\");
		r->SetMaterial(0);
		r->SetShader(m_core->m_shaders.GetShader(0));
		g->addComponent(r);

		RigidBody * p = new RigidBody();
		p->m_param_mass = 1.0f;
		if (i % 2 == 0) {
			p->m_param_shape = 1;
			float * m = new float[3]{ 1.f, 1.f, 1.f };
			p->m_param_shape_param = m;
		}
		else {
			p->m_param_shape = 2;
			float * m = new float[3]{ 1.f, 1.f, 1.f };
			p->m_param_shape_param = m;
		}
		g->addComponent(p);

		float rnd = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		float rnd2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		Transform * t = g->getTransform();
		t->setLocalPosition(glm::vec3(rnd * 10.f, 20.f, rnd2 * 10.f));

		m_core->m_gameObjects.push_back(g);
		i++;
	}
}