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

vector<node> Simulation::DrivingSimulator::RunSimulation(std::string configfile)
{
	loadConfiguration(configfile);

	createModifiedTrack();

	calcNewSpeedLimit();

	calcIsSpeedandTime();

	mapModifiedToRaw();

	return this->rawtrack;
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

	simulationNode oldStepSimNode = simulationNode();
	for (size_t i = 0; i < this->rawtrack.size(); i++)
	{
		node currentNode = rawtrack[i];
		simulationNode stepSimNode = simulationNode();
		stepSimNode.Coordinates = Position::PositionFromGeoCoordinates(currentNode.longitude, currentNode.latitude, currentNode.elevation);
		stepSimNode.baseID = i;
		stepSimNode.gradient = currentNode.gradient;
		stepSimNode.speedLimit = currentNode.speedLimit;
		newTrack.push_back(stepSimNode);
		//insert values between stepsimnode and oldstepsimnode through interpolation
		if (i > 0)
		{
			double DistanceOldNew = oldStepSimNode.Coordinates.Distance(stepSimNode.Coordinates);

			for (int j = 1; j <= NumberOfInterpolationPoints; j++) //TODO: Check boundaries
			{
				double DistanceToCheck = j * DistanceOldNew / (NumberOfInterpolationPoints + 1);
				simulationNode interpolatedNode = simulationNode();
				double interpolatedX = interpolateValues(0, oldStepSimNode.Coordinates.PositionX, DistanceOldNew, stepSimNode.Coordinates.PositionX, DistanceToCheck);
				double interpolatedY = interpolateValues(0, oldStepSimNode.Coordinates.PositionY, DistanceOldNew, stepSimNode.Coordinates.PositionY, DistanceToCheck);
				double interpolatedZ = interpolateValues(0, oldStepSimNode.Coordinates.PositionZ, DistanceOldNew, stepSimNode.Coordinates.PositionZ, DistanceToCheck);
				interpolatedNode.Coordinates = Position(interpolatedX, interpolatedY, interpolatedZ);
				interpolatedNode.speedLimit = interpolateValues(0, oldStepSimNode.speedLimit, DistanceOldNew, stepSimNode.speedLimit, DistanceToCheck);
				interpolatedNode.gradient = interpolateValues(0, oldStepSimNode.gradient, DistanceOldNew, stepSimNode.gradient, DistanceToCheck);
				newTrack.push_back(interpolatedNode);
			}
		}
		oldStepSimNode = stepSimNode;
	}
	this->modifiedtrack = newTrack;
}

void Simulation::DrivingSimulator::mapModifiedToRaw()
{
	vector<node> result;
	for (auto const& simNode : this->modifiedtrack)
	{
		if (simNode.baseID != INT16_MAX)
		{
			node newnode = this->rawtrack.at(simNode.baseID);
			newnode.raceTime = simNode.raceTime;
			newnode.speedIs = simNode.speedIs;
			this->rawtrack.at(simNode.baseID) = newnode;
		}
	}
}

void Simulation::DrivingSimulator::calcNewSpeedLimit()
{
	this->modifiedtrack[this->modifiedtrack.size() - 1].newLimit = this->modifiedtrack[this->modifiedtrack.size() - 1].speedLimit;                      //set the new limit at last point

	for (size_t i = this->modifiedtrack.size() - 1; i > 0; i--) {                                                                                          //calculate the new limit according to the max Brake from last point;
		if (this->modifiedtrack.at(i - 1).speedLimit <= this->modifiedtrack.at(i).newLimit) {                                                                          //no need to brake wenn acceleration
			this->modifiedtrack.at(i - 1).newLimit = this->modifiedtrack.at(i - 1).speedLimit;
		}
		else {                                                                                                                                                   //calculate the brake velocity wenn decceleration
			double BrakeDecceleration = -10;                   //TODO: amax should be a funtion;
			double localDistance = this->modifiedtrack.at(i).Coordinates.Distance(this->modifiedtrack.at(i - 1).Coordinates);                                              //get Distance between local point and previous point
			double BrakeSpeed = sqrt((this->modifiedtrack.at(i).newLimit) * (this->modifiedtrack.at(i).newLimit) - 2 * BrakeDecceleration * localDistance); //calculate the brake Velocity
			this->modifiedtrack.at(i - 1).newLimit = min(BrakeSpeed, this->modifiedtrack.at(i - 1).speedLimit);                                                         //get new limit
		}
	}
}


