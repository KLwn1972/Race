#pragma once

#define DEBUG

#include <iostream>
#include <iomanip>
#include <tchar.h>
#include <urlmon.h>
#include <vector>
#include "NASA_src/NASA_DataFileHandler.h"
#include "NASA_src/NASA_ElevationCalculator.h"
#include "NASA_src/NASA_GeoCoordConversion.h"
#include "tinyxml2.h"
#include <string>
#pragma comment (lib, "Urlmon.lib")

using namespace std;
using namespace tinyxml2;

class node
{
public:
	string id = "";
	double longitude = nan("");				//Grad
	double latitude = nan("");				//Grad
	double elevation = nan("");				//[m]

	double distanceToNext = nan(""); //m Distance to next node ==> nan = no next

	double horizontalCurveRadius = nan("");	//[m]
	double verticalCurveRadius = nan("");		//[m] +: Kuppe, -: Senke
	double gradient = nan("");				//[%], evtl. als [°]
	double speedLimit = nan("");				//[m/s]
	double speedIs = nan("");					//[m/s]*/
	double raceTime = nan("");				//[s]
};

//Funktionsdeklaration Gruppe Ausgabe
void ausgabe_visualisierung(vector <node>&);
string timeConversion(double, time_t);
void insertElementKML(tinyxml2::XMLDocument&, XMLElement*, string, string);
void insertColorDefinitionKML(tinyxml2::XMLDocument&, XMLElement*, string, string);
string generate_color_code(double, double, double);
XMLError output_kml(vector<node>, string);
void add_node_gpx(tinyxml2::XMLDocument*, node*, XMLElement*, time_t);
XMLError output_gpx(vector<node>, string);

//Übergaben als vector<node>()
vector<node> ExampleTrack();
vector<node> ExampleStraightTrack(double length);