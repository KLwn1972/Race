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
	this->modifiedtrack[this->modifiedtrack.size() - 1].newLimit = this->modifiedtrack[this->modifiedtrack.size() - 1].speedLimit;                      //set the new limit at last point 
	//int NumberofPoints = this->modifiedtrack.size();
	for (size_t i = this->modifiedtrack.size() - 1; i > 0; i--) {                                                                                          //calculate the new limit according to the max Brake from last point;
		if (this->modifiedtrack.at(i - 1).speedLimit <= this->modifiedtrack.at(i).newLimit) {                                                                          //no need to brake wenn acceleration 
			this->modifiedtrack.at(i - 1).newLimit = this->modifiedtrack.at(i-1).speedLimit;
		}
		else {                                                                                                                                                   //calculate the brake velocity wenn decceleration 
			double BrakeDecceleration = -10;                   //amax as funtion;
			double localDistance = this->modifiedtrack.at(i).Coordinates.Distance(this->modifiedtrack.at(i-1).Coordinates);                                              //get Distance between local point and previous point
			double time = SolveQuadraticEquation(0.5 * BrakeDecceleration, -1 * this->modifiedtrack.at(i).newLimit, localDistance);
			double BrakeSpeed = this->modifiedtrack.at(i).newLimit - BrakeDecceleration * time;                                                                     //calculate the brake Velocity
			this->modifiedtrack.at(i-1).newLimit = min(BrakeSpeed, this->modifiedtrack.at(i-1).speedLimit);                                                         //get new limit
		}
	}
}

double Simulation::DrivingSimulator::SolveQuadraticEquation(double a, double b, double c)
{
	double x, x1, x2, discriminant, realPart, imaginaryPart;
	discriminant = b * b - 4 * a * c;

	if (discriminant > 0) {
		x1 = (-b + sqrt(discriminant)) / (2 * a);
		x2 = (-b - sqrt(discriminant)) / (2 * a);
		x = min(abs(x1), abs(x2));
		cout << "x1 = " << x1 << endl;
		cout << "x2 = " << x2 << endl;
		cout << "x = " << x << endl;

	}

	else if (discriminant == 0) {
		x = x1 = (-b + sqrt(discriminant)) / (2 * a);
		cout << "x1 = x2 =" << x1 << endl;
	}

	else {
		realPart = -b / (2 * a);
		imaginaryPart = sqrt(-discriminant) / (2 * a);
		x = nan("");
		cout << "no Solution!" << endl;
	}

	return x;
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