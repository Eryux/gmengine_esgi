#pragma once

#ifndef ENGINE_SCENELOADER_H
#define ENGINE_SCENELOADER_H

#include <unordered_map>
#include <typeindex>

#include "Engine.h"

#include "GUI_FPSCounter.h"
#include "MoveCamera_Component.h"
#include "Renderer.h"

namespace Engine {

	struct {
		void * param;
		std::string type;
	} typedef component_param_t;

	class SceneLoader {

	private:
		// Desc - std::type_info::raw_name (component type), map<string (param name), param def>
		static std::unordered_map<std::string, std::unordered_map<std::string, component_param_t>> s_component_params;

		// Hash can change between application runs but hash comparaison is faster than string
		// Raw name to hash
		static std::unordered_map<std::string, std::size_t> s_literal_to_hash;

		// type index to hash
		static std::unordered_map<std::type_index, std::size_t> s_type_to_hash;

		static void SetInstanceParam(void * instance, component_param_t * param);
	public:
		static void Init();

		static void LoadScene(std::string scene_file);

		static void BindComponent(std::string component_type_name);

		static void BindParam(std::string component_type_name, std::string param_name, void * param_ptr, std::string param_type_name);

		template<typename T>
		static T * Instantiate(std::string type_name);

		static void ExtractTypename(std::string out_file);

	};

}

#endif