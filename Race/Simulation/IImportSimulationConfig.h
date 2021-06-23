/*
Team Fahrphysik: Alexander Eirich, Yuying Zhu, Max Paschke

Interface for the config importer class. This is used to provide testing possibilites via MockSimulationConfig.
The driving simulator uses this interface to get the vehicle and environment data.
*/

#pragma once
#ifndef  SIMINTERFACE
#define SIMINTERFACE

#include "SimulationEnvironment.h"
#include "Vehicle.h"

namespace Simulation {
	class IImportSimulationConfig
	{
	public:
		virtual Vehicle* getVehicle() = 0;
		virtual SimulationEnvironment* getEnvironment() = 0;
		virtual void saveSimulationConfig(std::string ConfigFileAsString) = 0;
	private:
	};
}

#endif
