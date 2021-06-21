// Race.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include<stdio.h>
#include "Race.h"
#include "OpenStreetMap.h"

#include "NASA_ElevationDataDownloader.h"
#include "NASA_ElevationCalculator.h"
#include "NASA_GeoCoordConversion.h"

#include "Simulation/Vehicle.h"
#include "Simulation/DataMap2D.h"
#include "Simulation/DrivingSimulator.h"
#include "Simulation/ImportSimulationConfig.h"

using namespace std;

int main()
{
	std::cout << "Hello World!\n";
	
	string route = "38566";
	OpenStreetMap* OSM_Nord = new OpenStreetMap(route);
	/*Ei: Langer Import der Knoten auskommentiert
	
	int retval = OSM_Nord->GetNodesFromOSM();

	// Hier macht Datenaufbereitung weiter
	if (retval == 0) {
		vector<node> nodes = OSM_Nord->nodes;
		//output_gpx(nodes, "output.gpx"); //funktioniert noch nicht, da raceTime fehlt
		output_kml(nodes, "output.kml");
	}
	// Wenn nicht mehr benötigt wird
	delete OSM_Nord;

	/* Da noch Sued. Eigentlich eine beliebige Route
	route = "38567";
	OpenStreetMap* OSM_Sued = new OpenStreetMap(route);
	if (OSM_Sued->GetNodesFromOSM() == 0){
	}
	delete OSM_Sued;
	*/

	//NASA
	FileDownloader testlader;
	testlader.downloadFile("Ha", "Hi");
	
	
	//Fahrphysik
	//Simulation::ExampleElectricVehicle();
	//TODO: Build Testtrack;
	vector<double> xdata = vector<double>{ 0,1,2,3,5,6,7 };
	vector<double> ydata = vector<double>{ 0,100,200,300,500,600,700 };
	Simulation::DataMap2D* Datamap = new Simulation::DataMap2D(xdata, ydata);
	cout << Datamap->getY(-1) << "\n";
	cout << Datamap->getY(1.9) << "\n";
	cout << Datamap->getY(800) << "\n";

	//
	vector<node> nodes = OSM_Nord->nodes;
	//

	string SimulationConfigFile = "Testconfiguration/SimulationConfig.json";
	Simulation::ImportSimulationConfig SimulationConfig(SimulationConfigFile);
	Simulation::DrivingSimulator(nodes, SimulationConfig);

	return 0;
}