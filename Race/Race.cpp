// Race.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include "Race.h"

#include "NASA_ElevationDataDownloader.h"
#include "NASA_ElevationCalculator.h"
#include "NASA_GeoCoordConversion.h"

#include<iostream>
#include "Race.h"

using namespace std;

int main()
{
    std::cout << "Hello World!\n";

    TCHAR URL[] = _T("https://www.openstreetmap.org/api/0.6/relation/38566.json");
    TCHAR OutFile[] = _T("C://Temp/38566.json");

    HRESULT aaa = DownloadFile(URL, OutFile);
    return 0;
}