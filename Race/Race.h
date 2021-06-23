#pragma once

#define DEBUG

#ifndef NODEDEF
#define NODEDEF

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
	double gradient = nan("");				//[%], evtl. als [?]
	double speedLimit = nan("");				//[m/s]
	double speedIs = nan("");					//[m/s]*/
	double raceTime = nan("");				//[s]
};


#endif