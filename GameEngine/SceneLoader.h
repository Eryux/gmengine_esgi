#pragma once

#ifndef ENGINE_SCENELOADER_H
#define ENGINE_SCENELOADER_H

#include <map>

#include "Engine.h"

namespace Engine {

	struct {
		void * param;
		std::type_info type;
	} typedef component_param_t;

	class SceneLoader {

	private:
		static std::map<std::string, std::type_info> s_component_types;
		static std::map<void *, std::map<std::string, component_param_t>> s_component_params;

		static void bindComponentParam(void * instance, std::string param_name, void * param, std::type_info type);
		static void runtimeCast(void * value, std::type_info type);
	public:
		static void bindComponent(std::string component_name, std::type_info type);
		static void LoadScene(std::string scene_file);

	};

}

#endif