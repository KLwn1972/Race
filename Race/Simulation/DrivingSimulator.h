#pragma once
#include "Vehicle.h"
#include "SimulationEnvironment.h"
#include "..\Race.h"
#include <string>

namespace Simulation {
	class DrivingSimulator
	{
	public:
		DrivingSimulator(vector<node> input);
		void RunSimulation(std::string configfile);
	private:
		void loadConfiguration(std::string configfile);
		void initSimulation();
		int simulateStep();
		vector<node> track;
		Simulation::Vehicle vehicle;
		Simulation::SimulationEnvironment environment;
	};
}
