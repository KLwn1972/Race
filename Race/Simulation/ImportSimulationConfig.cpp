#include "ImportSimulationConfig.h"

//using namespace std;

namespace Simulation {
    ImportSimulationConfig::ImportSimulationConfig(std::string SimulationConfigFile) {
        std::fstream ConfigFileStream;
        ConfigFileStream.open(SimulationConfigFile, std::ios_base::in);
        if (!ConfigFileStream) {
            std::cerr << "Simulation Config Inputfile could not be opened.\n\n";
        }
        else {
            ConfigFileStream.seekg(0, std::ios::end);
            ConfigFileStream.seekg(0, std::ios::beg);            
            std::string ConfigFileAsString;
            ConfigFileAsString.assign((std::istreambuf_iterator<char>(ConfigFileStream)), std::istreambuf_iterator<char>());
            ConfigFileStream.close();
            saveSimulationConfig(ConfigFileAsString);            
        }
    }

    void ImportSimulationConfig::saveSimulationConfig(std::string ConfigFileAsString) {
        cJSON* completeConfigFile = cJSON_Parse(ConfigFileAsString.c_str());
        cJSON* Environment = cJSON_GetObjectItemCaseSensitive(completeConfigFile, "Environment");
        cJSON* VehicleData = cJSON_GetObjectItemCaseSensitive(completeConfigFile, "VehicleData");

    }






}
