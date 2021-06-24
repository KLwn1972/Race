#pragma once

//#define DEBUG

//////////////////////////////
//Flags aus NASA Projekt --> Siehe Nasa_constant.h fuer Info
//#define CURL_ON  //Uncommend to activate automated CURL download 
#define USE_WINDOWS  //Switch für 7zip Nutzung unter Windows oder Linux (Linux ungetestet mangels Testsystem)
//////////////////////////////

#ifndef NODEDEF
#define NODEDEF

#include <iostream>
#include <iomanip>
#include <tchar.h>
#include <urlmon.h>
#include <vector>
#include <limits>
#include "NASA_src/NASA_constants.h"
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