#include "vehicle.h"
#include "SimulationEnvironment.h"
#include <iostream>

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
	this->EngineTorqueCurve = new DataMap2D();
	this->TorqueSpeedCurve = new DataMap2D();
}

Simulation::Vehicle::~Vehicle()
{
	if (this->VehiclespeedTorqueCurve != nullptr)
		delete this->VehiclespeedTorqueCurve;
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

	result->Mass = 2000;//nan("");
	result->FrontalArea = 2;
	result->DragCoefficient = 0.26;

	result->EngineUpperRevLimit = 6500 * RPM2HZ;
	result->EngineInertia = 0.35;
	result->AxleInertia = 0.07;
	vector<double> EngineSpeeds = vector<double>{ 1000 * RPM2HZ,2000 * RPM2HZ,3000 * RPM2HZ,4000 * RPM2HZ,5000 * RPM2HZ,6000 * RPM2HZ,7000 * RPM2HZ };
	vector<double> EngineTorque = vector<double>{ 100,200,300,400,500,600,700 };
	result->EngineTorqueCurve = new DataMap2D(EngineSpeeds, EngineTorque);

	result->PowerTrainType = PowerTrainTypes::Electric;

	vector<double> VehicleSpeeds = vector<double>{ 10 * KMH2MS,50 * KMH2MS,150 * KMH2MS };
	vector<double> VehicleTorque = vector<double>{ 100,300,500 };
	result->TorqueSpeedCurve = new DataMap2D(VehicleSpeeds, VehicleTorque);

	result->WheelWidth = 205;
	result->WheelRatioPercent = 75;
	result->WheelSize = 0.533 / INCH2M;

	result->RollingResistanceCoefficient = 0.02;
	result->VMaxElectric = 250 * KMH2MS;

	result->DeccelerationMax = 10.5;

	result->PowertrainEfficiency = 0.95;
	result->NumberOfGears = 1;
	result->FinalDriveRatio = 0.125;
	result->GearData = map<int, GearEntry>();
	GearEntry newEntry = GearEntry();
	newEntry.GearRatio = 1;
	newEntry.ShiftDownLimitMin = 250;
	newEntry.ShiftUpLimitMax = result->EngineUpperRevLimit;

	return result;
}

double Simulation::Vehicle::interpolateEngineTorqueFromVelocity(double V) {
	/* number of elements in the array */
	static const int count = sizeof(VehiclespeedTorqueCurve->yData) / sizeof(VehiclespeedTorqueCurve->yData[0]);

	int i;
	double dx, dy;

	if (V <= VehiclespeedTorqueCurve->xData[0]) {
		/* x is less than the minimum element
		 * handle error here if you want */

		cout << "Drehmoment" << VehiclespeedTorqueCurve->yData[0] << "\n";
		return VehiclespeedTorqueCurve->yData[0]; /* return minimum element */
	}

	if (V >= VehiclespeedTorqueCurve->xData[count - 1]) {
		cout << "Drehmoment" << VehiclespeedTorqueCurve->yData[count - 1] << "\n";
		return VehiclespeedTorqueCurve->yData[count - 1]; /* return maximum */
	}

	/* find i, such that EngineTorque_v_CUR[i] <= x < EngineTorque_v_CUR[i+1] */
	for (i = 0; i < count - 1; i++) {
		if (VehiclespeedTorqueCurve->xData[i + 1] > V) {
			break;
		}
	}

	/* interpolate */
	dx = VehiclespeedTorqueCurve->xData[i + 1] - VehiclespeedTorqueCurve->xData[i];
	dy = VehiclespeedTorqueCurve->yData[i + 1] - VehiclespeedTorqueCurve->yData[i];
	cout << "Drehmoment" << (VehiclespeedTorqueCurve->yData[i] + (V - VehiclespeedTorqueCurve->xData[i]) * dy / dx) << "\n";
	return (VehiclespeedTorqueCurve->yData[i] + (V - VehiclespeedTorqueCurve->xData[i]) * dy / dx);
}