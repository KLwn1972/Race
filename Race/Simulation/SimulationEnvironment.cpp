#include "SimulationEnvironment.h"
#include <math.h>

using namespace Simulation;

double Simulation::SimulationEnvironment::calcAirPressure(double height)
{
	return this->Airpressure; //TODO: calculate Height influence
}

double Simulation::SimulationEnvironment::calcAirTemperatureInKelvin()
{
	return this->Airtemperature + ABSOLUTEZEROTEMP;
}

double Simulation::SimulationEnvironment::calcRelevantWindSpeed(double VehicleDirection)
{
	return this->Windspeed; //TODO: Windir
}

void Simulation::SimulationEnvironment::setRollingResistanceCoefficient(double Coefficient)
{
	this->RollingResistanceCoefficient = Coefficient;
}

double Simulation::SimulationEnvironment::getRollingResistanceCoefficient()
{
	return this->RollingResistanceCoefficient;
}

double Simulation::SimulationEnvironment::calcFrictionCoefficient(double Velocity)
{
	return this->FrictionTable.getY(Velocity);
}

double Simulation::SimulationEnvironment::calcAirDensity(double height)
{
	return this->calcAirPressure(height) / (GASCONSTANT * (this->calcAirTemperatureInKelvin()));
}

void Simulation::SimulationEnvironment::setAirtemperatureCelsius(double Airtemperature)
{
	this->Airtemperature = Airtemperature;
}

void Simulation::SimulationEnvironment::setAirpressure(double Airpressure)
{
	this->Airpressure = Airpressure;
}

void Simulation::SimulationEnvironment::setWindspeed(double Windspeed)
{
	this->Windspeed = Windspeed;
}

void Simulation::SimulationEnvironment::setWinddirection(double Winddirection)
{
	this->Winddirection = Winddirection;
}

void Simulation::SimulationEnvironment::setFrictionTable(std::vector<double> velocityValues, std::vector<double> CoefficientValues) {
	this->FrictionTable = DataMap2D(velocityValues, CoefficientValues);
}

SimulationEnvironment* Simulation::ExampleSimulationEnvironment()
{
	SimulationEnvironment* result = new SimulationEnvironment();
	result->setAirpressure(1013 * MBAR2PASCAL);
	result->setAirtemperatureCelsius(22);
	result->setWinddirection(0);
	result->setWindspeed(10);
	result->setRollingResistanceCoefficient(0.014);
	std::vector<double> frictiondataSpeed = { 10 * KMH2MS,50 * KMH2MS,100 * KMH2MS };
	std::vector<double> frictiondataCoeff = { 1.15, 1.0, 0.8 };
	result->setFrictionTable(frictiondataSpeed, frictiondataCoeff);

	return result;
}