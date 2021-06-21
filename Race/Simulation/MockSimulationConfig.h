#pragma once
#include "IImportSimulationConfig.h"

namespace Simulation
{
	class MockSimulationConfig : public IImportSimulationConfig
	{
	public:
		Vehicle* getVehicle();
		SimulationEnvironment* getEnvironment();
		void saveSimulationConfig(std::string ConfigFileAsString);
	};
}