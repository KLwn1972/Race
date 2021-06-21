// Race.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//



#include<stdio.h>
#include "Race.h"
#include "OpenStreetMap.h"



#include "Simulation/Vehicle.h"
#include "Simulation/DataMap2D.h"

using namespace std;

int main()
{
	std::cout << "Hello World!\n";

	string route = "38566";
	OpenStreetMap* OSM_Nord = new OpenStreetMap(route);
	OSM_Nord->waysOffset = 3; // Ignoriere erste 3 Wege (Verbindungsstrasse)
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
	double long_stuttgart = 9.206802;
	double lat_stuttgart = 48.742211;
	ElevationCalculator calc;
	cout.setf(ios::fixed, ios::floatfield);
	cout.precision(6);
	cout << calc.getElevationFromSRTM_SIRCdata(long_stuttgart, lat_stuttgart) << endl;
	cout << GeoCoordConversion::getGrad_From_WGS84Decimal(long_stuttgart) << endl;
	cout << setw(20) <<  GeoCoordConversion::getMin_From_WGS84Decimal(long_stuttgart) << endl;
	cout << setw(20) << GeoCoordConversion::getSeconds_From_WGS84Decimal(long_stuttgart) << endl;
	cout << setw(20) << GeoCoordConversion::getDecimal_From_WGS84GradMinSec(9, 13, 24.4872) << endl;


	//Fahrphysik
	//Simulation::ExampleElectricVehicle();
	vector<double> xdata = vector<double>{ 0,1,2,3,5,6,7 };
	vector<double> ydata = vector<double>{ 0,100,200,300,500,600,700 };
	Simulation::DataMap2D* Datamap = new Simulation::DataMap2D(xdata, ydata);
	cout << Datamap->getY(-1) << "\n";
	cout << Datamap->getY(1.9) << "\n";
	cout << Datamap->getY(800) << "\n";

	return 0;
}