#include "SceneLoader.h"

#include "json.h"
#include "Skybox.h"

#include <iostream>
#include <fstream>
#include <map>
#include <memory>

using namespace Engine;

std::unordered_map<std::string, std::unordered_map<std::string, component_param_t>> SceneLoader::s_component_params;
std::unordered_map<std::string, std::size_t> SceneLoader::s_literal_to_hash;
std::unordered_map<std::type_index, std::size_t> SceneLoader::s_type_to_hash;

void SceneLoader::LoadScene(std::string scene_file) 
{
	Core * core = Core::Get();
	std::unordered_map<unsigned int, void *> s_instances;

	std::ifstream input_file(scene_file);
	nlohmann::json j; input_file >> j;

	nlohmann::json j_gameobjects = j["gameobjects"];
	for (auto gameobject_it = j_gameobjects.begin(); gameobject_it != j_gameobjects.end(); ++gameobject_it) 
	{
		nlohmann::json j_gameobject = gameobject_it.value();

		GameObject * o_gameobject = new GameObject(j_gameobject["name"].get<std::string>());
		s_instances[j_gameobject["id"].get<unsigned int>()] = o_gameobject;
		//o_gameobject->setActive(j_gameobject["enabled"].get<bool>());

		Transform * o_transform = o_gameobject->getTransform();
		nlohmann::json a_position = j_gameobject["position"];
		o_transform->setLocalPosition(glm::vec3(a_position[0].get<float>(), a_position[1].get<float>(), a_position[2].get<float>()));
		nlohmann::json a_rotation = j_gameobject["rotation"];
		o_transform->setLocalRotation(glm::vec3(a_rotation[0].get<float>(), a_rotation[1].get<float>(), a_rotation[2].get<float>()));
		nlohmann::json a_scale = j_gameobject["scale"];
		o_transform->setLocalSize(glm::vec3(a_scale[0].get<float>(), a_scale[1].get<float>(), a_scale[2].get<float>()));
		
		nlohmann::json j_components = j_gameobject["components"];

		// First pass for instanting component and default type populating
		for (auto components_it = j_components.begin(); components_it != j_components.end(); ++components_it)
		{
			nlohmann::json j_component = components_it.value();

			std::string s_type = j_component["type"].get<std::string>();

			void * o_component = Instantiate<void>(s_type);
			s_instances[j_component["id"].get<unsigned int>()] = o_component;
			Component * o_base_component = static_cast<Component*>(o_component);
			//o_base_component->setActive(j_component["enabled"].get<bool>());
			o_gameobject->addComponent(o_base_component);

			nlohmann::json j_params = j_component["params"];
			for (auto params_it = j_params.begin(); params_it != j_params.end(); ++params_it)
			{
				nlohmann::json j_param = params_it.value();
				if (j_param["instance"].get<bool>() == false)
				{
					component_param_t * ptr_param = &(s_component_params[s_type][j_param["name"].get<std::string>()]);
					
					auto type_hash_it = s_literal_to_hash.find(ptr_param->type);
					std::size_t type_hash = type_hash_it->second;
					if (type_hash == s_type_to_hash[std::type_index(typeid(int))]) {
						int * v_param = static_cast<int*>(ptr_param->param);
						*v_param = j_param["value"].get<int>();
					}
					else if (type_hash == s_type_to_hash[std::type_index(typeid(double))]) {
						double * v_param = static_cast<double*>(ptr_param->param);
						*v_param = j_param["value"].get<double>();
					}
					else if (type_hash == s_type_to_hash[std::type_index(typeid(float))]) {
						float * v_param = static_cast<float*>(ptr_param->param);
						*v_param = j_param["value"].get<float>();
					}
					else if (type_hash == s_type_to_hash[std::type_index(typeid(long))]) {
						long * v_param = static_cast<long*>(ptr_param->param);
						*v_param = j_param["value"].get<long>();
					}
					else if (type_hash == s_type_to_hash[std::type_index(typeid(unsigned int))]) {
						unsigned int * v_param = static_cast<unsigned int*>(ptr_param->param);
						*v_param = j_param["value"].get<unsigned int>();
					}
				}
			}
		}

		// Second pass for populate instiated component
		for (auto components_it = j_components.begin(); components_it != j_components.end(); ++components_it)
		{
			nlohmann::json j_component = components_it.value();
			std::string s_type = j_component["type"].get<std::string>();
			nlohmann::json j_params = j_component["params"];

			for (auto params_it = j_params.begin(); params_it != j_params.end(); ++params_it)
			{
				nlohmann::json j_param = params_it.value();
				if (j_param["instance"].get<bool>() == true)
				{
					component_param_t * ptr_param = &(s_component_params[s_type][j_param["name"].get<std::string>()]);
					unsigned int i_instance = j_param["value"].get<unsigned int>();
					SetInstanceParam(s_instances[i_instance], ptr_param);
				}
			}
		}

		core->m_gameObjects.push_back(o_gameobject);
	}
	
	if (core->m_skybox != nullptr) {
		core->m_skybox->loadTexture(j["skybox"].get<std::string>());
	}
}

