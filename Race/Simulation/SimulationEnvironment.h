/*
Team Fahrphysik: Alexander Eirich, Yuying Zhu, Max Paschke

SimulationEnvironment provides Environmentdata to do the simulation.
*/

#pragma once
#ifndef SIMENVIRONMENT
#define SIMENVIRONMENT

#include <tuple>
#include <cmath>
#include <vector>
#include "DataMap1D.h"
#include <string>
#include "SimulationConstants.h"

namespace Simulation
{
	/// <summary>
	/// Provides data about the environment around the vehicle
	/// </summary>
	class SimulationEnvironment
	{
	public:
		double calcAirPressure(double height);
		double calcAirTemperatureInKelvin();
		/// <summary>
		/// Calculates the relevant windspeed for the resistance calculation
		/// </summary>
		/// <param name="VehicleDirection"></param>
		/// <returns></returns>
		double calcRelevantWindSpeed(double VehicleDirection); //Vehicledirection in [°]

		double calcFrictionCoefficient(double Velocity);
		double calcAirDensity(double height);
		double getRollingResistanceCoefficient();

		void setAirtemperatureCelsius(double Airtemperature);
		void setRollingResistanceCoefficient(double RoadResistanceCoefficient);

		void setAirpressure(double Airpressure);
		void setAirpressureHeight(double PressureHeight);
		void setWindspeed(double Windspeed);
		void setWinddirection(double Winddirection);
		void setFrictionTable(std::vector<double>, std::vector<double>);

	private:
		double Airtemperature = nan("");
		double Airpressure = 1013 * MBAR2PASCAL;
		double PressureHeight = 0.0;
		double Windspeed = nan("");
		double Winddirection = nan(""); //[°];
		double RollingResistanceCoefficient = nan("");
		DataMap1D FrictionTable; //VehicleSpeed[m/s], µ [-]
	};

	SimulationEnvironment* ExampleSimulationEnvironment();
}

#endif