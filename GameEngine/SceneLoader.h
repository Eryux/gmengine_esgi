#pragma once

#ifndef ENGINE_SCENELOADER_H
#define ENGINE_SCENELOADER_H

#include <map>

#include "Engine.h"

namespace Engine {

	struct {
		void * param;
		std::string type;
	} typedef component_param_t;

	class SceneLoader {

	private:
		// Desc - std::type_info::raw_name (component type), map<string (param name), param def>
		static std::map<std::string, std::map<std::string, component_param_t>> s_component_params;

	public:
		static void LoadScene(std::string scene_file);

		static void BindComponent(std::string component_type_name);

		static void BindParam(std::string component_type_name, std::string param_name, void * param_ptr, std::string param_type_name);

		template<typename T>
		static T * Instantiate(std::string type_name);

	};

}

#endif