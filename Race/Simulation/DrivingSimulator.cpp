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
	this->modifiedtrack[this->modifiedtrack.size() - 1].newLimit = this->modifiedtrack[this->modifiedtrack.size() - 1].speedLimit;                      //set the new limit at last point 
	
	for (size_t i = this->modifiedtrack.size() - 1; i > 0; i--) {                                                                                          //calculate the new limit according to the max Brake from last point;
		
		if (this->modifiedtrack.at(i - 1).speedLimit <= this->modifiedtrack.at(i).newLimit) {                                                                          //no need to brake wenn acceleration 
			this->modifiedtrack.at(i - 1).newLimit = this->modifiedtrack.at(i-1).speedLimit;
		}
		
		else {                                                                                                                                                   //calculate the brake velocity wenn decceleration 
			double BrakeDecceleration = -10;                   //amax should be a funtion;
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

void Simulation::DrivingSimulator::calcIsSpeedandTime()
{
	this->rawtrack.at(0).speedIs = 0;                                                                                                                       //initialize the start point
	this->rawtrack.at(0).raceTime = 0;
	this->modifiedtrack.at(0).raceDistance = 0;

	for (int i = 0; i < modifiedtrack.size() - 1; i++) {
		double localDistance = this->modifiedtrack.at(i).Coordinates.Distance(this->modifiedtrack.at(i + 1).Coordinates);                                   //get distance between the local point and next point
		if (this->modifiedtrack.at(i + 1).newLimit > this->rawtrack.at(i).speedIs) {
			double MaxLocalAcceleration = 10;      // amax should be a function
			double time_temp = SolveQuadraticEquation(0.5 * MaxLocalAcceleration, this->modifiedtrack.at(i).newLimit, -1 * localDistance);
			double speed_temp = this->rawtrack.at(i).speedIs + MaxLocalAcceleration * time_temp;
			if (speed_temp > this->modifiedtrack.at(i + 1).newLimit) {
				this->rawtrack.at(i + 1).speedIs = this->modifiedtrack.at(i + 1).newLimit;
				double t1 = (this->rawtrack.at(i + 1).speedIs - this->rawtrack.at(i).speedIs) / MaxLocalAcceleration;
				double s1 = 0.5 * t1 * (this->rawtrack.at(i + 1).speedIs + this->rawtrack.at(i).speedIs);
				double s2 = localDistance - s1;
				double t2 = s2 / this->rawtrack.at(i + 1).speedIs;
				this->rawtrack.at(i + 1).raceTime = this->rawtrack.at(i).raceTime + t1 + t2;
			}
			else {
				this->rawtrack.at(i + 1).speedIs = speed_temp;
				this->rawtrack.at(i + 1).raceTime = this->rawtrack.at(i).raceTime + 2 * localDistance / (this->rawtrack.at(i).speedIs + this->rawtrack.at(i + 1).speedIs);	
			}
		}
		else if (this->modifiedtrack.at(i + 1).newLimit == this->rawtrack.at(i).speedIs) {
			this->rawtrack.at(i + 1).speedIs = this->modifiedtrack.at(i + 1).newLimit;
			this->rawtrack.at(i + 1).raceTime = localDistance / this->rawtrack.at(i + 1).speedIs;
		}
		else {
			double MaxLocalDecceleration = -10; // this should be a function getLocalMaxBrakeDecceleration;
			double time_temp = SolveQuadraticEquation(0.5 * MaxLocalDecceleration, this->modifiedtrack.at(i).newLimit, -1 * localDistance);
			double speed_temp = this->rawtrack.at(i).speedIs + MaxLocalDecceleration * time_temp;
			if (speed_temp < this->modifiedtrack.at(i + 1).newLimit) {
				this->rawtrack.at(i + 1).speedIs = this->modifiedtrack.at(i + 1).newLimit;
				double t1 = (this->rawtrack.at(i+1).speedIs - this->rawtrack.at(i).speedIs) / MaxLocalDecceleration;
				double s1 = 0.5 * t1 * (this->rawtrack.at(i + 1).speedIs + this->rawtrack.at(i).speedIs);
				double s2 = localDistance - s1;
				double t2 = s2 / this->rawtrack.at(i).speedIs;
				this->rawtrack.at(i + 1).raceTime = this->rawtrack.at(i).raceTime + t1 + t2;
			}
			else if (speed_temp = this->modifiedtrack.at(i + 1).newLimit) {
				this->rawtrack.at(i + 1).speedIs = speed_temp;
				this->rawtrack.at(i + 1).raceTime = this->rawtrack.at(i).raceTime + 2 * localDistance / (this->rawtrack.at(i).speedIs + this->rawtrack.at(i + 1).speedIs);
			}
		}
	}
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