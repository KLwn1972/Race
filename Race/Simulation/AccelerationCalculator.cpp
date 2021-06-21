#include "AccelerationCalculator.h"

Simulation::AccelerationCalculator::AccelerationCalculator(Vehicle& vehicle, SimulationEnvironment& environment) :vehicle(vehicle), environment(environment)
{
}

double Simulation::AccelerationCalculator::calcAcceleration(double velocity, simulationNode TrackPoint, simulationNode NextPoint)
{
	this->TrackPoint = TrackPoint;
	this->NextPoint = NextPoint;

	return 10;
}

double Simulation::AccelerationCalculator::calcDecceleration(double velocity, simulationNode TrackPoint, simulationNode NextPoint)
{
	this->TrackPoint = TrackPoint;
	this->NextPoint = NextPoint;
	double effectDeccelerationForce = -(calcAirResistance(velocity) + calcRollingResistance(TrackPoint.gradient) + calcGradientResistance(TrackPoint.gradient)) - this->vehicle.DeccelerationMax * this->vehicle.Mass;
	return effectDeccelerationForce / (this->vehicle.Mass + (this->vehicle.EngineInertia + this->vehicle.AxleInertia + this->vehicle.WheelInertia) / this->vehicle.calcDynamicWheelRadius());
}

double Simulation::AccelerationCalculator::calcAirResistance(double velocity)
{
	double deltaY = (this->NextPoint.Coordinates.PositionY - this->TrackPoint.Coordinates.PositionY);
	double deltaX = (this->NextPoint.Coordinates.PositionX - this->TrackPoint.Coordinates.PositionX);
	double vehicledirection = asin(deltaY / deltaX);

	double RelevantWindSpeed = this->environment.calcRelevantWindSpeed(vehicledirection);
	double VelocityAgainstWind = velocity - RelevantWindSpeed;
	return 0.5 * this->vehicle.DragCoefficient * this->vehicle.FrontalArea * this->environment.calcAirDensity(this->TrackPoint.Coordinates.PositionZ) * velocity * velocity; //1/2 cw roh A v^2
}

double Simulation::AccelerationCalculator::calcRollingResistance(double gradient)
{
	return vehicle.Mass * GRAVITATIONALCONSTANT * environment.calcRoadResistanceCoefficient() * cos(gradient); //TODO: check gradient format
}

double Simulation::AccelerationCalculator::calcGradientResistance(double gradient)
{
	return vehicle.Mass * GRAVITATIONALCONSTANT * sin(gradient);
}

double Simulation::AccelerationCalculator::calcEffectiveWheelForceLong(double gradient, double velocity)
{
	double maximumTorque = 0;
	if (this->vehicle.PowerTrainType == PowerTrainTypes::Electric)
	{
		maximumTorque = this->vehicle.TorqueSpeedCurve->getY(velocity);
	}
	else if (this->vehicle.PowerTrainType == PowerTrainTypes::ICE)
	{
		maximumTorque = 0.0; //TODO: Manual Gearbox Torque Calculation
	}
	double longitudalPowertrainForce = maximumTorque * this->vehicle.FinalDriveRatio * this->vehicle.PowertrainEfficiency / this->vehicle.calcDynamicWheelRadius();
	double Adhesionlimit = this->calcAdhesionLimit(gradient, velocity);
	double ResultingForce = min(longitudalPowertrainForce, Adhesionlimit);
	return 0.0;
}

double Simulation::AccelerationCalculator::calcAdhesionLimit(double gradient, double velocity)
{
	return vehicle.Mass * GRAVITATIONALCONSTANT * environment.calcFrictionCoefficient(gradient, velocity) * cos(gradient);
}