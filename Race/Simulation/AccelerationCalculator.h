#pragma once
#include "Vehicle.h"
#include "SimulationNode.h"
#include "SimulationEnvironment.h"

namespace Simulation
{
	class AccelerationCalculator
	{
	public:
		AccelerationCalculator(Vehicle& vehicle, SimulationEnvironment& environment);
		//Calculate acceleration
		double calcAcceleration(double velocity, simulationNode TrackPoint, simulationNode NextPoint);
		double calcDecceleration(double velocity, simulationNode TrackPoint, simulationNode NextPoint);
	private:
		double calcAirResistance(double velocity);
		double calcRollingResistance(double velocity);
		double calcAccelerationResistance(double velocity);
		double calcGradientResistance(double gradient);
		double calcAdhesionLimit(double gradient, double velocity);

		Vehicle& vehicle;
		SimulationEnvironment& environment;
		simulationNode TrackPoint;
		simulationNode NextPoint;
	};
}
