#include "AccelerationCalculator.h"

Simulation::AccelerationCalculator::AccelerationCalculator(Vehicle& vehicle, SimulationEnvironment& environment) :vehicle(vehicle), environment(environment)
{
}

double Simulation::AccelerationCalculator::calcAcceleration(double velocity, simulationNode TrackPoint, simulationNode NextPoint)
{
	this->TrackPoint = TrackPoint; //TODO: check CalcAcc
	this->NextPoint = NextPoint;
	double TotalInertia = this->vehicle.EngineInertia + this->vehicle.AxleInertia + this->vehicle.WheelInertia;
	return calcEffectiveWheelForceLong(TrackPoint.gradient, velocity) / (this->vehicle.Mass + TotalInertia / this->vehicle.calcDynamicWheelRadius());
}

double Simulation::AccelerationCalculator::calcDecceleration(double velocity, simulationNode TrackPoint, simulationNode NextPoint)
{
	this->TrackPoint = TrackPoint; //TODO: Check Calculation
	this->NextPoint = NextPoint;
	double SumResistance = calcAirResistance(velocity) + calcRollingResistance(TrackPoint.gradient) + calcGradientResistance(TrackPoint.gradient);
	double effectDeccelerationForce = -SumResistance - this->vehicle.DeccelerationMax * this->vehicle.Mass;
	double decceleration = effectDeccelerationForce / (this->vehicle.Mass + (this->vehicle.EngineInertia + this->vehicle.AxleInertia + this->vehicle.WheelInertia) / this->vehicle.calcDynamicWheelRadius());
	return decceleration;
}

double Simulation::AccelerationCalculator::calcAirResistance(double velocity)
{
	double deltaY = (this->NextPoint.Coordinates.PositionY - this->TrackPoint.Coordinates.PositionY); //TODO: Check with Excel
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
	//Calculation of resulting forces
	double longitudalPowertrainForceMax = maximumTorque * this->vehicle.FinalDriveRatio * this->vehicle.PowertrainEfficiency / this->vehicle.calcDynamicWheelRadius();
	double ResistanceForce = (calcAirResistance(velocity) + calcGradientResistance(TrackPoint.gradient) + calcRollingResistance(TrackPoint.gradient));
	double ResultingForceLong = longitudalPowertrainForceMax - ResistanceForce;
	double Adhesionlimit = this->calcAdhesionLimit(gradient, velocity);
	return min(ResultingForceLong, Adhesionlimit);
}

double Simulation::AccelerationCalculator::calcAdhesionLimit(double gradient, double velocity)
{
	return vehicle.Mass * GRAVITATIONALCONSTANT * environment.calcFrictionCoefficient(gradient, velocity) * cos(gradient);
}