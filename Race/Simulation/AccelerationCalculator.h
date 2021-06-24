/*
Team Fahrphysik: Alexander Eirich, Yuying Zhu, Max Paschke

AccelerationCalculator is a function which calculates the acceleration for a vehicle.
*/

#pragma once
#include "Vehicle.h"
#include "SimulationEnvironment.h"
#include "../Race.h"
#include "SimulationNode.h"

namespace Simulation
{
	class AccelerationCalculator
	{
	public:
		AccelerationCalculator(Vehicle* vehicle, SimulationEnvironment* environment);
		/// <summary>
		/// Calculate the acceleration
		/// </summary>
		/// <param name="velocity"></param>
		/// <param name="TrackPoint">Current Point in the track</param>
		/// <param name="NextPoint">Next Point</param>
		/// <returns></returns>
		double calcAcceleration(double velocity, SimulationNode& TrackPoint, SimulationNode& NextPoint);

		/// <summary>
		/// Calculate the decceleration
		/// </summary>
		/// <param name="velocity"></param>
		/// <param name="TrackPoint">Current Point in the track</param>
		/// <param name="NextPoint">Next entry in the track</param>
		/// <returns></returns>
		double calcDecceleration(double velocity, SimulationNode& TrackPoint, SimulationNode& NextPoint);
	private:
		double calcAirResistance(double velocity, double height);
		double calcRollingResistance(double velocity);
		double calcGradientResistance(double gradient);
		double calcEffectiveWheelForceLong(double gradient, double velocity, double height);
		double calcAdhesionLimit(double gradient, double velocity);
		double calcVehicledirection(node TrackPoint, node NextPoint);

		Vehicle* vehicle;
		SimulationEnvironment* environment;
		/// <summary>
		/// Vehicle direction in [rad]
		/// </summary>
		double vehicledirection = nan("");
	};
}