void Simulation::DrivingSimulator::calcIsSpeedandTime()
{
	this->modifiedtrack.at(0).speedIs = 0;                                                                                                                       //initialize the start point
	this->modifiedtrack.at(0).raceTime = 0;
	this->modifiedtrack.at(0).raceDistance = 0;

	for (size_t i = 0; i < modifiedtrack.size() - 1; i++) {
		double localDistance = this->modifiedtrack.at(i).Coordinates.Distance(this->modifiedtrack.at(i + 1).Coordinates);                                             //get distance between the local point and next point
		
		if (this->modifiedtrack.at(i + 1).newLimit > this->modifiedtrack.at(i).speedIs) {                                                                              //case 1: acceleration
			double MaxLocalAcceleration = 10;      // TODO: amax should be a function
			double speed_temp = sqrt((this->modifiedtrack.at(i).speedIs) * (this->modifiedtrack.at(i).speedIs) + 2 * MaxLocalAcceleration * localDistance);               //calculate the velocity at next point with maximal acceleration
			                                                                                                                                                              // determine the Is-speed and raceTime according to different situation
			if (speed_temp > this->modifiedtrack.at(i + 1).newLimit) {                                                                                                          //
				this->modifiedtrack.at(i + 1).speedIs = this->modifiedtrack.at(i + 1).newLimit;                                                                                 //Is-speed = new speed limit wenn the velocity > new speed limit 
				double t1 = (this->modifiedtrack.at(i + 1).speedIs - this->modifiedtrack.at(i).speedIs) / MaxLocalAcceleration;
				double s1 = 0.5 * t1 * (this->modifiedtrack.at(i + 1).speedIs + this->modifiedtrack.at(i).speedIs);
				double s2 = localDistance - s1;
				double t2 = s2 / this->modifiedtrack.at(i + 1).speedIs;
				this->modifiedtrack.at(i + 1).raceTime = this->modifiedtrack.at(i).raceTime + t1 + t2;
			}
			else {
				this->modifiedtrack.at(i + 1).speedIs = speed_temp;
				this->modifiedtrack.at(i + 1).raceTime = this->modifiedtrack.at(i).raceTime + 2 * localDistance / (this->modifiedtrack.at(i).speedIs + this->modifiedtrack.at(i + 1).speedIs);
			}
		}
		else if (this->modifiedtrack.at(i + 1).newLimit == this->modifiedtrack.at(i).speedIs) {                                                                        //case 2: hold speed  
			this->modifiedtrack.at(i + 1).speedIs = this->modifiedtrack.at(i + 1).newLimit;
			this->modifiedtrack.at(i + 1).raceTime = localDistance / this->modifiedtrack.at(i + 1).speedIs;
		}
		else {                                                                                                                                                         //case 3: decceleration
			double MaxLocalDecceleration = -10; // this should be a function getLocalMaxBrakeDecceleration;                                                                  
			this->modifiedtrack.at(i + 1).speedIs = this->modifiedtrack.at(i + 1).newLimit;                                                                              // IsSpeed always equal the new limt because the effect of maximal bremsen is already considered 
			if (this->modifiedtrack.at(i).speedIs < this->modifiedtrack.at(i).newLimit) {                                                                                      //determine the raceTime according to different situation
				double t1 = (this->modifiedtrack.at(i + 1).speedIs - this->modifiedtrack.at(i).speedIs) / MaxLocalDecceleration;
				double s1 = 0.5 * t1 * (this->modifiedtrack.at(i + 1).speedIs + this->modifiedtrack.at(i).speedIs);
				double s2 = localDistance - s1;
				double t2 = s2 / this->modifiedtrack.at(i).speedIs;
				this->modifiedtrack.at(i + 1).raceTime = this->modifiedtrack.at(i).raceTime + t1 + t2;
			}
			else  {
				this->modifiedtrack.at(i + 1).raceTime = this->modifiedtrack.at(i).raceTime + 2 * localDistance / (this->modifiedtrack.at(i).speedIs + this->modifiedtrack.at(i + 1).speedIs);
			}
		}
	}
}

double Simulation::DrivingSimulator::calcAcceleration(double velocity, simulationNode TrackPoint)
{
	return 0.0;
}

double Simulation::DrivingSimulator::calcDecceleration(double velocity, simulationNode TrackPoint)
{
	return 0.0;
}

Simulation::simulationNode::simulationNode()
{
	this->Coordinates = Position(0, 0, 0);
}

double Simulation::interpolateValues(double startX, double startValue, double endX, double endValue, double whereToInterPolate)
{
	return endValue + ((endValue - startValue) / (endX - startX)) * (whereToInterPolate - startX);
}