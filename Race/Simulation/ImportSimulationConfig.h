#pragma once
#ifndef  IMPORTCONFIG
#define IMPORTCONFIG

#include "..\cJSON.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include "SimulationEnvironment.h"
#include "Vehicle.h"
#include "IImportSimulationConfig.h"
#include "DataMap2D.h"

namespace Simulation {
	class ImportSimulationConfig : public IImportSimulationConfig
	{
	public:
		Vehicle* getVehicle();
		SimulationEnvironment* getEnvironment();
		ImportSimulationConfig(std::string SimulationConfigFile);
		void saveSimulationConfig(std::string ConfigFileAsString);
		double getDoubleFromcJSON(cJSON*, const char* const string);
		int getIntFromcJSON(cJSON*, const char* const string);
		string getStringFromcJSON(cJSON*, const char* const string);
		std::vector<double> getVectorFromcJSON(cJSON* data, const char* const string);
		std::vector<double> getVectorFromcJSON(cJSON* data, const char* const string, const double ConversionFactor);
		~ImportSimulationConfig();

	private:
		Vehicle* vehicle;
		SimulationEnvironment* environment;
	};
}

#endif
