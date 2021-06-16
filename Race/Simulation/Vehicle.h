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

	class Vehicle
	{
	public:
		double Mass = nan(""); //kg

		double FrontalArea = nan(""); //m^2
		double DragCoefficient = nan("");

		double EngineUpperRevLimit = nan(""); //rpm
		vector<EngineEntry> EngineTorqueCurve;
		double EngineInertia = nan("");
		double PowertrainEfficiency = nan("");

		double WheelWidth = nan(""); //m /0.205
		double WheelRatioPercent = nan(""); //% width to height in Percent /75
		double WheelSize = nan(""); //inch R16 => 16
		//double calcDynamicWheelDiameter(double Velocity);
		double calcStaticWheelDiameter();
		double RollingResistanceCoefficient = nan("");

		double VMaxElectric = nan("");

		double calcTorqueAtWheel(double Velocity);
		double calcWheelspeed(double EngineSpeed);
		int NumberOfGears = 0;
		map<int, GearEntry> GearData;
		double FinalDriveRatio = nan("");
	};
}