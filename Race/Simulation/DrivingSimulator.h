#pragma once
#include "Vehicle.h"
#include "SimulationEnvironment.h"
#include "..\Race.h"
#include <string>
#include "Position.h"

namespace Simulation {
	class simulationNode
	{
	public:
		Position Coordinates = Position();
		double raceDistance = nan("");
		double gradient = nan("");
		double speedLimit = nan("");
		double newLimit = nan("");
		double raceTime = nan("");
		size_t baseID = INT16_MAX; //the position of the entry in the base array
		simulationNode();
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
		void createModifiedTrack();

		//Calculate new speed limit from deccleration
		void calcNewSpeedLimit();

		int simulateStep();
		vector<node> rawtrack;
		vector<simulationNode> modifiedtrack;
		Simulation::Vehicle vehicle;
		Simulation::SimulationEnvironment environment;
	};

	static double interpolateValues(double startX, double startValue, double endX, double endValue, double whereToInterPolate);
}
