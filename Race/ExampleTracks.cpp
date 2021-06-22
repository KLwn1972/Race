#include "ExampleTracks.h"
#include "Race.h"
#include "Simulation/DataMap2D.h"
#include "Simulation/SimulationEnvironment.h"

std::vector<node> ExampleTrack()
{
	//Create a sample nürburgring with (bad) linear interpolation
	vector<double> longitude = { 6.945215, 6.971891, 6.994314, 6.995172, 6.992038, 6.985426,6.989161, 6.980189, 6.96349, 6.9607, 6.950525, 6.9495, 6.938555, 6.933661, 6.93761, 6.931342, 6.928638, 6.91989, 6.927059, 6.925857, 6.933327, 6.944404, 6.945215 };
	vector<double> latitude = { 50.33409, 50.34831, 50.360358, 50.376866, 50.373581, 50.371774, 50.374539, 50.372267, 50.375361, 50.380836, 50.378399, 50.376561, 50.37771, 50.374562, 50.371769, 50.367115, 50.362049, 50.358109, 50.350138, 50.34581, 50.341153, 50.337241, 50.33409 };
	vector<double> elevation = { 500, 333.3333333, 222.2222222, 148.1481481, 98.7654321, 65.8436214, 98.7654321, 148.1481481, 222.2222222, 333.3333333, 500, 750, 500, 333.3333333, 222.2222222, 148.1481481, 98.7654321, 65.8436214, 98.7654321, 148.1481481, 222.2222222, 333.3333333, 500 };

	//TODO: Create track from data
	return vector<node>();
}

std::vector<node> ExampleStraightTrack(double length)
{
	double startlong = 6.945215;
	double startlat = 50.33409;
	double startelevation = 500;

	double endlong = 6.971891;
	double endlat = 50.34831;
	double endelevation = 500;

	double distance = 3245.0;

	auto result = vector<node>();
	double numberOfSteps = 1000;
	double stepWidth = distance / numberOfSteps;
	for (int i = 0; i <= numberOfSteps; i++)
	{
		auto newnode = node();
		newnode.distanceToNext = distance / numberOfSteps;
		newnode.elevation = Simulation::interpolateValues(0, startelevation, distance, endelevation, i * stepWidth);
		newnode.latitude = Simulation::interpolateValues(0, startlat, distance, endlat, i * stepWidth);
		newnode.longitude = Simulation::interpolateValues(0, startlong, distance, endlong, i * stepWidth);
		newnode.gradient = ((endelevation - startelevation) / distance) * 100;
		newnode.speedLimit = 200 * Simulation::KMH2MS;
		newnode.horizontalCurveRadius = INFINITY;
		newnode.verticalCurveRadius = INFINITY;
		newnode.id = std::to_string(i);
		result.push_back(newnode);
	}
	return result;
}

std::vector<node> ExampleHillTrack() //TODO: can be split into functions for generating hills
{
	auto result = std::vector<node>();

	double startlong = 6.945215;
	double startlat = 50.33409;
	double startelevation = 500;

	double endlong = 6.971891;
	double endlat = 50.34831;
	double endelevation = 4000;

	double distance = 3000;
	double numberOfSteps = 100;
	double stepWidth = distance / numberOfSteps;
	for (int i = 0; i <= numberOfSteps; i++)
	{
		auto newnode = node();
		newnode.distanceToNext = distance / numberOfSteps;
		newnode.elevation = Simulation::interpolateValues(0, startelevation, distance, endelevation, i * stepWidth);
		newnode.latitude = Simulation::interpolateValues(0, startlat, distance, endlat, i * stepWidth);
		newnode.longitude = Simulation::interpolateValues(0, startlong, distance, endlong, i * stepWidth);
		newnode.gradient = ((endelevation - startelevation) / distance) * 100;
		newnode.speedLimit = 200 * Simulation::KMH2MS;
		newnode.horizontalCurveRadius = INFINITY;
		newnode.verticalCurveRadius = INFINITY;
		newnode.id = std::to_string(i);
		result.push_back(newnode);
	}

	startelevation = 600;
	endelevation = 500;
	distance = 500;
	numberOfSteps = 100;
	stepWidth = distance / numberOfSteps;
	for (int i = 0; i <= numberOfSteps; i++)
	{
		auto newnode = node();
		newnode.distanceToNext = distance / numberOfSteps;
		newnode.elevation = Simulation::interpolateValues(0, startelevation, distance, endelevation, i * stepWidth);
		newnode.latitude = Simulation::interpolateValues(0, startlat, distance, endlat, i * stepWidth);
		newnode.longitude = Simulation::interpolateValues(0, startlong, distance, endlong, i * stepWidth);
		newnode.gradient = ((endelevation - startelevation) / distance) * 100;
		newnode.speedLimit = 200 * Simulation::KMH2MS;
		newnode.horizontalCurveRadius = INFINITY;
		newnode.verticalCurveRadius = INFINITY;
		newnode.id = std::to_string(i);
		result.push_back(newnode);
	}
	return result;
}