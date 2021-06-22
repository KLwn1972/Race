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

void Simulation::SimulationEnvironment::setRoadResistanceCoefficient(double RoadResistanceCoefficient)
{
	this->RoadResistanceCoefficient = RoadResistanceCoefficient;
}

double Simulation::SimulationEnvironment::getRoadResistanceCoefficient()
{
	return this->RoadResistanceCoefficient;
}

double Simulation::SimulationEnvironment::calcFrictionCoefficient(double Velocity, double Gradient)
{
	return 0.0; //TODO: FrictionCoefficient
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

SimulationEnvironment* Simulation::ExampleSimulationEnvironment()
{
	SimulationEnvironment* result = new SimulationEnvironment();
	result->setAirpressure(1013 * MBAR2PASCAL);
	result->setAirtemperatureCelsius(22);
	result->setWinddirection(0);
	result->setWindspeed(10);

	return result;
}