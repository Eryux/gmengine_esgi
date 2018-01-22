#include <iostream>
#include <fstream>
#include <map>
#include <memory>
#include <typeindex>

#include "Engine.h"
#include "SceneLoader.h"

Engine::Core * Engine::Core::m_instance = nullptr;

int main()
{
	// Engine::SceneLoader::ExtractTypename("../supported_type.txt");

	std::cout << "Load core function ..." << std::endl;
	Engine::Core * dmn_engine = Engine::Core::Get();

	dmn_engine->Init();

	std::cout << "Start engine ..." << std::endl;
	dmn_engine->Run();

	std::cout << "Stop engine ..." << std::endl;

	dmn_engine->Free();
	Engine::Core::Kill();

	std::cout << "Engine stopped." << std::endl;
	
	std::cout << "Press ENTER to quit ... " << std::endl;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	return 0;
}