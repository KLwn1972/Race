/*
Team Fahrphysik: Alexander Eirich, Yuying Zhu, Max Paschke

AccelerationCalculator is a function which calculates the acceleration for a vehicle.
*/

#pragma once
#include "Vehicle.h"
#include "SimulationEnvironment.h"
#include "../Race.h"

namespace Simulation
{
	class AccelerationCalculator
	{
	public:
		AccelerationCalculator(Vehicle* vehicle, SimulationEnvironment* environment);
		//Calculate acceleration
		double calcAcceleration(double velocity, node TrackPoint, node NextPoint);
		double calcDecceleration(double velocity, node TrackPoint, node NextPoint);
	private:
		double calcAirResistance(double velocity);
		double calcRollingResistance(double velocity);
		double calcGradientResistance(double gradient);
		double calcEffectiveWheelForceLong(double gradient, double velocity);
		double calcAdhesionLimit(double gradient, double velocity);

		Vehicle* vehicle;
		SimulationEnvironment* environment;
		node TrackPoint;
		node NextPoint;
	};
}
