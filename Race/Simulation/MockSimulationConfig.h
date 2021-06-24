/*
Team Fahrphysik: Alexander Eirich, Yuying Zhu, Max Paschke

MockSimulationConfig provides fake data (in order to not have to rely on the import) to test the drivingsimulator.
*/

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