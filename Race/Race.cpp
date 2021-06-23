// Race.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include<stdio.h>
#include "Race.h"
#include "OpenStreetMap.h"
#include "Ausgabe-Visualisierung.h"
#include "Simulation/Vehicle.h"
#include "Simulation/DataMap1D.h"
#include "Simulation/ImportSimulationConfig.h"
#include "Simulation/DrivingSimulator.h"
#include "Simulation/MockSimulationConfig.h"
#include "Simulation/SimulationEnvironment.h"
#include "Simulation/MiscFunctions.h"
#include "ExampleTracks.h"

#include "Soll_Fahrtbestimmung.h"

#include "DatAuf.h"

using namespace std;

//#define USEDEBUGMAIN

#ifndef USEDEBUGMAIN

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
	double long_stuttgart = 9.206802;
	double lat_stuttgart = 48.742211;
	cout << HGT_ElevationCalculator::getElevationFromSRTM_SIRCdata(long_stuttgart, lat_stuttgart) << endl;

	double long_nuerburgringstart = 6.966279;
	double lat_nuerburgringstart = 50.346094;
	cout << HGT_ElevationCalculator::getElevationFromSRTM_SIRCdata(long_nuerburgringstart, lat_nuerburgringstart) << endl;
#endif

#if 1
	//////////////////////////////////////////////////////////////////////////
	// Datenbeschaffungsteam
	// Sued: route = "38567";
	vector<node> nodes;
	string route = "38566"; // Nord
	OpenStreetMap* OSM_Nord = new OpenStreetMap(route);
	OSM_Nord->waysOffset = 3; // Ignoriere erste 3 Wege (Verbindungsstrasse)
	int retval = OSM_Nord->GetNodesFromOSM();
	nodes = OSM_Nord->nodes;
	delete OSM_Nord;
	if (retval != 0) {
		// Fehler download
		return -1;
	}

	//////////////////////////////////////////////////////////////////////////
	// DATENAUFBEREITUNG
	if (nodes.size() > 4) {
		DatAuf::CalcDatAuf DatAuf_Nord;
		DatAuf_Nord.nodes = nodes;
		DatAuf_Nord.DataProcessing();
		//return-Wert einfuegen
		//return -1;
		nodes = DatAuf_Nord.nodes;
	}
	else {
		// Fehler mindestans vier Nodes
		return -1;
	}

	// Load Konfiguration für Sollfahrtbestimmung und Fahrphysik
	auto SimulationConfig = Simulation::ImportSimulationConfig("Testconfiguration/SimulationConfig_ModelSPerf.json");

	//////////////////////////////////////////////////////////////////////////
	//Sollfahrtbestimmung
	Soll_Fahrtbestimmung* SollFahrt = new Soll_Fahrtbestimmung();
	SollFahrt->setVehicle(SimulationConfig.getVehicle());
	SollFahrt->setEnvironment(SimulationConfig.getEnvironment());
	SollFahrt->SpeedLimit_route(nodes);

	//////////////////////////////////////////////////////////////////////////
	//Fahrphysik
	Simulation::DrivingSimulator* Drivingsim = new Simulation::DrivingSimulator(nodes, SimulationConfig);
	nodes.clear();
	nodes = Drivingsim->RunSimulation();
	Simulation::plotNodeVector(Drivingsim->ReturnModifiedTrack(), "simulationresult.csv");

	//////////////////////////////////////////////////////////////////////////
	//Ausgabe-Visualisierung
	//ausgabe_visualisierung(nodes, "Nordschleife");

#endif

	return 0;
}

#endif // STANDARDMAIN

#ifdef USEDEBUGMAIN
int main()
{
	// Load Konfiguration für Sollfahrtbestimmung und Fahrphysik
	auto SimulationConfig = Simulation::ImportSimulationConfig("Testconfiguration/SimulationConfig_ModelSPerf.json");

	auto nodes = ExampleHillTrack();
	Simulation::DrivingSimulator* Drivingsim = new Simulation::DrivingSimulator(nodes, SimulationConfig);
	//nodes.clear();
	//Drivingsim->setInterpolationLevel(0);
	//nodes = Drivingsim->RunSimulation();
	//Simulation::plotNodeVector(Drivingsim->ReturnModifiedTrack(), "simulationresultModelS_0_Hilltrack.csv");
	//Drivingsim->setInterpolationLevel(1);
	//nodes = Drivingsim->RunSimulation();
	//Simulation::plotNodeVector(Drivingsim->ReturnModifiedTrack(), "simulationresultModelS_1_Hilltrack.csv");

	nodes = ExampleStraightTrack();
	nodes.at(nodes.size() - 1).speedLimit = 1;
	nodes.at(nodes.size() / 2).speedLimit = 1;
	nodes.at(nodes.size() / 3).speedLimit = 1;

	Drivingsim = new Simulation::DrivingSimulator(nodes, SimulationConfig);
	nodes = Drivingsim->RunSimulation();
	Simulation::plotNodeVector(Drivingsim->ReturnModifiedTrack(), "simulationresultModelS_0_Straight_Speed.csv");

	Drivingsim->setInterpolationLevel(1);
	nodes = Drivingsim->RunSimulation();
	Simulation::plotNodeVector(Drivingsim->ReturnModifiedTrack(), "simulationresultModelS_0_Straight_Speed.csv");
}
#endif