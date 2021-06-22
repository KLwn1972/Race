#include "MockSimulationConfig.h"

Simulation::Vehicle* Simulation::MockSimulationConfig::getVehicle()
{
	return ExampleElectricVehicle();
}

Simulation::SimulationEnvironment* Simulation::MockSimulationConfig::getEnvironment()
{
	return ExampleSimulationEnvironment();
}

void Simulation::MockSimulationConfig::saveSimulationConfig(std::string ConfigFileAsString)
{
}