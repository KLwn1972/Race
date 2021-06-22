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

	Vehicle* VehicleObject = new Vehicle;
	SimulationEnvironment* EnvironmentObject = new SimulationEnvironment;

	EnvironmentObject->setAirtemperatureCelsius(getDoubleFromcJSON(Environment, "Airtemperature [C]"));
	EnvironmentObject->setWindspeed(getDoubleFromcJSON(Environment, "Windspeed [km/h]"));
	EnvironmentObject->setRollingResistanceCoefficient(getDoubleFromcJSON(Environment, "RollingResistanceCoefficient [-]"));
	EnvironmentObject->setFrictionTable(getVectorFromcJSON(Environment, "FrictionCoefficient_v_CUR [km/h]"), getVectorFromcJSON(Environment, "FrictionCoefficient_fac_CUR [-]"));
	/*
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
	*/

	//cJSON* FrictionCoefficient_v_CUR = cJSON_GetObjectItemCaseSensitive(Environment, "FrictionCoefficient_v_CUR [km/h]");
	//cJSON* FrictionCoefficient_fac_CUR = cJSON_GetObjectItemCaseSensitive(Environment, "FrictionCoefficient_fac_CUR [-]");
	//for (int i = 0; i < min(cJSON_GetArraySize(FrictionCoefficient_v_CUR), cJSON_GetArraySize(FrictionCoefficient_fac_CUR)); i++) {
	//	cJSON* CUR_Element_x = cJSON_GetArrayItem(FrictionCoefficient_v_CUR, i);
	//	cJSON* CUR_Element_y = cJSON_GetArrayItem(FrictionCoefficient_fac_CUR, i);
	//	cout << CUR_Element_x->valuedouble << endl;
	//	cout << CUR_Element_y->valuedouble << endl;
	//}

	//VehicleData
	cJSON* VehicleData = cJSON_GetObjectItemCaseSensitive(completeConfigFile, "VehicleData");
	VehicleData = VehicleData->child;
	VehicleObject->Manufacturer = getStringFromcJSON(VehicleData, "Manufacturer");
	VehicleObject->Model = getStringFromcJSON(VehicleData, "Model");
	VehicleObject->EngineInertia = getDoubleFromcJSON(VehicleData, "EngineInertia [kgm2]");
	VehicleObject->AxleInertia = getDoubleFromcJSON(VehicleData, "AxleInertia [kgm2]");
	VehicleObject->Mass = getDoubleFromcJSON(VehicleData, "Mass [kg]");
	VehicleObject->FrontalArea = getDoubleFromcJSON(VehicleData, "FrontalArea [m2]");
	VehicleObject->DragCoefficient = getDoubleFromcJSON(VehicleData, "DragCoefficient(cw) [-]");
	VehicleObject->FinalDriveRatio = getDoubleFromcJSON(VehicleData, "FinalDriveRatio [-]");
	VehicleObject->PowertrainEfficiency = getDoubleFromcJSON(VehicleData, "PowertrainEfficiency [-]");
	VehicleObject->DeccelerationMax = getDoubleFromcJSON(VehicleData, "DeccelerationMax [m/s2]");
	VehicleObject->WheelWidth = getDoubleFromcJSON(VehicleData, "WheelWidth");
	VehicleObject->WheelRatioPercent = getDoubleFromcJSON(VehicleData, "WheelRatioPercent");
	VehicleObject->WheelSize = getDoubleFromcJSON(VehicleData, "WheelSize");
	VehicleObject->WheelInertia = getDoubleFromcJSON(VehicleData, "WheelInertia [kgm2]");
	VehicleObject->VehiclespeedTorqueCurve = new DataMap2D(getVectorFromcJSON(VehicleData, "EngineTorque_v_CUR [km/h]"), getVectorFromcJSON(VehicleData, "EngineTorque_trq_CUR [Nm]"));

	//cJSON* EngineTorque_v_CUR = cJSON_GetObjectItemCaseSensitive(VehicleData, "EngineTorque_v_CUR [km/h]");
	//for (int i = 0; i < cJSON_GetArraySize(EngineTorque_v_CUR); i++) {
	//	cJSON* CUR_Element = cJSON_GetArrayItem(EngineTorque_v_CUR, i);
	//	cout << CUR_Element->valuedouble << endl;
	//}

	//cJSON* EngineTorque_trq_CUR = cJSON_GetObjectItemCaseSensitive(VehicleData, "EngineTorque_trq_CUR [N]");
	//for (int i = 0; i < cJSON_GetArraySize(EngineTorque_trq_CUR); i++) {
	//	cJSON* CUR_Element = cJSON_GetArrayItem(EngineTorque_trq_CUR, i);
	//	cout << CUR_Element->valuedouble << endl;
	//}

	//cJSON* EngineTorque_v_CUR = cJSON_GetObjectItemCaseSensitive(VehicleData, "EngineTorque_v_CUR [km/h]");
	//cJSON* EngineTorque_trq_CUR = cJSON_GetObjectItemCaseSensitive(VehicleData, "EngineTorque_trq_CUR [N]");
	//for (int i = 0; i < min(cJSON_GetArraySize(EngineTorque_v_CUR), cJSON_GetArraySize(EngineTorque_trq_CUR)); i++) {
	//	cJSON* CUR_Element_x = cJSON_GetArrayItem(EngineTorque_v_CUR, i);
	//	cJSON* CUR_Element_y = cJSON_GetArrayItem(EngineTorque_trq_CUR, i);
	//}
}

std::vector<double> Simulation::ImportSimulationConfig::getVectorFromcJSON(cJSON* data, const char* const string) {
	cJSON* input = cJSON_GetObjectItemCaseSensitive(data, string);
	std::vector<double> AxisValues;
	for (int i = 0; i < cJSON_GetArraySize(input); i++) {
		cJSON* CUR_Element = cJSON_GetArrayItem(input, i);
		AxisValues[i] = CUR_Element->valuedouble;
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
	return new Vehicle();
}

Simulation::SimulationEnvironment* Simulation::ImportSimulationConfig::getEnvironment()
{
	return new SimulationEnvironment();
}