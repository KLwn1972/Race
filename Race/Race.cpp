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

#include "NASA_src/NASA_constants.h"
#include "NASA_src/NASA_DataFileHandler.h"
#include "NASA_src/NASA_ElevationCalculator.h"
#include "NASA_src/NASA_GeoCoordConversion.h"

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
#if 1	
	// Herunterladen aller HGT für Deutschland
	//NASA::NASADataFileHandler filehandle;
	//filehandle.downloadElevationDataofGermany_NASA_SIRC();

	//Einfaches Auslesen von Hoeheninformation zu Longitude / Latitude
	double long_stuttgart = 9.206802;
	double lat_stuttgart = 48.742211;
	//cout << NASA::HGT_ElevationCalculator::getElevationFromSRTM_SIRCdata(long_stuttgart, lat_stuttgart) << endl;
	//cout << NASA::HGT_ElevationCalculator::getElevationFromSRTM_SIRCdata(1+long_stuttgart, 1+lat_stuttgart) << endl;
	//cout << NASA::HGT_ElevationCalculator::getElevationFromSRTM_SIRCdata(0.234+long_stuttgart, 0.145+lat_stuttgart) << endl;
	//cout << NASA::HGT_ElevationCalculator::getElevationFromSRTM_SIRCdata(0.734+long_stuttgart, -0.145+lat_stuttgart) << endl;*/

	double long_nuerburgringstart = 6.966279;
	double lat_nuerburgringstart = 50.346094;
	//cout << NASA::HGT_ElevationCalculator::getElevationFromSRTM_SIRCdata(long_nuerburgringstart, lat_nuerburgringstart) << endl;
	cout << HGT_ElevationCalculator::getElevationFromSRTM_SIRCdata(long_nuerburgringstart, lat_nuerburgringstart) << endl ;
#endif





#if 0
	//std::cout << "Hello World!\n";
	vector<node> nodes;
	string route = "38566";
	OpenStreetMap* OSM_Nord = new OpenStreetMap(route);
	OSM_Nord->waysOffset = 3; // Ignoriere erste 3 Wege (Verbindungsstrasse)
	int retval = OSM_Nord->GetNodesFromOSM();

    //DATENAUFBEREITUNG
    DatAuf::CalcDatAuf DatAuf_Nord;
    DatAuf_Nord.nodes = OSM_Nord->nodes;
    delete OSM_Nord;
    DatAuf_Nord.DataProcessing();

	///* Da noch Sued. Eigentlich eine beliebige Route
	//route = "38567";
	//OpenStreetMap* OSM_Sued = new OpenStreetMap(route);
	//if (OSM_Sued->GetNodesFromOSM() == 0){
	//}
	//delete OSM_Sued;
	//*/

	//Fahrphysik
	//auto track = ExampleHillTrack();
	//string SimulationConfigFile = "Testconfiguration/SimulationConfig.json";
	//track.at(track.size() - 1).speedLimit = 10 * KMH2MS;
	//auto SimulationConfig = Simulation::ImportSimulationConfig(SimulationConfigFile);
	//auto Drivingsim = Simulation::DrivingSimulator(track, SimulationConfig);
	//vector<node> result = Drivingsim.RunSimulation();
	//Simulation::plotNodeVector(Drivingsim.ReturnModifiedTrack(), "simulationresult.csv");

	//vector<double> xdata = vector<double>{ 0,1,2,3,5,6,7 };
	//vector<double> ydata = vector<double>{ 0,100,200,300,500,600,700 };
	//Simulation::DataMap2D* Datamap = new Simulation::DataMap2D(xdata, ydata);
	//cout << Datamap->getY(-1) << "\n";
	//cout << Datamap->getY(1.9) << "\n";
	//cout << Datamap->getY(800) << "\n";

	//Soll_Fahrtbestimmung* SollFahrt = new Soll_Fahrtbestimmung();
	//SollFahrt->setVehicle(SimulationConfig.getVehicle());
	//SollFahrt->setEnvironment(SimulationConfig.getEnvironment());
	////SollFahrt->V_max();
	//vector<node> Strecke = ExampleStraightTrack(0);
	//SollFahrt->SpeedLimit_route(Strecke);

	//////////////////////////////////////////////////////////////////////////
	//Ausgabe-Visualisierung
	ausgabe_visualisierung(nodes, "Nordschleife");

	//////////////////////////////////////////////////////////////////////////

#endif

	return 0;
}