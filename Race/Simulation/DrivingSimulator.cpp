#include "DrivingSimulator.h"
#include <string>
#include "..\Race.h"

using namespace std;

Simulation::DrivingSimulator::DrivingSimulator(vector<node> input)
{
	this->vehicle = Vehicle();
	this->environment = SimulationEnvironment();
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

int Simulation::DrivingSimulator::simulateStep()
{
	//Calculate the preview (vBremse)

	//Calculate new velocity

	//Calculate the forces

	//Decide which target should be used:
	//vx < vmax ==> accelerate
	//brakepoint reached ==> brake
	//vx == vmax && brakepoint not reached ==> hold speed

	//Calculate new acceleration

	return 0;
}