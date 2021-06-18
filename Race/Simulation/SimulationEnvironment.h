#pragma once
#include <tuple>
#include <cmath>

namespace Simulation
{
	class SimulationEnvironment
	{
	public:
		double calcAirPressure(double height);
		double calcAirTemperatureInKelvin();
		double calcWindSpeed(double VehicleDirection);
		double calcRoadResistanceCoefficient();
		double calcAirDensity(double height);

		void setAirtemperatureCelsius(double Airtemperature);
		void setAirpressure(double Airpressure);
		void setWindspeed(double Windspeed);
		void setWinddirection(double Winddirection);

	private:
		double Airtemperature;
		double Airpressure;
		double Windspeed;
		double Winddirection; //[°];
	};

	const double GRAVITATIONALCONSTANT = 9.81;
	const double GASCONSTANT = 287.0529;
	const double ABSOLUTEZEROTEMP = 273.15;

	const double RPM2HZ = 1 / 60.0;
	const double HZ2RPM = 60;

	const double MS2KMH = 3.6;
	const double KMH2MS = 1 / 3.6;

	const double MBAR2PASCAL = 100;
	const double PASCAL2MBAR = 1 / 100.0;

	static const double PI = atan(1) * 4;
}
