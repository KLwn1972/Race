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

Simulation::simulationNode::simulationNode()
{
	this->Coordinates = Position(0, 0, 0);
}

}