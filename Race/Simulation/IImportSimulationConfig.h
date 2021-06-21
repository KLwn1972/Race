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
