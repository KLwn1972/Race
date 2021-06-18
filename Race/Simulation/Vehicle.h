#pragma once

#include <vector>
#include <map>
using namespace std;

namespace Simulation
{
	class ShiftEntry
	{
		double EngineSpeed = nan("");
		double PedalPosition = nan("");
	};

	class GearEntry
	{
	public:
		double ShiftDownLimitMin = nan(""); //rpm at which driver should shift down
		double ShiftUpLimitMax = nan(""); //rpm at which driver should shift up
		vector<ShiftEntry> LowerShiftLimit;
		vector<ShiftEntry> UpperShiftLimit;
		double GearRatio = nan("");
	};

	class TorqueSpeedEntry
	{
	public:
		double VehicleSpeed = nan("");
		double VehicleTorque = nan("");
		TorqueSpeedEntry(double VehicleSpeed, double VehicleTorque)
		{
			this->VehicleSpeed = VehicleSpeed;
			this->VehicleTorque = VehicleTorque;
		}
	};

	class EngineEntry
	{
	public:
		double EngineSpeed = nan("");
		double Torque = nan("");
		EngineEntry(double EngineSpeed, double Torque)
		{
			this->EngineSpeed = EngineSpeed;
			this->Torque = Torque;
		}
	};

	enum class PowerTrainTypes
	{
		Electric = 1,
		ICE = 0
	};

	class Vehicle
	{
	public:
		double Mass = nan(""); //kg

		double FrontalArea = nan(""); //m^2
		double DragCoefficient = nan("");

		double EngineUpperRevLimit = nan(""); //rpm
		vector<EngineEntry> EngineTorqueCurve;
		double EngineInertia = nan("");
		double AxleInertia = nan("");
		double PowertrainEfficiency = nan("");

		vector<TorqueSpeedEntry> TorqueSpeedCurve;
		PowerTrainTypes PowerTrainType = PowerTrainTypes::ICE;

		double WheelWidth = nan(""); //m /0.205
		double WheelRatioPercent = nan(""); //% width to height in Percent /75
		double WheelSize = nan(""); //inch R16 => 16
		double calcDynamicWheelRadius(double WheelWidth, double WheelRatioPercent, double WheelSize);
		double calcStaticWheelDiameter();
		double RollingResistanceCoefficient = nan("");

		double VMaxElectric = nan("");

		double calcTorqueAtWheel(double Velocity);
		double calcWheelspeed(double EngineSpeed);
		int NumberOfGears = 0;
		map<int, GearEntry> GearData;
		double FinalDriveRatio = nan("");
		double DeccelerationMax = nan("");

		double getSelectedGear();
		double setSelectedGear();
	};
}