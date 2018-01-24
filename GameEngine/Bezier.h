#pragma once

#include "Engine.h"
#include "Renderer.h"

#ifndef ENGINE_BEZIER_H
#define ENGINE_BEZIER_H

namespace Engine {


	class Math : public Component
	{

	private:

		std::vector<glm::vec3> m_control_points;

		std::vector<glm::vec3> m_curve_points;

		model_t m_surface;

	public:

		int m_subdivision[2] = { 2, 2 };

		Renderer * m_renderer;

		std::string m_data_filename;

		int m_precision = 3;

		bool m_show_controlpoint = false;

		Math();

		~Math();

		void start();

		// IO

		void read_json_data(std::string filename);

		// Surface

		void surface_calc();

		void set_show_points(bool show_control_points);
		

		// Math

		int factorial(int n);

		float bernstein(int i, int n, int t);

		glm::vec3 de_castle_jau(std::vector<glm::vec3> points, int degree, float t);

	};

}


#endif