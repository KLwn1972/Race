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
		DrivingSimulator(vector<node> input, IImportSimulationConfig* config);
		~DrivingSimulator();
		/// <summary>
		/// Run the simulation an return a nodevector
		/// </summary>
		/// <returns></returns>
		vector<node> RunSimulation();

		/// <summary>
		/// Return the modified track for debug purposes
		/// </summary>
		/// <returns></returns>
		vector<SimulationNode> ReturnModifiedTrack();

		/// <summary>
		///Interpolationlevel can be set to increase the number of nodes by interpolating linearily
		///This is not needed if distancetonext <= 1m
		/// </summary>
		/// <param name="level">Number of interpolation points</param>
		void setInterpolationLevel(unsigned int level);

	private:

		/// <summary>
		///Create a modified track which contains the simulationNodes
		/// </summary>
		void createModifiedTrack();

		/// <summary>
		/// Map back method for the modified track
		/// </summary>
		void mapModifiedToRaw();

		/// <summary>
		/// Calculate new speed limit from deccleration values
		/// </summary>
		void calcNewSpeedLimit();

		/// <summary>
		/// Calculate is-speed and the local time
		/// </summary>
		void calcIsSpeedandTime();

		/// <summary>
		/// Calculate the Racetime between two points if the acceleration not constant
		/// </summary>
		/// <param name="acceleration"></param>
		/// <param name="Speed_LocalPoint"></param>
		/// <param name="Speed_nextPoint"></param>
		/// <param name="Distance"></param>
		/// <returns></returns>
		double calcRaceTimeBetweenTwoPointsWithDifferentAccleration(double acceleration, double Speed_LocalPoint, double Speed_nextPoint, double Distance);

		/// <summary>
		/// The raw track (nodes)
		/// </summary>
		vector<node> rawtrack;
		/// <summary>
		/// modified track consisting of the simulationnodes to have more control over the nodes, e.g. for adding data members
		/// </summary>
		vector<SimulationNode> modifiedtrack;
		Simulation::Vehicle* vehicle;
		Simulation::SimulationEnvironment* environment;
		AccelerationCalculator* accelerationcalc;
		node TrackPoint;
		node NextPoint;
		/// <summary>
		/// The number of points that should be added via interpolation, default: no extra interpolation
		/// </summary>
		unsigned int InterpolationLevel = 0;
	};
}

#endif