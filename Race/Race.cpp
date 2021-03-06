// Race.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

// Debug auf X86 einstellen, da sonst Pfad zur NASA-Datei falsch.

#include<stdio.h>
#include <string>
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
#include "Testing.h"
#include <fstream>
#include "Soll_Fahrtbestimmung.h"

#include "DatAuf.h"

using namespace std;

//#define USEDEBUGMAIN

#ifndef USEDEBUGMAIN

int main()
{
#if 0	
	///////////////////////////////////////////////////////////////////////
	// 	   Usage Beispiele aus NASA Team
	// 	   Beschreibung in NASA_constants.h lesen.
	///////////////////////////////////////////////////////////////////////

	// Herunterladen aller HGT für Deutschland
	//NASADataFileHandler filehandle;
	//filehandle.downloadElevationDataofGermany_NASA_SIRC();

	//Einfaches Auslesen von Hoeheninformation zu Longitude / Latitude
	double long_stuttgart = 9.20;
	double lat_stuttgart = 48.80;
	cout << HGT_ElevationCalculator::getElevationFromSRTM_SIRCdata(long_stuttgart, lat_stuttgart) << endl;

	double long_nuerburgringstart = 6.94067417;
	double lat_nuerburgringstart = 50.3309196;
	cout << HGT_ElevationCalculator::getElevationFromSRTM_SIRCdata(long_nuerburgringstart, lat_nuerburgringstart) << endl;
#endif


#if 1
	//////////////////////////////////////////////////////////////////////////
	//Initialisierung Testing Log
	ErrorLog elog = ErrorLog();
	Testing Inittest;
	Inittest.Aufgabe = "Aufgabe";
	Inittest.Testname = "Testname";
	Inittest.Ergebnisse = "Ergebnisse";
	elog.Testvektor.push_back(Inittest);

	//////////////////////////////////////////////////////////////////////////
	/* Datenbeschaffung OpenStreetMap
		Route: Eine beliebige Route
		Nord: route = "38566" -> waysOffset = 3
		Sued: route = "38567"
		Gesamt: route = "Nuerburgring_nord_sued" -> Einfach die Datei in %temp% kopieren
	*/
	vector<node> nodes; // Alle Knoten von Route
	string route = "38566"; // Nord Ring
	OpenStreetMap* OSM_Nord = new OpenStreetMap(route);
	OSM_Nord->waysOffset = 3; // Ignoriere erste 3 Wege (Verbindungsstrassen): Nur fuer die route "38566".
	std::cout << "Start Download the route " << route << ": " << endl;
	int retval = OSM_Nord->GetNodesFromOSM();
	if (retval != 0) {
		std::cout << endl << "#####ERROR: The download failed!!!" << endl;
		return -1;
	}

	std::cout << endl << "The download was successful." << endl;
	nodes = OSM_Nord->nodes;
	delete OSM_Nord;
	OSM_Nord = nullptr;
  
  elog.TestDatenbeschaffung(nodes);

	//////////////////////////////////////////////////////////////////////////
	// DATENAUFBEREITUNG
	if (nodes.size() > 4) {
		DatAuf::CalcDatAuf DatAuf_Nord;
		DatAuf_Nord.nodes = nodes;
		retval = DatAuf_Nord.DataProcessing();
		nodes = DatAuf_Nord.nodes;
		if (retval != 0) {
			return -1;  // Fehler Datenaufbereitung
		}
	}
	else {		
		return -1;    // Fehler weniger wie 4 Nodes
	}

	elog.TestDatenAufbereitung(nodes);
	
	// Load Konfiguration für Sollfahrtbestimmung und Fahrphysik
	auto SimulationConfig = new Simulation::ImportSimulationConfig("Testconfiguration/SimulationConfig_ModelSPerf.json");

	//////////////////////////////////////////////////////////////////////////
	//Sollfahrtbestimmung
	Soll_Fahrtbestimmung* SollFahrt = new Soll_Fahrtbestimmung();
	SollFahrt->setVehicle(SimulationConfig->getVehicle());
	SollFahrt->setEnvironment(SimulationConfig->getEnvironment());
	SollFahrt->SpeedLimit_route(nodes);

	elog.TestSollfahrtbestimmung(nodes);

	//////////////////////////////////////////////////////////////////////////
	//Fahrphysik
	elog.PreTestFahrphysik(nodes);

	Simulation::DrivingSimulator* Drivingsim = new Simulation::DrivingSimulator(nodes, SimulationConfig);
	nodes.clear();
	nodes = Drivingsim->RunSimulation();
	Simulation::plotNodeVector(Drivingsim->ReturnModifiedTrack(), "simulationresult.csv");

	elog.TestFahrphysik(nodes);

	//////////////////////////////////////////////////////////////////////////
	//Ausgabe-Visualisierung
	ausgabe_visualisierung(nodes, "Nordschleife");

	//Ausgabe Testlog
	size_t Sizetestvektor = elog.Testvektor.size();
	ofstream outputfile;
	outputfile.open("ErrorLog.txt");
	for (int i = 0; i < Sizetestvektor; i++) {
		outputfile << elog.Testvektor[i].Aufgabe << "," << "\t" << elog.Testvektor[i].Testname << "," << "\t" << elog.Testvektor[i].Ergebnisse << "\n";
	}
	outputfile.close();


#endif

	return 0;
}

#endif // STANDARDMAIN

