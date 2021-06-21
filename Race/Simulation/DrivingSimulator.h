#pragma once
#ifndef  DRIVINGSIMULATOR
#define DRIVINGSIMULATOR

#include "Vehicle.h"
#include "SimulationEnvironment.h"
#include "..\Race.h"
#include <string>
#include "Position.h"
#include "AccelerationCalculator.h"
#include "SimulationNode.h"
#include "ImportSimulationConfig.h"
#include "IImportSimulationConfig.h"

namespace Simulation {
	class DrivingSimulator
	{
	public:
		DrivingSimulator(vector<node> input, IImportSimulationConfig& config);
		~DrivingSimulator();

		vector<node> RunSimulation();
	private:
		//Create a modified track which contains the simulationNodes
		void createModifiedTrack();
		//Map back function for the modified track
		void mapModifiedToRaw();

		//Calculate new speed limit from deccleration
		void calcNewSpeedLimit();

		//Calculate is-speed and the local time
		void calcIsSpeedandTime();

		double calcRaceTimeBetweenTwoPointsWithDifferentAccleration(double acceleration, double Speed_LocalPoint, double Speed_nextPoint, double Distance);
		double calcDistanceBetweenTwoPoints(node Trackpoint, node NextPoint);

		vector<node> rawtrack;
		vector<simulationNode> modifiedtrack; //Switch to normal nodes
		Simulation::Vehicle vehicle;
		Simulation::SimulationEnvironment environment;
		AccelerationCalculator* accelerationcalc;
		node TrackPoint;
		node NextPoint;
	};
}

#endif