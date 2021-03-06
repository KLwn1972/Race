#include "DrivingSimulator.h"
#include <string>
#include "..\Race.h"
#include "DataMap1D.h"
#include "ImportSimulationConfig.h"

using namespace std;

Simulation::DrivingSimulator::DrivingSimulator(vector<node> input, IImportSimulationConfig* config)
{
	this->vehicle = config->getVehicle();
	this->environment = config->getEnvironment();
	this->rawtrack = input;
	this->accelerationcalc = new AccelerationCalculator(this->vehicle, this->environment);
}

Simulation::DrivingSimulator::~DrivingSimulator()
{
	delete this->accelerationcalc;
}

vector<node> Simulation::DrivingSimulator::RunSimulation()
{
	cout << "Running Simulation:" << endl;
	cout << "Create the modified track with interpolation points" << endl;
	createModifiedTrack();
	cout << "Modified track has " << this->modifiedtrack.size() << " Points instead of " << this->rawtrack.size() << " (normal track)" << endl;
	cout << "Calculate the speed limit which is determined by braking" << endl;
	calcNewSpeedLimit();
	cout << "Calculate actual speed limit and time" << endl;
	calcIsSpeedandTime();
	cout << "Map the modified track back to the original one" << endl;
	mapModifiedToRaw();
	cout << "Calculation finished, returning data" << endl;
	return this->rawtrack;
}

vector<Simulation::SimulationNode> Simulation::DrivingSimulator::ReturnModifiedTrack()
{
	return this->modifiedtrack;
}

void Simulation::DrivingSimulator::setInterpolationLevel(unsigned int level)
{
	cout << "Setting interpolation level to " << level;
	this->InterpolationLevel = level;
}

void Simulation::DrivingSimulator::createModifiedTrack()
{
	vector<SimulationNode> newTrack = vector<SimulationNode>();

	SimulationNode oldStepSimNode = SimulationNode();
	for (size_t i = 0; i < this->rawtrack.size(); i++)
	{
		SimulationNode currentNode = SimulationNode(rawtrack[i]);
		currentNode.PositionInStartVector = i;
		//insert values between stepsimnode and oldstepsimnode through interpolation
		if (i > 0 && this->InterpolationLevel > 0)
		{
			double DistanceOldNew = oldStepSimNode.distanceToNext;
			double DistanceBetweenPoints = DistanceOldNew / (this->InterpolationLevel + 1.0);
			newTrack.at(newTrack.size()-1).distanceToNext = DistanceBetweenPoints;
			for (unsigned int j = 1; j <= this->InterpolationLevel; j++)
			{
				double DistanceToCheck = j * DistanceBetweenPoints;
				SimulationNode interpolatedNode = SimulationNode();
				interpolatedNode.latitude = interpolateValues(0, oldStepSimNode.latitude, DistanceOldNew, currentNode.latitude, DistanceToCheck);
				interpolatedNode.longitude = interpolateValues(0, oldStepSimNode.longitude, DistanceOldNew, currentNode.longitude, DistanceToCheck);
				interpolatedNode.elevation = interpolateValues(0, oldStepSimNode.elevation, DistanceOldNew, currentNode.elevation, DistanceToCheck);
				interpolatedNode.speedLimit = interpolateValues(0, oldStepSimNode.speedLimit, DistanceOldNew, currentNode.speedLimit, DistanceToCheck);
				interpolatedNode.gradient = interpolateValues(0, oldStepSimNode.gradient, DistanceOldNew, currentNode.gradient, DistanceToCheck);
				interpolatedNode.horizontalCurveRadius = interpolateValues(0, oldStepSimNode.horizontalCurveRadius, DistanceOldNew, currentNode.horizontalCurveRadius, DistanceToCheck);
				interpolatedNode.verticalCurveRadius = interpolateValues(0, oldStepSimNode.verticalCurveRadius, DistanceOldNew, currentNode.verticalCurveRadius, DistanceToCheck);
				interpolatedNode.id = INTERPOLATEDIDENT;
				interpolatedNode.distanceToNext = DistanceBetweenPoints;
				newTrack.push_back(interpolatedNode);
			}
		}
		newTrack.push_back(currentNode);
		oldStepSimNode = currentNode;
	}
	this->modifiedtrack = newTrack;
}

