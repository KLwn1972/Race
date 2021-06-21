#pragma once

#include <iostream>
#include <tchar.h>
#include <urlmon.h>
#include <vector>
#include "NASA_ElevationCalculator.h"
#pragma comment (lib, "Urlmon.lib")

class node
{
public:
	double longitude = nan("");				//Grad
	double latitude = nan("");				//Grad
	double elevation = nan("");				//[m]
	double distanceToNext = nan("");
	double horizontalCurveRadius = nan("");	//[m]
	double verticalCurveRadius = nan("");		//[m] +: Kuppe, -: Senke
	double gradient = nan("");				//[%], evtl. als [°]
	double speedLimit = nan("");				//[m/s]
	double speedIs = nan("");					//[m/s]*/
	double raceTime = nan("");				//[s]
};

//Übergaben als vector<node>()