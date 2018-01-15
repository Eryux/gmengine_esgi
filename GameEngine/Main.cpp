#include <iostream>

#include "Engine.h"

Engine::Core * Engine::Core::m_instance = nullptr;

int main()
{
	std::cout << "Load core function ..." << std::endl;
	Engine::Core * dmn_engine = Engine::Core::Get();

	dmn_engine->Init();

	std::cout << "Start engine ..." << std::endl;
	dmn_engine->Run();

	std::cout << "Stop engine ..." << std::endl;

	dmn_engine->Free();
	Engine::Core::Kill();

	std::cout << "Engine stopped." << std::endl;
	return 0;
}