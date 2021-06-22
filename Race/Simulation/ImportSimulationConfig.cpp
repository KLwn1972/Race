#include "ImportSimulationConfig.h"

//using namespace std;
Simulation::ImportSimulationConfig::ImportSimulationConfig(std::string SimulationConfigFile) {
	this->vehicle = new Vehicle();
	this->environment = new SimulationEnvironment();

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
	cJSON* VehicleData = cJSON_GetObjectItemCaseSensitive(completeConfigFile, "VehicleData");
	VehicleData = VehicleData->child;

	Vehicle* VehicleObject = getVehicle();
	SimulationEnvironment* EnvironmentObject = getEnvironment();

	//EnvironmentData
	EnvironmentObject->setAirtemperatureCelsius(getDoubleFromcJSON(Environment, "Airtemperature [C]"));
	EnvironmentObject->setWindspeed(getDoubleFromcJSON(Environment, "Windspeed [km/h]") * KMH2MS);
	EnvironmentObject->setRollingResistanceCoefficient(getDoubleFromcJSON(Environment, "RollingResistanceCoefficient [-]"));
	EnvironmentObject->setFrictionTable(getVectorFromcJSON(Environment, "FrictionCoefficient_v_CUR [km/h]", KMH2MS), getVectorFromcJSON(Environment, "FrictionCoefficient_fac_CUR [-]"));

	//VehicleData
	VehicleObject->Manufacturer = getStringFromcJSON(VehicleData, "Manufacturer");
	VehicleObject->Model = getStringFromcJSON(VehicleData, "Model");
	//VehicleObject->PowertrainType = getStringFromcJSON(VehicleData, "PowertrainType");
	VehicleObject->VMaxLimited = getDoubleFromcJSON(VehicleData, "VMaxLimited");
	VehicleObject->NumberOfGears = getDoubleFromcJSON(VehicleData, "NumberOfGears");
	VehicleObject->EngineInertia = getDoubleFromcJSON(VehicleData, "EngineInertia [kgm2]");
	VehicleObject->AxleInertia = getDoubleFromcJSON(VehicleData, "AxleInertia [kgm2]");
	VehicleObject->Mass = getDoubleFromcJSON(VehicleData, "Mass [kg]");
	VehicleObject->FrontalArea = getDoubleFromcJSON(VehicleData, "FrontalArea [m2]");
	VehicleObject->DragCoefficient = getDoubleFromcJSON(VehicleData, "DragCoefficient(cw) [-]");
	VehicleObject->FinalDriveRatio = getDoubleFromcJSON(VehicleData, "FinalDriveRatio [-]");
	VehicleObject->PowertrainEfficiency = getDoubleFromcJSON(VehicleData, "PowertrainEfficiency [-]");
	VehicleObject->DeccelerationMax = getDoubleFromcJSON(VehicleData, "DeccelerationMax [m/s2]");
	VehicleObject->WheelWidth = getDoubleFromcJSON(VehicleData, "WheelWidth [mm]") / 1000;
	VehicleObject->WheelRatioPercent = getDoubleFromcJSON(VehicleData, "WheelRatioPercent [%]");
	VehicleObject->WheelSize = getDoubleFromcJSON(VehicleData, "WheelSize [inch]") * INCH2M;
	VehicleObject->WheelInertia = getDoubleFromcJSON(VehicleData, "WheelInertia [kgm2]");
	VehicleObject->VehiclespeedTorqueCurve = new DataMap2D(getVectorFromcJSON(VehicleData, "VehiclespeedTorque_v_CUR [km/h]", KMH2MS), getVectorFromcJSON(VehicleData, "VehiclespeedTorque_trq_CUR [Nm]"));
	VehicleObject->EnginespeedTorqueCurve = new DataMap2D(getVectorFromcJSON(VehicleData, "EnginespeedTorque_n_CUR [1/min]", RPM2HZ), getVectorFromcJSON(VehicleData, "EnginespeedTorque_trq_CUR [Nm]"));

	this->vehicle = VehicleObject;
	this->environment = EnvironmentObject;
}

std::vector<double> Simulation::ImportSimulationConfig::getVectorFromcJSON(cJSON* data, const char* const ConfigString) {
	cJSON* input = cJSON_GetObjectItemCaseSensitive(data, ConfigString);
	std::vector<double> AxisValues;
	for (int i = 0; i < cJSON_GetArraySize(input); i++) {
		cJSON* CUR_Element = cJSON_GetArrayItem(input, i);
		AxisValues.push_back(CUR_Element->valuedouble);
	}
	return AxisValues;
}

std::vector<double> Simulation::ImportSimulationConfig::getVectorFromcJSON(cJSON* data, const char* const string, const double ConversionFacto) {
	cJSON* input = cJSON_GetObjectItemCaseSensitive(data, string);
	std::vector<double> AxisValues;
	for (int i = 0; i < cJSON_GetArraySize(input); i++) {
		cJSON* CUR_Element = cJSON_GetArrayItem(input, i);
		AxisValues.push_back(CUR_Element->valuedouble * ConversionFacto);
	}
	return AxisValues;
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

Simulation::Vehicle* Simulation::ImportSimulationConfig::getVehicle()
{
	return this->vehicle;
}

Simulation::SimulationEnvironment* Simulation::ImportSimulationConfig::getEnvironment()
{
	return this->environment;
}