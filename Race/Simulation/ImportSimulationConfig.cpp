#include "ImportSimulationConfig.h"

//using namespace std;
Simulation::ImportSimulationConfig::ImportSimulationConfig(std::string SimulationConfigFile) {
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

void Simulation::ImportSimulationConfig::saveSimulationConfig(std::string ConfigFileAsString) {
	cJSON* completeConfigFile = cJSON_Parse(ConfigFileAsString.c_str());
	cJSON* Environment = cJSON_GetObjectItemCaseSensitive(completeConfigFile, "Environment");
	Environment = Environment->child;

	Vehicle* car = new Vehicle;
	SimulationEnvironment* environment = new SimulationEnvironment;

	environment->setAirtemperatureCelsius(getDoubleFromcJSON(Environment, "Airtemperature [C]"));
	environment->setWindspeed(getDoubleFromcJSON(Environment, "Windspeed [km/h]"));
	environment->setRoadResistanceCoefficient(getDoubleFromcJSON(Environment, "RollingResistanceCoefficient [-]"));

	cJSON* FrictionCoefficient_v_CUR = cJSON_GetObjectItemCaseSensitive(Environment, "FrictionCoefficient_v_CUR [-]");
	for (int i = 0; i < cJSON_GetArraySize(FrictionCoefficient_v_CUR);i++) {
		cJSON* CUR_Element = cJSON_GetArrayItem(FrictionCoefficient_v_CUR, i);
		cout << CUR_Element->valuedouble << endl;
	}
	cJSON* FrictionCoefficient_fac_CUR = cJSON_GetObjectItemCaseSensitive(Environment, "FrictionCoefficient_fac_CUR [-]");
	for (int i = 0; i < cJSON_GetArraySize(FrictionCoefficient_fac_CUR); i++) {
		cJSON* CUR_Element = cJSON_GetArrayItem(FrictionCoefficient_fac_CUR, i);
		cout << CUR_Element->valuedouble << endl;
	}

	//VehicleData
	cJSON* VehicleData = cJSON_GetObjectItemCaseSensitive(completeConfigFile, "VehicleData");
	VehicleData = VehicleData->child;
	car->Manufacturer = getStringFromcJSON(VehicleData, "Manufacturer");
	car->Model = getStringFromcJSON(VehicleData, "Model");
	car->EngineInertia = getDoubleFromcJSON(VehicleData, "EngineInertia [kgm2]");
	car->AxleInertia = getDoubleFromcJSON(VehicleData, "AxleInertia [kgm2]");
	car->Mass = getDoubleFromcJSON(VehicleData, "Mass [kg]");
	car->FrontalArea = getDoubleFromcJSON(VehicleData, "FrontalArea [m2]");
	car->DragCoefficient = getDoubleFromcJSON(VehicleData, "DragCoefficient(cw) [-]");
	car->FinalDriveRatio = getDoubleFromcJSON(VehicleData, "FinalDriveRatio [-]");
	car->PowertrainEfficiency = getDoubleFromcJSON(VehicleData, "PowertrainEfficiency [-]");
	car->DeccelerationMax = getDoubleFromcJSON(VehicleData, "DeccelerationMax [m/s2]");
	car->WheelWidth = getDoubleFromcJSON(VehicleData, "WheelWidth");
	car->WheelRatioPercent = getDoubleFromcJSON(VehicleData, "WheelRatioPercent");
	car->WheelSize = getDoubleFromcJSON(VehicleData, "WheelSize");
	car->WheelInertia = getDoubleFromcJSON(VehicleData, "WheelInertia [kgm2]");
	
	cJSON* EngineTorque_v_CUR = cJSON_GetObjectItemCaseSensitive(VehicleData, "EngineTorque_v_CUR [km/h]");
	for (int i = 0; i < cJSON_GetArraySize(EngineTorque_v_CUR); i++) {
		cJSON* CUR_Element = cJSON_GetArrayItem(EngineTorque_v_CUR, i);
		cout << CUR_Element->valuedouble << endl;
	}

	cJSON* EngineTorque_trq_CUR = cJSON_GetObjectItemCaseSensitive(VehicleData, "EngineTorque_trq_CUR [N]");
	for (int i = 0; i < cJSON_GetArraySize(EngineTorque_trq_CUR); i++) {
		cJSON* CUR_Element = cJSON_GetArrayItem(EngineTorque_trq_CUR, i);
		cout << CUR_Element->valuedouble << endl;
	}

}

double Simulation::ImportSimulationConfig::getDoubleFromcJSON(cJSON* data, const char* const string) {
	cJSON* input = cJSON_GetObjectItemCaseSensitive(data, string);
	return input->valuedouble;
}

string Simulation::ImportSimulationConfig::getStringFromcJSON(cJSON* data, const char* const string) {
	cJSON* input = cJSON_GetObjectItemCaseSensitive(data, string);
	return input->valuestring;
}

Simulation::ImportSimulationConfig::~ImportSimulationConfig()
{
}

Simulation::Vehicle Simulation::ImportSimulationConfig::getVehicle()
{
	return Vehicle();
}

Simulation::SimulationEnvironment Simulation::ImportSimulationConfig::getEnvironment()
{
	return SimulationEnvironment();
}