#pragma once
#include "..\cJSON.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "SimulationEnvironment.h"
#include "Vehicle.h"

namespace Simulation {
	class ImportSimulationConfig
	{
	public:
		Vehicle getVehicle();
		SimulationEnvironment getEnvironment();
		ImportSimulationConfig(std::string SimulationConfigFile);
		void saveSimulationConfig(std::string ConfigFileAsString);
		~ImportSimulationConfig();
	};
}