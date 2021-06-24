#include "AccelerationCalculator.h"
#include "MiscFunctions.h"

Simulation::AccelerationCalculator::AccelerationCalculator(Vehicle* vehicle, SimulationEnvironment* environment) :vehicle(vehicle), environment(environment)
{
}

double Simulation::AccelerationCalculator::calcAcceleration(double velocity, SimulationNode& TrackPoint, SimulationNode& NextPoint)
{
	this->vehicledirection = calcVehicledirection(TrackPoint, NextPoint);
	double TotalInertia = this->vehicle->EngineInertia + this->vehicle->AxleInertia + this->vehicle->WheelInertia;
	//FOr debug purposes
	double AirResistance = calcAirResistance(velocity, TrackPoint.elevation);
	double RollingResistance = calcRollingResistance(TrackPoint.gradient);
	double GradientResistance = calcGradientResistance(TrackPoint.gradient);
	TrackPoint.AirResistance = AirResistance;
	TrackPoint.RollingResistance = RollingResistance;
	TrackPoint.GradientResistance = GradientResistance;

	return calcEffectiveWheelForceLong(TrackPoint.gradient, velocity, TrackPoint.elevation) / (this->vehicle->Mass + TotalInertia / this->vehicle->calcDynamicWheelRadius());
}

double Simulation::AccelerationCalculator::calcDecceleration(double velocity, SimulationNode& TrackPoint, SimulationNode& NextPoint)
{
	this->vehicledirection = calcVehicledirection(TrackPoint, NextPoint);
	double AirResistance = calcAirResistance(velocity, TrackPoint.elevation);
	double RollingResistance = calcRollingResistance(TrackPoint.gradient);
	double GradientResistance = calcGradientResistance(TrackPoint.gradient);
	TrackPoint.AirResistance = AirResistance;
	TrackPoint.RollingResistance = RollingResistance;
	TrackPoint.GradientResistance = GradientResistance;
	double SumResistance = AirResistance + RollingResistance + GradientResistance;
	double effectDeccelerationForce = -SumResistance - this->vehicle->DeccelerationMax * this->vehicle->Mass;
	double decceleration = effectDeccelerationForce / (this->vehicle->Mass + (this->vehicle->EngineInertia + this->vehicle->AxleInertia + this->vehicle->WheelInertia) / this->vehicle->calcDynamicWheelRadius());
	return decceleration;
}

double Simulation::AccelerationCalculator::calcAirResistance(double velocity, double height)
{
	double RelevantWindSpeed = this->environment->calcRelevantWindSpeed(this->vehicledirection);
	double VelocityAgainstWind = velocity - RelevantWindSpeed;
	return 0.5 * this->vehicle->DragCoefficient * this->vehicle->FrontalArea * this->environment->calcAirDensity(height) * velocity * velocity; //1/2 cw roh A v^2
}

double Simulation::AccelerationCalculator::calcRollingResistance(double gradient)
{
	double gradientAngle = angleRadFromGradient(gradient);
	return this->vehicle->Mass * GRAVITATIONALCONSTANT * this->environment->getRollingResistanceCoefficient() * cos(gradientAngle);
}

double Simulation::AccelerationCalculator::calcGradientResistance(double gradient)
{
	double gradientAngle = angleRadFromGradient(gradient);
	return this->vehicle->Mass * GRAVITATIONALCONSTANT * sin(gradientAngle);
}

double Simulation::AccelerationCalculator::calcEffectiveWheelForceLong(double gradient, double velocity, double height)
{
	double maximumTorque = 0;
	if (this->vehicle->PowertrainType == PowerTrainTypes::Electric)
	{
		maximumTorque = this->vehicle->VehicleSpeedTorqueCurve->getY(velocity);
	}
	else if (this->vehicle->PowertrainType == PowerTrainTypes::ICE)
	{
		maximumTorque = 0.0; //TODO: Manual Gearbox Torque Calculation
	}
	//Calculation of resulting forces
	double longitudalPowertrainForceMax = maximumTorque * this->vehicle->FinalDriveRatio * this->vehicle->PowertrainEfficiency / this->vehicle->calcDynamicWheelRadius();
	double ResistanceForce = (calcAirResistance(velocity, height) + calcGradientResistance(gradient) + calcRollingResistance(gradient));
	double ResultingForceLong = longitudalPowertrainForceMax - ResistanceForce;
	double Adhesionlimit = this->calcAdhesionLimit(gradient, velocity);
	return min(ResultingForceLong, Adhesionlimit);
}

double Simulation::AccelerationCalculator::calcAdhesionLimit(double gradient, double velocity)
{
	double gradientAngle = angleRadFromGradient(gradient);
	return this->vehicle->Mass * GRAVITATIONALCONSTANT * this->environment->calcFrictionCoefficient(velocity) * cos(gradientAngle);
}

double Simulation::AccelerationCalculator::calcVehicledirection(node TrackPoint, node NextPoint)
{
	double deltaLat = (NextPoint.latitude - TrackPoint.latitude);
	double deltaLong = (NextPoint.longitude - TrackPoint.longitude);
	return asin(deltaLat / deltaLong);
}