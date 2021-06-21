#pragma once
#include "Position.h"
namespace Simulation
{
	class simulationNode
	{
	public:
		Position Coordinates = Position();
		double raceDistance = nan("");
		double gradient = nan("");
		double speedLimit = nan("");
		double newLimit = nan("");
		double raceTime = nan("");
		double speedIs = nan("");
		size_t baseID = INT16_MAX; //the position of the entry in the base array
		simulationNode();
	};
}