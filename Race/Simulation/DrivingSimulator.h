/*
Team Fahrphysik: Alexander Eirich, Yuying Zhu, Max Paschke

The drivingsimulator calculates a vehicle going along a track. The track is provided by the input nodes.
*/

#pragma once
#ifndef  DRIVINGSIMULATOR
#define DRIVINGSIMULATOR

#include "Vehicle.h"
#include "SimulationEnvironment.h"
#include "../Race.h"
#include <string>
#include "AccelerationCalculator.h"
#include "ImportSimulationConfig.h"
#include "IImportSimulationConfig.h"
#include "SimulationNode.h"

namespace Simulation {
	class DrivingSimulator
	{
	public:
		DrivingSimulator(vector<node> input, IImportSimulationConfig& config);
		~DrivingSimulator();

		vector<node> RunSimulation();
		vector<SimulationNode> ReturnModifiedTrack();

		void setInterpolationLevel(unsigned int level);

	private:
		//Create a modified track which contains the simulationNodes
		void createModifiedTrack();
		//Map back function for the modified track
		void mapModifiedToRaw();

		//Calculate new speed limit from deccleration values
		void calcNewSpeedLimit();

		//Calculate is-speed and the local time
		void calcIsSpeedandTime();

		double calcRaceTimeBetweenTwoPointsWithDifferentAccleration(double acceleration, double Speed_LocalPoint, double Speed_nextPoint, double Distance);

		vector<node> rawtrack;
		vector<SimulationNode> modifiedtrack; //Switch to normal nodes
		Simulation::Vehicle* vehicle;
		Simulation::SimulationEnvironment* environment;
		AccelerationCalculator* accelerationcalc;
		node TrackPoint;
		node NextPoint;
		unsigned int InterpolationLevel = 0;
	};
}

#endif