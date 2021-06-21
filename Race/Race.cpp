// Race.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include "Race.h"
#include "OpenStreetMap.h"

#include "NASA_ElevationDataDownloader.h"
#include "NASA_ElevationCalculator.h"
#include "NASA_GeoCoordConversion.h"

using namespace std;

int main()
{
    std::cout << "Hello World!\n";

    string route = "38566";
    OpenStreetMap* OSM_Nord = new OpenStreetMap(route);
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
    FileDownloader testlader;
    testlader.downloadFile("Ha", "Hi");

    return 0;
}