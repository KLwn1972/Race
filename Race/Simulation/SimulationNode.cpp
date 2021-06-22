#include "SimulationNode.h"

Simulation::SimulationNode::SimulationNode()
{
}

Simulation::SimulationNode::SimulationNode(node input)
{
	this->distanceToNext = input.distanceToNext;
	this->elevation = input.elevation;
	this->gradient = input.gradient;
	this->horizontalCurveRadius = input.horizontalCurveRadius;
	this->id = input.id;
	this->latitude = input.latitude;
	this->longitude = input.longitude;
	this->raceTime = input.raceTime;
	this->speedIs = input.speedIs;
	this->speedLimit = input.speedLimit;
	this->verticalCurveRadius = input.verticalCurveRadius;
}