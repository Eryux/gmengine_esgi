#pragma once

#ifndef ENGINE_LEVEL_3_H
#define ENGINE_LEVEL_3_H

#include "Component.h"
#include "Engine.h"

namespace Engine
{


	class Level_3 : public Component {

	private:

		int i = 0;

		Core * m_core;

		float m_timer = 0.0f;

	public:

		Level_3();

		void start();

		void update();

	};

}


#endif