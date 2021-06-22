#pragma once

#include <vector>
#include <map>
#include "DataMap2D.h"
using namespace std;

namespace Simulation
{
	class GearEntry
	{
	public:
		double ShiftDownLimitMin = nan(""); //rpm at which driver should shift down
		double ShiftUpLimitMax = nan(""); //rpm at which driver should shift up
		DataMap2D LowerShiftLimit; //DataMap with engine speed [1/s], low shift pedal point
		DataMap2D UpperShiftLimit; //DataMap with engine speed [1/s], upshift pedal point
		double GearRatio = nan("");
	};

	enum class PowerTrainTypes
	{
		Electric = 1,
		ICE = 0
	};

	class Vehicle
	{
	public:
		string Manufacturer = "";
		string Model = "";

		double Mass = nan(""); //kg

		double FrontalArea = nan(""); //m^2
		double DragCoefficient = nan("");

		double EngineUpperRevLimit = nan(""); //rpm
		DataMap2D* EngineTorqueCurve; //Curve with enginespeed, torque [1/s, Nm]
		double EngineInertia = nan("");
		double AxleInertia = nan("");
		double WheelInertia = nan("");
		double PowertrainEfficiency = nan("");

		DataMap2D* VehiclespeedTorqueCurve; //Curve with speed, torque [m/s, Nm]
		PowerTrainTypes PowerTrainType = PowerTrainTypes::ICE;

		double WheelWidth = nan(""); //m /0.205
		double WheelRatioPercent = nan(""); //% width to height in Percent /75
		double WheelSize = nan(""); //inch R16 => 16
		double calcDynamicWheelRadius();
		double calcStaticWheelDiameter();
		double RollingResistanceCoefficient = nan("");

		double VMaxElectric = nan("");

		double interpolateEngineTorqueFromVelocity(double Velocity);
		double calcWheelspeed(double EngineSpeed);
		int NumberOfGears = 0;
		map<int, GearEntry> GearData;
		double FinalDriveRatio = nan("");
		double DeccelerationMax = nan("");

		int getSelectedGear();
		void setSelectedGear(int GearToSet);
		Vehicle();
		~Vehicle();
	private:
		int selectedgear = 0;
	};

	Vehicle* ExampleElectricVehicle();
}