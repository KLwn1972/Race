#include "AccelerationCalculator.h"

Simulation::AccelerationCalculator::AccelerationCalculator(Vehicle& vehicle, SimulationEnvironment& environment) :vehicle(vehicle), environment(environment)
{
}

double Simulation::AccelerationCalculator::calcAcceleration(double velocity, simulationNode TrackPoint)
{
	return 0.0;
}

double Simulation::AccelerationCalculator::calcDecceleration(double velocity, simulationNode TrackPoint)
{
	return 0.0;
}

double Simulation::AccelerationCalculator::calcAirResistance(double velocity, double height)
{
	return 0.5 * this->vehicle.DragCoefficient * this->vehicle.FrontalArea * this->environment.calcAirDensity(height) * velocity * velocity; //1/2 cw roh A v^2
}

double Simulation::AccelerationCalculator::calcRollingResistance(double velocity)
{
	return 0.0;
}

double Simulation::AccelerationCalculator::calcAccelerationForce(double velocity)
{
	return 0.0;
}

double Simulation::AccelerationCalculator::calcGradientForce(double gradient)
{
	return 0.0;
}