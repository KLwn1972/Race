#include "SimulationEnvironment.h"
#include <math.h>

using namespace Simulation;

SimulationEnvironment* ExampleSimEnvironment()
{
	SimulationEnvironment* result = new SimulationEnvironment();
	result->setAirpressure(1013 * MBAR2PASCAL);
	result->setAirtemperatureCelsius(22);
	result->setWinddirection(0);
	result->setWindspeed(10);

	return result;
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

std::tuple<double, double> Simulation::GeoCoordinates2KarthesianLongLat(double GeoLong, double GeoLat)
{
	double CartX, CartY;
	const double EarthRadius_m = 6378.137 * 1000;
	CartX = EarthRadius_m * (GeoLong * pi()) / (180.0);
	CartY = EarthRadius_m * atanh(sin((GeoLat * pi()) / 180.0));

	return { CartX,CartY };
}

std::tuple<double, double> Simulation::GeoCoordinates2KarthesianLatLong(double GeoLat, double GeoLong)
{
	return GeoCoordinates2KarthesianLongLat(GeoLong, GeoLat);
}