void SceneLoader::SetInstanceParam(void * instance, component_param_t * param)
{
	auto type_hash_it = s_literal_to_hash.find(param->type);
	if (type_hash_it == s_literal_to_hash.end()) {
		std::cerr << "Type not exist or are not supported." << std::endl;
		return;
	}

	std::size_t type_hash = type_hash_it->second;

	if (type_hash == s_type_to_hash[std::type_index(typeid(GameObject*))]) {
		GameObject * o_instance = static_cast<GameObject*>(instance);
		GameObject ** ptr_param = static_cast<GameObject**>(param->param);
		*(ptr_param) = o_instance;
	}
	else if (type_hash == s_type_to_hash[std::type_index(typeid(Component*))]) {
		Component * o_instance = static_cast<Component*>(instance);
		Component ** ptr_param = static_cast<Component**>(param->param);
		*(ptr_param) = o_instance;
	}
	else if (type_hash == s_type_to_hash[std::type_index(typeid(Renderer*))]) {
		Renderer * o_instance = static_cast<Renderer*>(instance);
		Renderer ** ptr_param = static_cast<Renderer**>(param->param);
		*(ptr_param) = o_instance;
	}
	else if (type_hash == s_type_to_hash[std::type_index(typeid(Transform*))]) {
		Transform * o_instance = static_cast<Transform*>(instance);
		Transform ** ptr_param = static_cast<Transform**>(param->param);
		*(ptr_param) = o_instance;
	}
	else if (type_hash == s_type_to_hash[std::type_index(typeid(FPSCounter*))]) {
		FPSCounter * o_instance = static_cast<FPSCounter*>(instance);
		FPSCounter ** ptr_param = static_cast<FPSCounter**>(param->param);
		*(ptr_param) = o_instance;
	}
	else if (type_hash == s_type_to_hash[std::type_index(typeid(MoveCamera_Component*))]) {
		MoveCamera_Component * o_instance = static_cast<MoveCamera_Component*>(instance);
		MoveCamera_Component ** ptr_param = static_cast<MoveCamera_Component **>(param->param);
		*(ptr_param) = o_instance;
	}
	else if (type_hash == s_type_to_hash[std::type_index(typeid(Camera*))]) {
		Camera * o_instance = static_cast<Camera*>(instance);
		Camera ** ptr_param = static_cast<Camera**>(param->param);
		*(ptr_param) = o_instance;
	}
}


void SceneLoader::BindComponent(std::string component_type_name)
{
	s_component_params[component_type_name];
}

void SceneLoader::BindParam(std::string component_type_name, std::string param_name,
	void * param_ptr, std::string param_type_name) 
{
	component_param_t param;
	param.param = param_ptr;
	param.type = param_type_name;

	s_component_params[component_type_name][param_name] = param;
}

void SceneLoader::Init() 
{
	s_literal_to_hash[".H"] = typeid(int).hash_code();
	s_literal_to_hash[".N"] = typeid(double).hash_code();
	s_literal_to_hash[".M"] = typeid(float).hash_code();
	s_literal_to_hash[".J"] = typeid(long).hash_code();
	s_literal_to_hash[".I"] = typeid(unsigned int).hash_code();
	s_literal_to_hash[".PAH"] = typeid(int*).hash_code();
	s_literal_to_hash[".PAN"] = typeid(double*).hash_code();
	s_literal_to_hash[".PAM"] = typeid(float*).hash_code();
	s_literal_to_hash[".PAJ"] = typeid(long*).hash_code();
	s_literal_to_hash[".PAI"] = typeid(unsigned int*).hash_code();
	s_literal_to_hash[".PAVGameObject@Engine@@"]		= typeid(GameObject*).hash_code();
	s_literal_to_hash[".PAVComponent@Engine@@"]			= typeid(Component*).hash_code();
	s_literal_to_hash[".PAVRenderer@Engine@@"]			= typeid(Renderer*).hash_code();
	s_literal_to_hash[".PAVTransform@Engine@@"]			= typeid(Transform*).hash_code();
	s_literal_to_hash[".PAVFPSCounter@Engine@@"]		= typeid(FPSCounter*).hash_code();
	s_literal_to_hash[".PAVCamera@Engine@@"]			= typeid(Camera*).hash_code();
	s_literal_to_hash[".PAVMoveCamera_Component@Engine@@"] = typeid(MoveCamera_Component*).hash_code();


	s_type_to_hash[std::type_index(typeid(int))] = typeid(int).hash_code();
	s_type_to_hash[std::type_index(typeid(float))] = typeid(float).hash_code();
	s_type_to_hash[std::type_index(typeid(double))] = typeid(double).hash_code();
	s_type_to_hash[std::type_index(typeid(long))] = typeid(long).hash_code();
	s_type_to_hash[std::type_index(typeid(unsigned int))] = typeid(unsigned int).hash_code();

	s_type_to_hash[std::type_index(typeid(int*))] = typeid(int*).hash_code();
	s_type_to_hash[std::type_index(typeid(float*))] = typeid(float*).hash_code();
	s_type_to_hash[std::type_index(typeid(double*))] = typeid(double*).hash_code();
	s_type_to_hash[std::type_index(typeid(long*))] = typeid(long*).hash_code();
	s_type_to_hash[std::type_index(typeid(unsigned int*))] = typeid(unsigned int*).hash_code();

	s_type_to_hash[std::type_index(typeid(GameObject*))] = typeid(GameObject*).hash_code();
	s_type_to_hash[std::type_index(typeid(Component*))] = typeid(Component*).hash_code();
	s_type_to_hash[std::type_index(typeid(Renderer*))] = typeid(Renderer*).hash_code();
	s_type_to_hash[std::type_index(typeid(Transform*))] = typeid(Transform*).hash_code();
	s_type_to_hash[std::type_index(typeid(Camera*))] = typeid(Camera*).hash_code();

	s_type_to_hash[std::type_index(typeid(FPSCounter*))] = typeid(FPSCounter*).hash_code();
	s_type_to_hash[std::type_index(typeid(MoveCamera_Component*))] = typeid(MoveCamera_Component*).hash_code();
}