#ifdef USEDEBUGMAIN
int main()
{
	// Load Konfiguration für Sollfahrtbestimmung und Fahrphysik
	auto SimulationConfig = new Simulation::ImportSimulationConfig("Testconfiguration/SimulationConfig_ModelSPerf.json");

	auto nodes = ExampleHillTrack();
	Simulation::DrivingSimulator* Drivingsim = new Simulation::DrivingSimulator(nodes, SimulationConfig);
	Drivingsim->setInterpolationLevel(0);
	nodes = Drivingsim->RunSimulation();
	Simulation::plotNodeVector(Drivingsim->ReturnModifiedTrack(), "simulationresultModelS_0_Hilltrack.csv");
	Drivingsim->setInterpolationLevel(1);
	nodes = Drivingsim->RunSimulation();
	Simulation::plotNodeVector(Drivingsim->ReturnModifiedTrack(), "simulationresultModelS_1_Hilltrack.csv");
	Drivingsim->setInterpolationLevel(4);
	nodes = Drivingsim->RunSimulation();
	Simulation::plotNodeVector(Drivingsim->ReturnModifiedTrack(), "simulationresultModelS_4_Hilltrack.csv");
	Drivingsim->setInterpolationLevel(10);
	nodes = Drivingsim->RunSimulation();
	Simulation::plotNodeVector(Drivingsim->ReturnModifiedTrack(), "simulationresultModelS_10_Hilltrack.csv");

	nodes = ExampleStraightTrack();
	nodes.at(nodes.size() - 1).speedLimit = 1;
	nodes.at(nodes.size() / 2).speedLimit = 1;
	nodes.at(nodes.size() / 3).speedLimit = 1;

	Drivingsim = new Simulation::DrivingSimulator(nodes, SimulationConfig);
	nodes = Drivingsim->RunSimulation();
	Simulation::plotNodeVector(Drivingsim->ReturnModifiedTrack(), "simulationresultModelS_0_Straight_Speed.csv");

	Drivingsim->setInterpolationLevel(1);
	nodes = Drivingsim->RunSimulation();
	Simulation::plotNodeVector(Drivingsim->ReturnModifiedTrack(), "simulationresultModelS_1_Straight_Speed.csv");

	Drivingsim->setInterpolationLevel(5);
	nodes = Drivingsim->RunSimulation();
	Simulation::plotNodeVector(Drivingsim->ReturnModifiedTrack(), "simulationresultModelS_5_Straight_Speed.csv");

	Drivingsim->setInterpolationLevel(10);
	nodes = Drivingsim->RunSimulation();
	Simulation::plotNodeVector(Drivingsim->ReturnModifiedTrack(), "simulationresultModelS_10_Straight_Speed.csv");

	Drivingsim->setInterpolationLevel(20);
	nodes = Drivingsim->RunSimulation();
	Simulation::plotNodeVector(Drivingsim->ReturnModifiedTrack(), "simulationresultModelS_20_Straight_Speed.csv");

	nodes = ExampleHillTrack();
	auto SimulationConfigSmart = new Simulation::ImportSimulationConfig("Testconfiguration/SimulationConfig_SMARTe.json");
	Simulation::DrivingSimulator* DrivingsimSmart = new Simulation::DrivingSimulator(nodes, SimulationConfigSmart);
	DrivingsimSmart->setInterpolationLevel(0);
	nodes = DrivingsimSmart->RunSimulation();
	Simulation::plotNodeVector(DrivingsimSmart->ReturnModifiedTrack(), "simulationresultSmart_0_Hilltrack.csv");
	DrivingsimSmart->setInterpolationLevel(1);
	nodes = DrivingsimSmart->RunSimulation();
	Simulation::plotNodeVector(DrivingsimSmart->ReturnModifiedTrack(), "simulationresultSmart_1_Hilltrack.csv");
	DrivingsimSmart->setInterpolationLevel(4);
	nodes = DrivingsimSmart->RunSimulation();
	Simulation::plotNodeVector(DrivingsimSmart->ReturnModifiedTrack(), "simulationresultSmart_4_Hilltrack.csv");
	DrivingsimSmart->setInterpolationLevel(10);
	nodes = DrivingsimSmart->RunSimulation();
	Simulation::plotNodeVector(Drivingsim->ReturnModifiedTrack(), "simulationresultSmart_10_Hilltrack.csv");

	nodes = ExampleStraightTrack();
	nodes.at(nodes.size() - 1).speedLimit = 1;
	nodes.at(nodes.size() / 2).speedLimit = 1;
	nodes.at(nodes.size() / 3).speedLimit = 1;

	DrivingsimSmart = new Simulation::DrivingSimulator(nodes, SimulationConfigSmart);
	nodes = DrivingsimSmart->RunSimulation();
	Simulation::plotNodeVector(DrivingsimSmart->ReturnModifiedTrack(), "simulationresultSmart_0_Straight_Speed.csv");

	DrivingsimSmart->setInterpolationLevel(1);
	nodes = DrivingsimSmart->RunSimulation();
	Simulation::plotNodeVector(DrivingsimSmart->ReturnModifiedTrack(), "simulationresultSmart_1_Straight_Speed.csv");

	DrivingsimSmart->setInterpolationLevel(4);
	nodes = DrivingsimSmart->RunSimulation();
	Simulation::plotNodeVector(DrivingsimSmart->ReturnModifiedTrack(), "simulationresultSmart_4_Straight_Speed.csv");

	DrivingsimSmart->setInterpolationLevel(10);
	nodes = DrivingsimSmart->RunSimulation();
	Simulation::plotNodeVector(DrivingsimSmart->ReturnModifiedTrack(), "simulationresultSmart_4_Straight_Speed.csv");
}
#endif