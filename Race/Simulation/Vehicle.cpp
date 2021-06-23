#include "vehicle.h"
#include "SimulationEnvironment.h"
#include <iostream>

using namespace Simulation;

double Simulation::Vehicle::calcStaticWheelDiameter()
{
	return (2 * (this->WheelWidth) * (this->WheelRatioPercent) / 100.0 + (this->WheelSize));
}

int Simulation::Vehicle::getSelectedGear()
{
	return this->selectedgear;
}

void Simulation::Vehicle::setSelectedGear(int GearToSet)
{
	if ((GearToSet < NumberOfGears) && (GearToSet >= 0))
	{
		this->selectedgear = GearToSet;
	}
}

Simulation::Vehicle::Vehicle()
{
	this->EnginespeedTorqueCurve = new DataMap2D();
	this->VehiclespeedTorqueCurve = new DataMap2D();
}

Simulation::Vehicle::~Vehicle()
{
	if (this->VehiclespeedTorqueCurve != nullptr)
		delete this->VehiclespeedTorqueCurve;
	if (this->EnginespeedTorqueCurve != nullptr)
		delete this->EnginespeedTorqueCurve;
}

double Simulation::Vehicle::calcDynamicWheelRadius()
{
	return ((calcStaticWheelDiameter()) * 0.97 / (2));
}

Vehicle* Simulation::ExampleElectricVehicle()
{
	Vehicle* result = new Vehicle();

	result->Mass = 2000;//nan("");
	result->FrontalArea = 2;
	result->DragCoefficient = 0.26;

	result->EngineUpperRevLimit = 6500 * RPM2HZ;
	result->EngineInertia = 0.35;
	result->AxleInertia = 0.07;
	vector<double> EngineSpeeds = vector<double>{ 1000 * RPM2HZ,2000 * RPM2HZ,3000 * RPM2HZ,4000 * RPM2HZ,5000 * RPM2HZ,6000 * RPM2HZ,7000 * RPM2HZ };
	vector<double> EngineTorque = vector<double>{ 100,200,300,400,500,600,700 };
	result->EnginespeedTorqueCurve = new DataMap2D(EngineSpeeds, EngineTorque);

	result->PowertrainType = PowerTrainTypes::Electric;

	vector<double> VehicleSpeeds = vector<double>{ 10 * KMH2MS,50 * KMH2MS,150 * KMH2MS };
	vector<double> VehicleTorque = vector<double>{ 100,300,500 };
	result->VehiclespeedTorqueCurve = new DataMap2D(VehicleSpeeds, VehicleTorque);

	result->WheelWidth = 0.205;
	result->WheelRatioPercent = 75;
	result->WheelSize = 16;
	result->WheelInertia = 0.35;

	result->VMaxLimited = 250 * KMH2MS;

	result->DeccelerationMax = 10.5;

	result->PowertrainEfficiency = 0.95;
	result->NumberOfGears = 1;
	result->FinalDriveRatio = 1;
	result->GearData = map<int, GearEntry>();
	GearEntry newEntry = GearEntry();
	newEntry.GearRatio = 1;
	newEntry.ShiftDownLimitMin = 250;
	newEntry.ShiftUpLimitMax = result->EngineUpperRevLimit;

	return result;
}

double Simulation::Vehicle::interpolateEngineTorqueFromVelocity(double V) {
	vector<double> xData = this->VehiclespeedTorqueCurve->getXData();
	vector<double> yData = this->VehiclespeedTorqueCurve->getYData();

	static const int count = yData.size();

	int i;
	double dx, dy;

	if (V <= xData[0]) {
		return yData[0];
	}

	if (V >= xData[count - 1]) {
		return yData[count - 1];
	}

	for (i = 0; i < count - 1; i++) {
		if (xData[i + 1] > V) {
			break;
		}
	}

	dx = xData[i + 1] - xData[i];
	dy = yData[i + 1] - yData[i];
	return (yData[i] + (V - xData[i]) * dy / dx);
}