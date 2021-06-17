#include "DrivingSimulator.h"
#include <string>
#include "..\Race.h"

using namespace std;

Simulation::DrivingSimulator::DrivingSimulator(vector<node> input)
{
	this->vehicle = Vehicle();
	this->environment = SimulationEnvironment();
	this->rawtrack = input;
}

void Simulation::DrivingSimulator::RunSimulation(std::string configfile)
{
	loadConfiguration(configfile);

	//node startpoint = this->track.

		//this->track.
}

void Simulation::DrivingSimulator::loadConfiguration(std::string configfile)
{
}

void Simulation::DrivingSimulator::initSimulation()
{
}

void Simulation::DrivingSimulator::createModifiedTrack()
{
	const size_t NumberOfInterpolationPoints = 1;
	size_t TotalNumberOfPoints = (this->rawtrack.size() - 1) * NumberOfInterpolationPoints;
	vector<simulationNode> newTrack = vector<simulationNode>(TotalNumberOfPoints, simulationNode());

	node oldNode = rawtrack[0];
	simulationNode oldStepSimNode = simulationNode();
	for (size_t i = 0; i < this->rawtrack.size(); i++)
	{
		node currentNode = rawtrack[i];
		simulationNode stepSimNode = simulationNode();
		stepSimNode.Coordinates = Position::PositionFromGeoCoordinates(oldNode.longitude, oldNode.latitude, oldNode.elevation);
		stepSimNode.baseID = i;
		stepSimNode.gradient = oldNode.gradient;
		stepSimNode.speedLimit = oldNode.speedLimit;
		newTrack.push_back(stepSimNode);
		//insert values between stepsimnode and oldstepsimnode through interpolation
		if (i > 0)
		{
			double Distance = stepSimNode.Coordinates.Distance(oldStepSimNode.Coordinates);
		}
		oldStepSimNode = stepSimNode;
	}
}

void Simulation::DrivingSimulator::calcNewSpeedLimit()
{
	//last entry: this->modifiedtrack[this->modifiedtrack.size()-1];
	//first entry: this->modifiedtrack[0];
	simulationNode simNode = this->modifiedtrack[0];
}

int Simulation::DrivingSimulator::simulateStep()
{
	//Calculate the decceleration (vBremse)

	//Calculate new velocity

	//Calculate the forces

	//Decide which target should be used:
	//vx < vmax ==> accelerate
	//brakepoint reached ==> brake
	//vx == vmax && brakepoint not reached ==> hold speed

	//Calculate new acceleration

	return 0;
}

Simulation::simulationNode::simulationNode()
{
	this->Coordinates = Position(0, 0, 0);
}

double Simulation::interpolateValues(double startX, double startValue, double endX, double endValue, double whereToInterPolate)
{
	return endValue + ((endValue - startValue) / (endX - startX)) * (whereToInterPolate - startX);
}