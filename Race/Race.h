#pragma once

#define DEBUG

#include <iostream>
#include <tchar.h>
#include <urlmon.h>
#include <vector>
#include "NASA_ElevationCalculator.h"
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
	double horizontalCurveRadius = 150;//nan("");	//[m]
	double verticalCurveRadius = 500;//nan("");		//[m] +: Kuppe, -: Senke
	double gradient = 6;//nan("");				//[%], evtl. als [°]
	double speedLimit = nan("");				//[m/s]
	double speedIs = nan("");					//[m/s]*/
	double raceTime = nan("");				//[s]
	double distanceToNext = nan(""); //m Distance to next node ==> nan = no next
};

//Funktionsdeklaration Gruppe Ausgabe
string timeConversion(double, time_t);
void insertElementKML(tinyxml2::XMLDocument&, XMLElement*, string, string);
XMLError output_kml(vector<node>, string);
void add_node_gpx(tinyxml2::XMLDocument*, node*, XMLElement*, time_t);
XMLError output_gpx(vector<node>, string);

//Übergaben als vector<node>()
vector<node> ExampleTrack();
vector<node> ExampleStraightTrack(double length);