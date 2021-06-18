#pragma once
#include <tuple>
#include <cmath>
#include <vector>

namespace Simulation
{
	class FrictionEntry {
	public:
		double VehicleSpeed = nan("");
		double FrictionCoefficient = nan("");
		FrictionEntry(double VehicleSpeed, double FrictionCoefficient)
		{
			this->VehicleSpeed = VehicleSpeed;
			this->FrictionCoefficient = FrictionCoefficient;
		}
	};

	class SimulationEnvironment
	{
	public:
		double calcAirPressure(double height);
		double calcAirTemperatureInKelvin();
		double calcRelevantWindSpeed(double VehicleDirection); //Vehicledirection in [°]
		double calcRoadResistanceCoefficient();
		double calcFrictionCoefficient(double Velocity, double Gradient);
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
		std::vector<FrictionEntry> FrictionTable;
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

	static const double RAD2DEG = 180.0 / atan(1) * 4;
	static const double DEG2RAD = atan(1) * 4 / 180.0;

	static const double PI = atan(1) * 4;
}
