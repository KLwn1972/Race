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
	double longitude;				//Grad
	double latitude;				//Grad
	double elevation;				//[m]
	double horizontalCurveRadius;	//[m]
	double verticalCurveRadius;		//[m] +: Kuppe, -: Senke
	double gradient;				//[%], evtl. als [°]
	double speedLimit;				//[m/s]
	double speedIs;					//[m/s]*/
	double raceTime;				//[s]
};

//Übergaben als vector<node>()