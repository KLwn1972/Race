#pragma once
#include "Vehicle.h"
#include "SimulationEnvironment.h"
#include "..\Race.h"
#include <string>
#include "Position.h"
#include "AccelerationCalculator.h"

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
		double speedIs = nan("");
		size_t baseID = INT16_MAX; //the position of the entry in the base array
		simulationNode();
	};

	class DrivingSimulator
	{
	public:
		DrivingSimulator(vector<node> input);
		vector<node> RunSimulation(std::string configfile);
	private:
		void loadConfiguration(std::string configfile);
		void initSimulation();

		//Create a modified track which contains the simulationNodes
		void createModifiedTrack();
		//Map back function for the modified track
		void mapModifiedToRaw();

		//Calculate new speed limit from deccleration
		void calcNewSpeedLimit();

		//Calculate is-speed and the local time
		void calcIsSpeedandTime();

		double calcRaceTimeBetweenTwoPointsWithDifferentAccleration(double acceleration, double Speed_LocalPoint, double Speed_nextPoint, double Distance);

		vector<node> rawtrack;
		vector<simulationNode> modifiedtrack;
		Simulation::Vehicle vehicle;
		Simulation::SimulationEnvironment environment;
		//AccelerationCalculator accelerationcalc;
	};

	static double interpolateValues(double startX, double startValue, double endX, double endValue, double whereToInterPolate);
}
