// Race.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include<stdio.h>
#include "Race.h"
#include "OpenStreetMap.h"
#include "Ausgabe-Visualisierung.h"
#include "Simulation/Vehicle.h"
#include "Simulation/DataMap2D.h"
#include "Simulation/ImportSimulationConfig.h"
#include "Simulation/DrivingSimulator.h"
#include "Simulation/MockSimulationConfig.h"
#include "Simulation/SimulationEnvironment.h"
#include "Simulation/MiscFunctions.h"
#include "ExampleTracks.h"

#include "Soll_Fahrtbestimmung.h"

#include "DatAuf.h"

using namespace std;

int main()
{
	//std::cout << "Hello World!\n";
	vector<node> nodes;
	string route = "38566";
	OpenStreetMap* OSM_Nord = new OpenStreetMap(route);
	OSM_Nord->waysOffset = 3; // Ignoriere erste 3 Wege (Verbindungsstrasse)
	int retval = OSM_Nord->GetNodesFromOSM();

    // DATENAUFBEREITUNG
    if (retval == 0) {
        DatAuf::CalcDatAuf DatAuf_Nord;
        DatAuf_Nord.nodes = OSM_Nord->nodes;
        delete OSM_Nord;
        DatAuf_Nord.DataProcessing();
        //return-Wert einfuegen
    }





	///* Da noch Sued. Eigentlich eine beliebige Route
	//route = "38567";
	//OpenStreetMap* OSM_Sued = new OpenStreetMap(route);
	//if (OSM_Sued->GetNodesFromOSM() == 0){
	//}
	//delete OSM_Sued;
	//*/

	//NASA
	//double long_stuttgart = 9.206802;
	//double lat_stuttgart = 48.742211;
	//ElevationCalculator calc;
	//cout.setf(ios::fixed, ios::floatfield);
	//cout.precision(6);
	//cout << calc.getElevationFromSRTM_SIRCdata(long_stuttgart, lat_stuttgart) << endl;
	//cout << GeoCoordConversion::getGrad_From_WGS84Decimal(long_stuttgart) << endl;
	//cout << setw(20) << GeoCoordConversion::getMin_From_WGS84Decimal(long_stuttgart) << endl;
	//cout << setw(20) << GeoCoordConversion::getSeconds_From_WGS84Decimal(long_stuttgart) << endl;
	//cout << setw(20) << GeoCoordConversion::getDecimal_From_WGS84GradMinSec(9, 13, 24.4872) << endl;

	//Fahrphysik
	auto track = ExampleHillTrack();
	string SimulationConfigFile = "Testconfiguration/SimulationConfig.json";
	track.at(track.size() - 1).speedLimit = 10 * KMH2MS;
	auto SimulationConfig = Simulation::ImportSimulationConfig(SimulationConfigFile);
	auto Drivingsim = Simulation::DrivingSimulator(track, SimulationConfig);
	vector<node> result = Drivingsim.RunSimulation();
	Simulation::plotNodeVector(Drivingsim.ReturnModifiedTrack(), "simulationresult.csv");

	Soll_Fahrtbestimmung* SollFahrt = new Soll_Fahrtbestimmung();
	SollFahrt->setVehicle(SimulationConfig.getVehicle());
	SollFahrt->setEnvironment(SimulationConfig.getEnvironment());
	//SollFahrt->V_max();
	vector<node> Strecke = ExampleStraightTrack(0);
	SollFahrt->SpeedLimit_route(Strecke);

	
	//////////////////////////////////////////////////////////////////////////
	//Ausgabe-Visualisierung
	ausgabe_visualisierung(nodes, "Nordschleife");


	//////////////////////////////////////////////////////////////////////////


	return 0;
}