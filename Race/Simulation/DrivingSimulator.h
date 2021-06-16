#pragma once
#include "Vehicle.h"
#include "SimulationEnvironment.h"
#include "..\Race.h"
#include <string>

namespace Simulation {
	class DrivingSimulator
	{
	public:
		DrivingSimulator(graph input, std::string startnode);
		void RunSimulation(std::string configfile);
	private:
		void loadConfiguration(std::string configfile);
		void initSimulation();
		int simulateStep();
		graph track;
		Simulation::Vehicle vehicle;
		Simulation::SimulationEnvironment environment;
	};
}
