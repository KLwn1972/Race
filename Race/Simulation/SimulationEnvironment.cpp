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

std::tuple<double, double> Simulation::GeoCoordinatesLongLat2Karthesian(double GeoLong, double GeoLat)
{
	double CartX, CartY;
	//Web Mercator Projektion
	const double EarthRadius_m = 6378.137 * 1000;
	CartX = EarthRadius_m * (GeoLong * PI) / (180.0);
	CartY = EarthRadius_m * atanh(sin((GeoLat * PI) / 180.0));

	return { CartX,CartY };
}

std::tuple<double, double> Simulation::GeoCoordinatesLatLong2Karthesian(double GeoLat, double GeoLong)
{
	return GeoCoordinatesLongLat2Karthesian(GeoLong, GeoLat);
}