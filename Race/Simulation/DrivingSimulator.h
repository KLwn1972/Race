#pragma once
#include "Vehicle.h"
#include "SimulationEnvironment.h"
#include "..\Race.h"
#include <string>
#include "Position.h"

namespace Simulation {
	class simulationNode
	{
		Position Coordinates;
		double raceDistance;
		double gradient;
		double speedLimit;
		double newLimit;
		double raceTime;
		long baseID; //the position of the entry in the base array
	};

	class DrivingSimulator
	{
	public:
		DrivingSimulator(vector<node> input);
		void RunSimulation(std::string configfile);
	private:
		void loadConfiguration(std::string configfile);
		void initSimulation();

		//Create a modified track which contains the simulationNodes
		void createModifiedTrack(vector<node> track);

		//Calculate new speed limit from deccleration
		void calcNewSpeedLimit();

		int simulateStep();
		vector<node> rawtrack;
		vector<simulationNode> modifiedtrack;
		Simulation::Vehicle vehicle;
		Simulation::SimulationEnvironment environment;
	};
}
