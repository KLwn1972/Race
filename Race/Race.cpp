// Race.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include<stdio.h>
#include "Race.h"
#include "OpenStreetMap.h"

#include "Simulation/Vehicle.h"
#include "Simulation/DataMap2D.h"
#include "Simulation/ImportSimulationConfig.h"
#include "Simulation/DrivingSimulator.h"
#include "Simulation/MockSimulationConfig.h"

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
		ausgabe_visualisierung(nodes);
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
	//TODO: Build Testtrack;
	auto track = vector<node>();
	////Fahrphysik
	////Simulation::ExampleElectricVehicle();
	////TODO: Build Testtrack;
	auto track = ExampleStraightTrack(0);
	string SimulationConfigFile = "SimulationConfig.json";
	auto SimulationConfig = Simulation::MockSimulationConfig();
	auto Drivingsim = Simulation::DrivingSimulator(track, SimulationConfig);
	vector<node> result = Drivingsim.RunSimulation();

	vector<double> xdata = vector<double>{ 0,1,2,3,5,6,7 };
	vector<double> ydata = vector<double>{ 0,100,200,300,500,600,700 };
	Simulation::DataMap2D* Datamap = new Simulation::DataMap2D(xdata, ydata);
	cout << Datamap->getY(-1) << "\n";
	cout << Datamap->getY(1.9) << "\n";
	cout << Datamap->getY(800) << "\n";

	return 0;
}

vector<node> ExampleTrack()
{
	//Create a sample nürburgring with (bad) linear interpolation
	vector<double> longitude = { 6.945215, 6.971891, 6.994314, 6.995172, 6.992038, 6.985426,6.989161, 6.980189, 6.96349, 6.9607, 6.950525, 6.9495, 6.938555, 6.933661, 6.93761, 6.931342, 6.928638, 6.91989, 6.927059, 6.925857, 6.933327, 6.944404, 6.945215 };
	vector<double> latitude = { 50.33409, 50.34831, 50.360358, 50.376866, 50.373581, 50.371774, 50.374539, 50.372267, 50.375361, 50.380836, 50.378399, 50.376561, 50.37771, 50.374562, 50.371769, 50.367115, 50.362049, 50.358109, 50.350138, 50.34581, 50.341153, 50.337241, 50.33409 };
	vector<double> elevation = { 500, 333.3333333, 222.2222222, 148.1481481, 98.7654321, 65.8436214, 98.7654321, 148.1481481, 222.2222222, 333.3333333, 500, 750, 500, 333.3333333, 222.2222222, 148.1481481, 98.7654321, 65.8436214, 98.7654321, 148.1481481, 222.2222222, 333.3333333, 500 };

	//TODO: Create vector from this
	return vector<node>();
}

vector<node> ExampleStraightTrack(double length)
{
	double startlong = 6.945215;
	double startlat = 50.33409;
	double startelevation = 500;

	double endlong = 6.971891;
	double endlat = 50.34831;
	double endelevation = 500;

	double distance = 3245.0;

	auto result = vector<node>();
	double numberOfSteps = 1000;
	double stepWidth = distance / numberOfSteps;
	for (int i = 0; i <= 1000; i++)
	{
		auto newnode = node();
		newnode.distanceToNext = distance / 1000;
		newnode.elevation = Simulation::interpolateValues(0, startelevation, distance, endelevation, i * stepWidth);
		newnode.latitude = Simulation::interpolateValues(0, startlat, distance, endlat, i * stepWidth);
		newnode.longitude = Simulation::interpolateValues(0, startlong, distance, endlong, i * stepWidth);
		newnode.gradient = ((endelevation - startelevation) / distance) * 100;
		newnode.speedLimit = 200 * Simulation::KMH2MS;
		newnode.horizontalCurveRadius = 0;
		newnode.verticalCurveRadius = 0;
		newnode.id = i;
		result.push_back(newnode);
	}
	return result;
}