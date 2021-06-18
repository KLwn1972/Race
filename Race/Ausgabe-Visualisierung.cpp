#include <iostream>
#include <vector>
#include "race.h"
#include "tinyxml2.h"
#include <string>
#include <time.h>

//#include <fstream> //ofstream d


using namespace std;
using namespace tinyxml2;
string timeConversion(double raceTime, time_t startTime) {
    time_t rawtime = (time_t)raceTime + startTime;
    struct tm timeinfo;
    localtime_s(&timeinfo, &rawtime);

    char buffer[21];
    strftime(buffer, 21, "%FT%TZ", &timeinfo);
    //cout << buffer;
    return buffer;
}

//void output(vector <node> nordschleife) {
//    XMLError fault_flag_gpx, fault_flag_kml;
//    string filename_output;
//    fault_flag_gpx = output_gpx(nordschleife, filename_output);
//    fault_flag_kml = output_kml(nordschleife, filename_output);
//}

void insertElementKML(tinyxml2::XMLDocument& xmlDoc, XMLElement* Element_parent, string elementName, string text) {
    XMLElement* Element_child = xmlDoc.NewElement(elementName.c_str());
    Element_child->SetText(text.c_str());
    Element_parent->InsertEndChild(Element_child);
}



//Erstellung KML Ausgabe
XMLError output_kml(vector <node> nordschleife, string filepath) {
    time_t startTime;
    time(&startTime);
    string timestr;

    tinyxml2::XMLDocument xmlDoc;


    XMLNode* pRoot = xmlDoc.NewElement("kml");
    //pRoot->SetAttribute("itemCount", "ABC");
    //XMLDeclaration * decl = new XMLDeclaration("1.0   ", "UTF-8 ");
    //XMLDeclaration* decl = xmlDoc.NewDeclaration("sfdfsdff");
    xmlDoc.InsertFirstChild(pRoot);
    XMLElement* Element_Document = xmlDoc.NewElement("Document");

    insertElementKML(xmlDoc, Element_Document, "name", "Name");
    insertElementKML(xmlDoc, Element_Document, "description", "description");
    insertElementKML(xmlDoc, Element_Document, "visibility", "1");
    insertElementKML(xmlDoc, Element_Document, "open", "1");

    XMLElement* Element_Style = xmlDoc.NewElement("Style");
    Element_Style->SetAttribute("id", "orange");
    Element_Document->InsertEndChild(Element_Style);

    XMLElement* Element_LineStyle = xmlDoc.NewElement("LineStyle");
    Element_Style->InsertEndChild(Element_LineStyle);
    insertElementKML(xmlDoc, Element_LineStyle, "color", "ff00aaff");
    insertElementKML(xmlDoc, Element_LineStyle, "width", "12");


    XMLElement* Element_Placemark = xmlDoc.NewElement("Placemark");
    Element_Document->InsertEndChild(Element_Placemark);

    insertElementKML(xmlDoc, Element_Placemark, "visibility", "1");
    insertElementKML(xmlDoc, Element_Placemark, "open", "0");
    insertElementKML(xmlDoc, Element_Placemark, "styleUrl", "#orange");
    insertElementKML(xmlDoc, Element_Placemark, "name", "Beispieltext");
    insertElementKML(xmlDoc, Element_Placemark, "description", "Track no. 1");

    XMLElement* Element_LineString = xmlDoc.NewElement("LineString");
    Element_Placemark->InsertEndChild(Element_LineString);

    insertElementKML(xmlDoc, Element_LineString, "extrude", "true");
    insertElementKML(xmlDoc, Element_LineString, "tessellate", "true");
    insertElementKML(xmlDoc, Element_LineString, "altitudeMode", "clampToGround");

    string coordinates = "";
    for (unsigned int i = 0; i < nordschleife.size(); i++) {
        coordinates += to_string(nordschleife[i].longitude) + ","
            + to_string(nordschleife[i].latitude) + ","
            + to_string(nordschleife[i].elevation) + " ";
    }
    insertElementKML(xmlDoc, Element_LineString, "coordinates", coordinates);

    pRoot->InsertEndChild(Element_Document);
    xmlDoc.InsertEndChild(pRoot);

    XMLError eResult = xmlDoc.SaveFile(filepath.c_str());;
    return eResult;
}

void add_node_gpx(tinyxml2::XMLDocument* xmlDoc, node* node_to_add, XMLElement* parent_element, time_t starttime) {
    XMLElement* Element_trkpt = xmlDoc->NewElement("trkpt");
    XMLElement* Element_elevation = xmlDoc->NewElement("ele");
    XMLElement* Element_time = xmlDoc->NewElement("time");
    string timestr;
    Element_trkpt = xmlDoc->NewElement("trkpt");
    Element_trkpt->SetAttribute("lat", node_to_add->latitude);
    Element_trkpt->SetAttribute("lon", node_to_add->longitude);
    Element_elevation = xmlDoc->NewElement("ele");
    Element_elevation->SetText(node_to_add->elevation);
    Element_trkpt->InsertEndChild(Element_elevation);
    Element_time = xmlDoc->NewElement("time");
    Element_time->SetText(timeConversion(node_to_add->raceTime, starttime).c_str());
    Element_trkpt->InsertEndChild(Element_time);
    parent_element->InsertEndChild(Element_trkpt);
}


XMLError output_gpx(vector <node> nordschleife, string filename_output) {
    time_t startTime;
    time(&startTime);
    string timestr;

    tinyxml2::XMLDocument xmlDoc;
    XMLDeclaration* decl = xmlDoc.NewDeclaration("test123");
    //decl->ToDeclaration();
    //xmlDoc->InsertEndChild(decl);
    //XMLDeclaration*decl=xmlDoc.NewDeclaration("null");
    XMLNode* pRoot = xmlDoc.NewElement("gpx");

    //pRoot->SetAttribute("itemCount", "ABC");
    //XMLDeclaration * decl = new XMLDeclaration("1.0   ", "UTF-8 ");
    //XMLDeclaration* decl = xmlDoc.NewDeclaration("sfdfsdff");
    xmlDoc.InsertFirstChild(pRoot);

    XMLElement* Element_trk = xmlDoc.NewElement("trk");
    XMLElement* pElement2 = xmlDoc.NewElement("name");
    pElement2->SetText("Beispieltext");
    Element_trk->InsertEndChild(pElement2);
    pElement2 = xmlDoc.NewElement("number");
    pElement2->SetText("1");
    Element_trk->InsertEndChild(pElement2);
    pElement2 = xmlDoc.NewElement("trkseg");
    XMLElement* Element_trkpt = xmlDoc.NewElement("trkpt");
    XMLElement* Element_elevation = xmlDoc.NewElement("ele");
    XMLElement* Element_time = xmlDoc.NewElement("time");


    //insert Trackpoints to gpx
    for (unsigned int i = 0; i < nordschleife.size(); i++) {
        add_node_gpx(&xmlDoc, &nordschleife[i], pElement2, startTime);
    }
    Element_trk->InsertEndChild(pElement2);
    pRoot->InsertEndChild(Element_trk);
    xmlDoc.InsertEndChild(pRoot);


    XMLError eResult = xmlDoc.SaveFile("racetrack.gpx");
    ////XMLCheckResult(eResult); 

    //cout << "eResult:" <<eResult  ;
    return eResult;
}
