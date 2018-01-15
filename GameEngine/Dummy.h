#pragma once

#ifndef ENGINE_DUMMY_H
#define ENGINE_DUMMY_H

#include "Component.h"

namespace Engine {

	class Dummy : public Component {

	public:
		void start();
		void update();

	};

}

#endif