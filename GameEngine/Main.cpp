#include <iostream>
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
	/*std::ofstream write_file;
	write_file.open("typeinfo_name.txt");

	Engine::MoveCamera_Component * a;

	bindType(typeid(Engine::MoveCamera_Component *).raw_name(), &typecast_MoveCamera<Engine::MoveCamera_Component>);
	void * func = g_cast_functions[".PAVMoveCamera_Component@Engine@@"];
	a = reinterpret_cast<Engine::MoveCamera_Component*>(reinterpret_cast<long>(func));

	write_file << typeid(float).raw_name() << std::endl;
	write_file << typeid(a).name() << std::endl;
	write_file.close();*/

	//Engine::SceneLoader::ExtractTypename("../supported_type.txt");

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