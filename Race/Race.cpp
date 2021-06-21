// Race.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include "Race.h"
#include "OpenStreetMap.h"

#include "NASA_ElevationDataDownloader.h"
#include "NASA_ElevationCalculator.h"
#include "NASA_GeoCoordConversion.h"

#include "DatAuf.h"
#include "VectorklassenTest.h"

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
    }


    // **** DATENAUFBEREITUNG Start ********

    DatAuf::CalcDatAuf DatAuf_Nord;             // Muss DatAuf_Nord über new allokiert werden?!
    DatAuf_Nord.nodes = OSM_Nord->nodes;
    delete OSM_Nord;
    DatAuf_Nord.DataProcessing();
    //node node1, node2;
    //DatAuf::GetDistanceMeters(node1, node2);

    // **** DATENAUFBEREITUNG ENDE *********





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