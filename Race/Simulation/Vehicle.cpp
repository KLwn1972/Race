#include "vehicle.h"
#include "SimulationEnvironment.h"

using namespace Simulation;

Vehicle* ExampleElectricVehicle()
{
	Vehicle* result = new Vehicle();

	result->Mass = 1200;
	result->FrontalArea = 2;
	result->DragCoefficient = 0.3;

	result->EngineUpperRevLimit = 6500 * RPM2HZ;
	result->EngineInertia = 0.35;
	result->AxleInertia = 0.07;
	result->EngineTorqueCurve = vector<EngineEntry>();
	result->EngineTorqueCurve.push_back(EngineEntry(500 * RPM2HZ, 50));
	result->EngineTorqueCurve.push_back(EngineEntry(5000 * RPM2HZ, 250));
	result->EngineTorqueCurve.push_back(EngineEntry(result->EngineUpperRevLimit, 200));
	result->PowerTrainType = PowerTrainTypes::Electric;

	result->TorqueSpeedCurve = vector<TorqueSpeedEntry>();
	result->TorqueSpeedCurve.push_back(TorqueSpeedEntry(0, 100));
	result->TorqueSpeedCurve.push_back(TorqueSpeedEntry(100 * KMH2MS, 1000));

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

double Simulation::Vehicle::calcStaticWheelDiameter()
{
	return (2 * (this->WheelWidth) * (this->WheelRatioPercent) / 100.0 + (this->WheelSize * 25.4));
}

double Simulation::Vehicle::calcDynamicWheelRadius(double WheelWidth, double WheelRatioPercent, double WheelSize)
{
	return ((2 * (this->WheelWidth) * (this->WheelRatioPercent) / 100.0 + (this->WheelSize * 0.0254))*Simulation::PI*0.97/(2*Simulation::PI));
}