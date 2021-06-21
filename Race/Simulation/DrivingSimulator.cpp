#include "DrivingSimulator.h"
#include <string>
#include "..\Race.h"
#include "DataMap2D.h"
#include "ImportSimulationConfig.h"

using namespace std;

Simulation::DrivingSimulator::DrivingSimulator(vector<node> input, ImportSimulationConfig config)
{
	this->vehicle = Vehicle(); //TODO
	this->environment = SimulationEnvironment(); //TODO
	this->rawtrack = input;
	this->accelerationcalc = new AccelerationCalculator(this->vehicle, this->environment);
}

Simulation::DrivingSimulator::~DrivingSimulator()
{
	delete this->accelerationcalc;
}

vector<node> Simulation::DrivingSimulator::RunSimulation()
{
	createModifiedTrack();

	calcNewSpeedLimit();

	calcIsSpeedandTime();

	mapModifiedToRaw();

	return this->rawtrack;
}

void Simulation::DrivingSimulator::createModifiedTrack()
{
	const size_t NumberOfInterpolationPoints = 1; //TODO
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
		simulationNode& currentPos = this->modifiedtrack.at(i);
		simulationNode& previousPos = this->modifiedtrack.at(i - 1);
		if (previousPos.speedLimit <= currentPos.newLimit) {                                                                          //no need to brake wenn acceleration
			previousPos.newLimit = previousPos.speedLimit;
		}
		else {                                                                                                                                                   //calculate the brake velocity wenn decceleration
			double BrakeDecceleration = this->accelerationcalc->calcDecceleration(currentPos.newLimit, previousPos, currentPos);   //get max decceleration at current point
			double localDistance = currentPos.Coordinates.Distance(previousPos.Coordinates);                                              //get Distance between local point and previous point
			double BrakeSpeed = sqrt((currentPos.newLimit) * (currentPos.newLimit) - 2 * BrakeDecceleration * localDistance); //calculate the brake Velocity
			previousPos.newLimit = min(BrakeSpeed, previousPos.speedLimit);                                                         //get new limit
		}
	}
}

void Simulation::DrivingSimulator::calcIsSpeedandTime()
{
	this->modifiedtrack.at(0).speedIs = 0;                                                                                                                       //initialize the start point
	this->modifiedtrack.at(0).raceTime = 0;

	for (size_t i = 0; i < modifiedtrack.size() - 1; i++) {
		simulationNode& currentPos = this->modifiedtrack.at(i);
		simulationNode& nextPos = this->modifiedtrack.at(i + 1);
		double localDistance = currentPos.Coordinates.Distance(nextPos.Coordinates);        //TODO: Use References                                     //get distance between the local point and next point
		//case 1: acceleration
		if (nextPos.newLimit > currentPos.speedIs) {
			double MaxLocalAcceleration = 10;      // TODO: amax should be a function
			double speed_temp = sqrt((currentPos.speedIs) * (currentPos.speedIs) + 2 * MaxLocalAcceleration * localDistance);               //calculate the velocity at next point with maximal acceleration
			 // determine the Is-speed and raceTime according to different situation
			if (speed_temp > nextPos.newLimit) {                                         // Velocity with max acceleration larger than the SpeedLimit: IsSpeed korrigieren
				nextPos.speedIs = this->modifiedtrack.at(i + 1).newLimit;
				nextPos.raceTime = currentPos.raceTime + calcRaceTimeBetweenTwoPointsWithDifferentAccleration(MaxLocalAcceleration, currentPos.speedIs, nextPos.speedIs, localDistance);
			}
			else {                                                                                             //Velocity with max acceleration <= the SpeedLimit: IsSpeed = Velocity with max acceleration
				nextPos.speedIs = speed_temp;
				nextPos.raceTime = currentPos.raceTime + 2 * localDistance / (currentPos.speedIs + nextPos.speedIs);
			}
		}
		//case 2: hold speed
		else if (nextPos.newLimit == currentPos.speedIs) {
			nextPos.speedIs = nextPos.newLimit;
			nextPos.raceTime = localDistance / nextPos.speedIs;
		}
		//case 3: decceleration
		else {
			double MaxLocalDecceleration = this->accelerationcalc->calcDecceleration(currentPos.speedIs, currentPos, nextPos);
			nextPos.speedIs = nextPos.newLimit;                                       // IsSpeed always equal the new limt because the effect of maximal bremsen is already considered
			//determine the raceTime according to different situation
			if (currentPos.speedIs < currentPos.newLimit) {                                         // IsSpeed at current point smaller than speed limit, hold speed till the speed limit reached  then brake with max decceleration
				nextPos.raceTime = currentPos.raceTime + calcRaceTimeBetweenTwoPointsWithDifferentAccleration(MaxLocalDecceleration, currentPos.speedIs, nextPos.speedIs, localDistance);;
			}
			else {                                                                                                                 // IsSpeed at current point equals speed limit, brake with max decceleration
				nextPos.raceTime = currentPos.raceTime + 2 * localDistance / (currentPos.speedIs + nextPos.speedIs);
			}
		}
	}
}

double Simulation::DrivingSimulator::calcRaceTimeBetweenTwoPointsWithDifferentAccleration(double acceleration, double Speed_LocalPoint, double Speed_nextPoint, double Distance)
{
	double TimeAcceleration = (Speed_nextPoint - Speed_LocalPoint) / acceleration;
	double DistanceAcceleration = 0.5 * TimeAcceleration * (Speed_nextPoint + Speed_LocalPoint);
	double time = (Distance - DistanceAcceleration) / max(Speed_nextPoint, Speed_LocalPoint) + TimeAcceleration;
	return time;
}

Simulation::simulationNode::simulationNode()
{
	this->Coordinates = Position(0, 0, 0);
}