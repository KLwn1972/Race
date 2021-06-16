#pragma once
#include <tuple>
#include <math.h>

namespace Simulation
{
	class SimulationEnvironment
	{
	public:
		double calcAirPressure(double height);
		double calcAirTemperature();
		double calcWindSpeed(double VehicleDirection);
		double calcRoadResistance();

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

	static double GRAVITATIONALCONSTANT = 9.81;
	static double GASCONSTANT = 287.0529;

	const double RPM2HZ = 1 / 60.0;
	const double HZ2RPM = 60;

	const double MS2KMH = 3.6;
	const double KMH2MS = 1 / 3.6;

	const double MBAR2PASCAL = 100;
	const double PASCAL2MBAR = 1 / 100.0;

	//Calculate coordinates, for returns see https://stackoverflow.com/questions/321068/returning-multiple-values-from-a-c-function
	static std::tuple<double, double> GeoCoordinates2KarthesianLongLat(double GeoLong, double GeoLat);
	static std::tuple<double, double> GeoCoordinates2KarthesianLatLong(double Lat, double Long);

	constexpr double pi() { return atan(1) * 4; }
}
