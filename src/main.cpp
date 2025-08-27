#include "Core/EnvironmentInitializer.hpp"
#include "Core/Simulation.hpp"

#include <fstream>
#include <iostream>

int main(int argc, char** argv)
{
	using namespace sw;

	if (argc != 2)
	{
		throw std::runtime_error("Error: No file specified in command line argument");
	}

	std::ifstream file(argv[1]);
	if (!file)
	{
		throw std::runtime_error("Error: File not found - " + std::string(argv[1]));
	}

	core::EnvironmentInitializer env_init;
	env_init.Init(file);

	std::cout << "\n\nEvents:\n";

	Simulation sim(env_init.GetEnvironment());
	sim.Run();

	return 0;
}
