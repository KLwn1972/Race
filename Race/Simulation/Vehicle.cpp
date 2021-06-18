#include "vehicle.h"
#include "SimulationEnvironment.h"

using namespace Simulation;

double Simulation::Vehicle::calcStaticWheelDiameter()
{
	return (2 * (this->WheelWidth) * (this->WheelRatioPercent) / 100.0 + (this->WheelSize * INCH2M));
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
}

Simulation::Vehicle::~Vehicle()
{
	if (this->TorqueSpeedCurve != nullptr)
		delete this->TorqueSpeedCurve;
	if (this->EngineTorqueCurve != nullptr)
		delete this->EngineTorqueCurve;
}

double Simulation::Vehicle::calcDynamicWheelRadius()
{
	return ((calcStaticWheelDiameter()) * 0.97 / (2));
}

Vehicle* Simulation::ExampleElectricVehicle()
{
	Vehicle* result = new Vehicle();

	result->Mass = 1200;
	result->FrontalArea = 2;
	result->DragCoefficient = 0.3;

	result->EngineUpperRevLimit = 6500 * RPM2HZ;
	result->EngineInertia = 0.35;
	result->AxleInertia = 0.07;
	vector<double> EngineSpeeds = vector<double>{ 1000 * RPM2HZ,2000 * RPM2HZ,3000 * RPM2HZ,4000 * RPM2HZ,5000 * RPM2HZ,6000 * RPM2HZ,7000 * RPM2HZ };
	vector<double> EngineTorque = vector<double>{ 100,200,300,400,500,600,700 };
	result->EngineTorqueCurve = new DataMap2D(EngineSpeeds, EngineTorque);

	result->PowerTrainType = PowerTrainTypes::Electric;

	vector<double> VehicleSpeeds = vector<double>{ 10 * KMH2MS,50 * KMH2MS,150 * KMH2MS };
	vector<double> VehicleTorque = vector<double>{ 100,200,300,400,500,600,700 };
	result->TorqueSpeedCurve = new DataMap2D(VehicleSpeeds, VehicleTorque);

	result->WheelWidth = 205;
	result->WheelRatioPercent = 75;
	result->WheelSize = 16;

	result->RollingResistanceCoefficient = 0.02;
	result->VMaxElectric = 250 * KMH2MS;

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