void Simulation::DrivingSimulator::mapModifiedToRaw()
{
	vector<node> result;
	int nodeIt = 0;
	for (auto const& simNode : this->modifiedtrack)
	{
		if (simNode.id != INTERPOLATEDIDENT)
		{
			node resultNode = this->rawtrack.at(simNode.PositionInStartVector);
			resultNode.raceTime = simNode.raceTime;
			resultNode.speedIs = simNode.speedIs;
			result.push_back(resultNode);
		}
	}
	this->rawtrack = result;
}

void Simulation::DrivingSimulator::calcNewSpeedLimit()
{
	for (size_t i = this->modifiedtrack.size() - 1; i > 0; i--) {   //calculate the new limit according to the max Brake from last point;
		SimulationNode& currentPos = this->modifiedtrack.at(i);
		SimulationNode& previousPos = this->modifiedtrack.at(i - 1);
		if (previousPos.speedLimit > currentPos.speedLimit) {   //calculate the brake velocity wenn decceleration
			double BrakeDecceleration = this->accelerationcalc->calcDecceleration(currentPos.speedLimit, previousPos, currentPos);   //get max decceleration at current point
			double localDistance = previousPos.distanceToNext;                        //get Distance between local point and previous point
			double BrakeSpeedSquared = (currentPos.speedLimit) * (currentPos.speedLimit) - 2 * BrakeDecceleration * localDistance;
			double BrakeSpeed = sqrt(BrakeSpeedSquared); //calculate the brake Velocity
			previousPos.speedLimit = min(BrakeSpeed, previousPos.speedLimit);                                                         //get new limit
		}
	}
}

void Simulation::DrivingSimulator::calcIsSpeedandTime()
{
	this->modifiedtrack.at(0).speedIs = 0;                                                                                                                       //initialize the start point
	this->modifiedtrack.at(0).raceTime = 0;

	for (size_t i = 0; i < modifiedtrack.size() - 1; i++) {
		SimulationNode& currentPos = this->modifiedtrack.at(i);
		SimulationNode& nextPos = this->modifiedtrack.at(i + 1);
		double localDistance = currentPos.distanceToNext;
		//acceleration or trying to hold speed
		if (nextPos.speedLimit >= currentPos.speedIs) {
			double MaxLocalAcceleration = this->accelerationcalc->calcAcceleration(currentPos.speedIs, currentPos, nextPos);                //get amax
			currentPos.MaxAcceleration = MaxLocalAcceleration;
			double speed_tempSquared = (currentPos.speedIs) * (currentPos.speedIs) + 2 * MaxLocalAcceleration * localDistance;
			if (speed_tempSquared <= 0)
				break; //Abbruch wenn das Fahrzeug anh?lt
			double speed_temp = sqrt(speed_tempSquared);               //calculate the velocity at next point with maximal acceleration

			 // determine the Is-speed and raceTime according to different situation
			nextPos.speedIs = min(speed_temp, nextPos.speedLimit);
			nextPos.raceTime = currentPos.raceTime + 2 * localDistance / (currentPos.speedIs + nextPos.speedIs);
		}
		//decceleration
		else {
			double MaxLocalDecceleration = this->accelerationcalc->calcDecceleration(currentPos.speedIs, currentPos, nextPos);
			currentPos.MaxAcceleration = MaxLocalDecceleration;
			nextPos.speedIs = nextPos.speedLimit;                                       // IsSpeed always equal the new limt because the effect of maximal bremsen is already considered
			//determine the raceTime according to different situation
			if (currentPos.speedIs < currentPos.speedLimit) {                                         // IsSpeed at current point smaller than speed limit, hold speed till the speed limit reached  then brake with max decceleration
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