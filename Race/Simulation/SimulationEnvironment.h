#pragma once
namespace Simulation
{
	class SimulationEnvironment
	{
	public:
		double calcAirPressure(double height);
		double calcAirTemperature();
		double calcWindSpeed(double VehicleDirection);
		double calcRoadResistance();

	private:
		double Airtemperature;
		double Airpressure;
		double Windspeed;
		double Winddirection; //[°];
	};

	static double GRAVITATIONALCONSTANT = 9.81;
	static double GASCONSTANT = 287.0529;
}
