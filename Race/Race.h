#pragma once

#include <iostream>
#include <tchar.h>
#include <urlmon.h>
#include <vector>
#pragma comment (lib, "Urlmon.lib")

HRESULT DownloadFile(wchar_t* URL, wchar_t* OutFile);

class node
{
public:
	double longitude = nan("");				//Grad
	double latitude = nan("");				//Grad
	double elevation = nan("");				//[m]
	double horizontalCurveRadius = nan("");	//[m]
	double verticalCurveRadius = nan("");		//[m] +: Kuppe, -: Senke
	double gradient = nan("");				//[%], evtl. als [°]
	double speedLimit = nan("");				//[m/s]
	double speedIs = nan("");					//[m/s]*/
	double raceTime = nan("");				//[s]
};

//Übergaben als vector<node>()