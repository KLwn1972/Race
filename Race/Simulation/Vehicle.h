#pragma once

#include <vector>
#include <map>
using namespace std;

namespace Simulation
{
	class ShiftEntry
	{
		double EngineSpeed;
		double PedalPosition;
	};

	class GearEntry
	{
	public:
		double ShiftDownLimitMin; //rpm at which driver should shift down
		double ShiftUpLimitMax; //rpm at which driver should shift up
		vector<ShiftEntry> LowerShiftLimit;
		vector<ShiftEntry> UpperShiftLimit;
		double GearRatio;
	};

	class EngineEntry
	{
	public:
		double EngineSpeed;
		double Torque;
		EngineEntry(double EngineSpeed, double Torque)
		{
			this->EngineSpeed = EngineSpeed;
			this->Torque = Torque;
		}
	};

	class Vehicle
	{
	public:
		double Mass; //kg

		double FrontalArea; //m^2
		double DragCoefficient;

		double EngineUpperRevLimit; //rpm
		vector<EngineEntry> EngineTorqueCurve;
		double EngineInertia;

		double WheelWidth; //mm /205
		double WheelRatioPercent; //width to height /75
		double WheelSize; //inch R16 => 16
		//double calcDynamicWheelDiameter(double Velocity);
		double calcStaticWheelDiameter();
		double RollingResistanceCoefficient;

		double VMaxElectric;

		double calcTorqueAtWheel(double Velocity);
		int NumberOfGears;
		map<int, GearEntry> GearData;
		double FinalDriveRatio;
	};
}