/*
Team Fahrphysik: Alexander Eirich, Yuying Zhu, Max Paschke

Vehicle holds all the data to describe the vehicle and some calculation functions
*/

#pragma once

#ifndef VEHICLEDEF
#define VEHICLEDEF

#include <vector>
#include <map>
#include "DataMap1D.h"
using namespace std;

namespace Simulation
{
	class GearEntry
	{
	public:
		double ShiftDownLimitMin = nan(""); //rpm at which driver should shift down
		double ShiftUpLimitMax = nan(""); //rpm at which driver should shift up
		DataMap1D LowerShiftLimit; //DataMap with engine speed [1/s], low shift pedal point
		DataMap1D UpperShiftLimit; //DataMap with engine speed [1/s], upshift pedal point
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
		DataMap1D* EngineSpeedTorqueCurve; //Curve with enginespeed, torque [1/s, Nm]
		double EngineInertia = nan("");
		double AxleInertia = nan("");
		double WheelInertia = nan("");
		double PowertrainEfficiency = nan("");

		DataMap1D* VehicleSpeedTorqueCurve; //Curve with speed, torque [m/s, Nm]
		PowerTrainTypes PowertrainType = PowerTrainTypes::Electric;

		double WheelWidth = nan(""); //m /0.205
		double WheelRatioPercent = nan(""); //% width to height in Percent /75
		double WheelSize = nan(""); //m     Hint: Convert inch to m on import (*INCH2M)
		double calcDynamicWheelRadius();
		double calcStaticWheelDiameter();

		double VMaxLimited = nan("");
		double EngineSpeedMax = nan("");

		double interpolateEngineTorqueFromVelocity(double Velocity); //Team Soll_Fahrtbestimmung
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

#endif