template<typename T>
T * SceneLoader::Instantiate(std::string type_name)
{

	auto type_hash_it = s_literal_to_hash.find(type_name);
	if (type_hash_it == s_literal_to_hash.end()) {
		std::cerr << "Type not exist or are not supported." << std::endl;
		return nullptr;
	}

	std::size_t type_hash = type_hash_it->second;

	T * r;
	
	if (type_hash == s_type_to_hash[std::type_index(typeid(int*))])
		r = new int;
	else if (type_hash == s_type_to_hash[std::type_index(typeid(double*))])
		r = new double;
	else if (type_hash == s_type_to_hash[std::type_index(typeid(float*))])
		r = new float;
	else if (type_hash == s_type_to_hash[std::type_index(typeid(long*))])
		r = new long;
	else if (type_hash == s_type_to_hash[std::type_index(typeid(unsigned int*))])
		r = new unsigned int;

	else if (type_hash == s_type_to_hash[std::type_index(typeid(GameObject*))])
		r = new GameObject("");
	else if (type_hash == s_type_to_hash[std::type_index(typeid(Renderer*))])
		r = new Renderer();
	else if (type_hash == s_type_to_hash[std::type_index(typeid(Camera*))])
		r = new Camera();
	else if (type_hash == s_type_to_hash[std::type_index(typeid(Transform*))])
		r = new Transform();
	else if (type_hash == s_type_to_hash[std::type_index(typeid(Component*))])
		r = new Component();

	else if (type_hash == s_type_to_hash[std::type_index(typeid(MoveCamera_Component*))])
		r = new MoveCamera_Component();
	else if (type_hash == s_type_to_hash[std::type_index(typeid(FPSCounter*))])
		r = new FPSCounter();
	else {
		r = nullptr;
	}

	return r;
}

void SceneLoader::ExtractTypename(std::string out_file) 
{
	std::ofstream write_file;
	write_file.open(out_file);

	write_file << "name,type" << std::endl;

	write_file << "int\t" << typeid(int).raw_name() << std::endl;
	write_file << "double\t" << typeid(double).raw_name() << std::endl;
	write_file << "float\t" << typeid(float).raw_name() << std::endl;
	write_file << "long\t" << typeid(long).raw_name() << std::endl;
	write_file << "uint\t" << typeid(unsigned int).raw_name() << std::endl;

	write_file << "int*\t" << typeid(int*).raw_name() << std::endl;
	write_file << "double*\t" << typeid(double*).raw_name() << std::endl;
	write_file << "float*\t" << typeid(float*).raw_name() << std::endl;
	write_file << "long*\t" << typeid(long*).raw_name() << std::endl;
	write_file << "uint*\t" << typeid(unsigned int*).raw_name() << std::endl;

	write_file << "gameobject\t" << typeid(GameObject*).raw_name() << std::endl;
	write_file << "component\t" << typeid(Component*).raw_name() << std::endl;
	write_file << "renderer\t" << typeid(Renderer*).raw_name() << std::endl;
	write_file << "transform\t" << typeid(Transform*).raw_name() << std::endl;
	write_file << "camera\t" << typeid(Camera*).raw_name() << std::endl;
	write_file << "movecamera\t" << typeid(MoveCamera_Component*).raw_name() << std::endl;
	write_file << "fpscounter\t" << typeid(FPSCounter*).raw_name() << std::endl;

	write_file.close();
}

/*
Visual Studio 2015 -----------------------
int		.H
double	.N
float	.M
long	.J
uint	.I
int*	.PAH
double*	.PAN
float*	.PAM
long*	.PAJ
uint*	.PAI
gameobject	.PAVGameObject@Engine@@
component	.PAVComponent@Engine@@
renderer	.PAVRenderer@Engine@@
transform	.PAVTransform@Engine@@
camera		.PAVCamera@Engine@@
movecamera	.PAVMoveCamera_Component@Engine@@
fpscounter	.PAVFPSCounter@Engine@@
*/