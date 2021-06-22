#pragma once
#ifndef SIMENVIRONMENT
#define SIMENVIRONMENT

#include <tuple>
#include <cmath>
#include <vector>
#include "DataMap2D.h"

namespace Simulation
{
	class SimulationEnvironment
	{
	public:
		double calcAirPressure(double height);
		double calcAirTemperatureInKelvin();
		double calcRelevantWindSpeed(double VehicleDirection); //Vehicledirection in [°]
		double calcFrictionCoefficient(double Velocity);
		double calcAirDensity(double height);
		double getRollingResistanceCoefficient();

		void setAirtemperatureCelsius(double Airtemperature);
		void setRollingResistanceCoefficient(double RoadResistanceCoefficient);

		void setAirpressure(double Airpressure);
		void setWindspeed(double Windspeed);
		void setWinddirection(double Winddirection);
		void setFrictionTable(std::vector<double>, std::vector<double>);

	private:
		double Airtemperature = nan("");
		double Airpressure = nan("");
		double Windspeed = nan("");
		double Winddirection = nan(""); //[°];
		double RollingResistanceCoefficient = nan("");
		DataMap2D FrictionTable; //VehicleSpeed[m/s], µ [-]
	};

	//Conversions
	const double GRAVITATIONALCONSTANT = 9.81;
	const double GASCONSTANT = 287.0529;
	const double ABSOLUTEZEROTEMP = 273.15;

	const double RPM2HZ = 1 / 60.0;
	const double HZ2RPM = 60;

	const double MS2KMH = 3.6;
	const double KMH2MS = 1 / 3.6;

	const double MBAR2PASCAL = 100;
	const double PASCAL2MBAR = 1 / 100.0;

	const double RAD2DEG = 180.0 / atan(1) * 4;
	const double DEG2RAD = atan(1) * 4 / 180.0;

	const double INCH2M = 0.0254;
	const double M2INCH = 1 / INCH2M;

	static const double PI = atan(1) * 4;
	const std::string INTERPOLATEDIDENT = std::string("INTERPOLATEDIDENT");
	//

	SimulationEnvironment* ExampleSimulationEnvironment();
}

#endif