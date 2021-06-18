#pragma once
#include "Vehicle.h"
#include "DrivingSimulator.h"

namespace Simulation
{
	class AccelerationCalculator
	{
	public:
		AccelerationCalculator(Vehicle& vehicle, SimulationEnvironment& environment);
		//Calculate acceleration
		double calcAcceleration(double velocity, simulationNode TrackPoint);
		double calcDecceleration(double velocity, simulationNode TrackPoint);
	private:
		double calcAirResistance(double velocity, double height);
		double calcRollingResistance(double velocity);
		double calcAccelerationForce(double velocity);
		double calcGradientForce(double gradient);

		Vehicle& vehicle;
		SimulationEnvironment& environment;
	};
}