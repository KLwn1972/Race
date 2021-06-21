#pragma once
#include "..\cJSON.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

namespace Simulation {
	class ImportSimulationConfig
	{
		ImportSimulationConfig(std::string SimulationConfigFile);
		void saveSimulationConfig(std::string ConfigFileAsString);
		~ImportSimulationConfig();
	};
}

