#include "Bezier.h"

#include "json.h"

#include <iostream>
#include <fstream>

using namespace Engine;

Math::Math()
{

}

Math::~Math() 
{

}

void Math::start()
{
	read_json_data(m_data_filename);

	std::cout << "Control points -----------------" << std::endl;
	for (int i = 0; i < m_control_points.size(); i++) {
		std::cout << "x " << m_control_points[i].x << " y " << m_control_points[i].y << " z " << m_control_points[i].z << std::endl;
	}
	
	surface_calc();

	std::cout << "Curve points -----------------" << std::endl;
	for (int i = 0; i < m_curve_points.size(); i++) {
		std::cout << "x " << m_curve_points[i].x << " y " << m_curve_points[i].y << " z " << m_curve_points[i].z << std::endl;
	}
}

// Surface

void Math::surface_calc()
{
	int n = 0;
	for (int i = 0; i <= m_precision; i++)
	{
		float u = (float)i / (float)m_precision;


		for (int j = 0; j <= m_precision; j++)
		{
			float v = (float)j / (float) m_precision;

			std::vector<glm::vec3> q_points;
			for (int q = 0; q <= m_subdivision[0]; q++) 
			{
				std::vector<glm::vec3> line_points;
				for (int z = 0; z < m_subdivision[1]; z++)
					line_points.push_back(m_control_points[q * m_subdivision[1] + z]);

				q_points.push_back(de_castle_jau(line_points, m_subdivision[1], v));
			}

			m_curve_points.push_back(de_castle_jau(q_points, m_subdivision[0], u));
		}
	}
}

// IO

void Math::read_json_data(std::string filename)
{
	std::ifstream input_file(filename);
	nlohmann::json j; input_file >> j;

	std::vector<int> j_size = j["size"].get<std::vector<int>>();
	m_subdivision[0] = j_size[0];
	m_subdivision[1] = j_size[1];

	nlohmann::json j_points = j["control_points"];
	for (auto points_it = j_points.begin(); points_it != j_points.end(); ++points_it)
	{
		std::vector<float> r_coord = points_it.value().get<std::vector<float>>();
		glm::vec3 coord(r_coord[0], r_coord[1], r_coord[2]);
		m_control_points.push_back(coord);
	}

	input_file.close();
}


// Math

int Math::factorial(int n)
{
	int r = 1;
	for (int i = n; i>0; i--) { r *= i; }
	return r;
}

float Math::bernstein(int i, int n, int t)
{
	float r = (float)factorial(n) / (float)(factorial(i) * factorial(n - i));
	r *= pow(t, i);
	r *= pow(1 - t, n - i);
	return r;
}

glm::vec3 Math::de_castle_jau(std::vector<glm::vec3> points, int degree, float t)
{
	std::vector<glm::vec3> q_points;
	for (int j = 0; j < degree; j++) { q_points.push_back(points[j]); }

	for (int k = 1; k <= degree; k++)
	{
		for (int j = 0; j < degree - k; j++)
		{
			q_points[j] = glm::vec3(
				(1 - t) * q_points[j].x + t * q_points[j + 1].x,
				(1 - t) * q_points[j].y + t * q_points[j + 1].y,
				(1 - t) * q_points[j].z + t * q_points[j + 1].z
			);
		}
	}

	return glm::vec3(q_points[0].x, q_points[0].y, q_points[0].z);
}