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

///////////////////////////////////////////////////////////////////////
// 	   Usage Beispiele aus NASA Team
// 	   Vor Nutzung in NASA_constants.h anpassen: Pfade fuer Download
//		string nasa_download_zielpfad 
//		string nasa_download_zielpfad_win 
///////////////////////////////////////////////////////////////////////
#if 0	
	// Herunterladen aller HGT für Deutschland
	//NASA::NASADataFileHandler filehandle;
	//filehandle.downloadElevationDataofGermany_NASA_SIRC();

	//Einfaches Auslesen von Hoeheninformation zu Longitude / Latitude
	double long_stuttgart = 9.20;
	double lat_stuttgart = 48.80;
	cout << HGT_ElevationCalculator::getElevationFromSRTM_SIRCdata(long_stuttgart, lat_stuttgart) << endl;

	double long_nuerburgringstart = 6.966279;
	double lat_nuerburgringstart = 50.346094;
	//cout << HGT_ElevationCalculator::getElevationFromSRTM_SIRCdata(long_nuerburgringstart, lat_nuerburgringstart) << endl ;
#endif






#if 1
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
	vector<node> Strecke = ExampleStraightTrack(0);
	SollFahrt->SpeedLimit_route(Strecke);

	//////////////////////////////////////////////////////////////////////////
	//Ausgabe-Visualisierung
	ausgabe_visualisierung(nodes, "Nordschleife");

	//////////////////////////////////////////////////////////////////////////

#endif

	return 0;
}