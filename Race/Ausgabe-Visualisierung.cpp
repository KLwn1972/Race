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
    time_t rawtime;
    if (isnan(raceTime)) rawtime = (time_t)0 + startTime;
    else rawtime = (time_t)raceTime + startTime;
    struct tm timeinfo;
    localtime_s(&timeinfo, &rawtime);

    char buffer[21];
    strftime(buffer, 21, "%FT%TZ", &timeinfo);
    //cout << buffer;
    return buffer;
}

void ausgabe_visualisierung(vector <node>& nordschleife) {
    XMLError fault_flag_gpx, fault_flag_kml;
    string filename_output = "Race.";
    fault_flag_gpx = output_gpx(nordschleife, filename_output + "gpx");
    fault_flag_kml = output_kml(nordschleife, filename_output + "kml");
}

void insertElementKML(tinyxml2::XMLDocument& xmlDoc, XMLElement* Element_parent, string elementName, string text) {
    XMLElement* Element_child = xmlDoc.NewElement(elementName.c_str());
    Element_child->SetText(text.c_str());
    Element_parent->InsertEndChild(Element_child);
}

void insertColorDefinitionKML(tinyxml2::XMLDocument& xmlDoc, XMLElement* Element_parent, string colorName, string colorCode) {
    XMLElement* Element_Style = xmlDoc.NewElement("Style");
    Element_Style->SetAttribute("id", colorName.c_str());
    Element_parent->InsertEndChild(Element_Style);

    XMLElement* Element_LineStyle = xmlDoc.NewElement("LineStyle");
    Element_Style->InsertEndChild(Element_LineStyle);
    insertElementKML(xmlDoc, Element_LineStyle, "color", colorCode.c_str());
    insertElementKML(xmlDoc, Element_LineStyle, "width", "12");
}
string generate_color_code(double act_velo, double min_velo, double max_velo) {
    const int number_categories = 9;
    int  act_category;
    string color_code = "FF000000";
    act_category = (act_velo - min_velo) / ((max_velo - min_velo) / (number_categories - 1));
    switch (act_category) {
    case 0: color_code = "FF0000FF";
        break;
    case 1: color_code = "FF0040FF";
        break;
    case 2: color_code = "FF0080FF";
        break;
    case 3: color_code = "FF00BFFF";
        break;
    case 4: color_code = "FF00FFFF";
        break;
    case 5: color_code = "FF00FFBF";
        break;
    case 6: color_code = "FF00FF80";
        break;
    case 7: color_code = "FF00FF40";
        break;
    case 8: color_code = "FF00FF00";
        break;
    }
    return color_code;
}


//Erstellung KML Ausgabe
XMLError output_kml(vector <node> nordschleife, string filepath) {
    time_t startTime;
    time(&startTime);
    string timestr;

    tinyxml2::XMLDocument xmlDoc;
    xmlDoc.LinkEndChild(xmlDoc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\""));

    XMLNode* pRoot = xmlDoc.NewElement("kml");
    xmlDoc.InsertFirstChild(pRoot);
    XMLElement* Element_Document = xmlDoc.NewElement("Document");

    insertElementKML(xmlDoc, Element_Document, "name", "Name");
    insertElementKML(xmlDoc, Element_Document, "description", "description");
    insertElementKML(xmlDoc, Element_Document, "visibility", "1");
    insertElementKML(xmlDoc, Element_Document, "open", "1");

    insertColorDefinitionKML(xmlDoc, Element_Document, "color0", "ff0000ff");
    insertColorDefinitionKML(xmlDoc, Element_Document, "color1", "ff0040ff");
    insertColorDefinitionKML(xmlDoc, Element_Document, "color2", "ff0080ff");
    insertColorDefinitionKML(xmlDoc, Element_Document, "color3", "ff00bfff");
    insertColorDefinitionKML(xmlDoc, Element_Document, "color4", "ff00ffff");
    insertColorDefinitionKML(xmlDoc, Element_Document, "color5", "ffbfff00");
    insertColorDefinitionKML(xmlDoc, Element_Document, "color6", "ff80ff00");
    insertColorDefinitionKML(xmlDoc, Element_Document, "color7", "ff40ff00");
    insertColorDefinitionKML(xmlDoc, Element_Document, "color8", "ff00ff00");

    for (unsigned int i = 0; i < nordschleife.size() - 1; i++) {
        XMLElement* Element_Placemark = xmlDoc.NewElement("Placemark");
        Element_Document->InsertEndChild(Element_Placemark);

        insertElementKML(xmlDoc, Element_Placemark, "visibility", "1");
        insertElementKML(xmlDoc, Element_Placemark, "open", "0");
        string color = "#color" + to_string(i % 9);  //Funktionsaufruf generate_color_code
        insertElementKML(xmlDoc, Element_Placemark, "styleUrl", color.c_str());
        string description = "Track no. " + to_string(i);
        insertElementKML(xmlDoc, Element_Placemark, "name", description.c_str());
        description += "description";
        insertElementKML(xmlDoc, Element_Placemark, "description", description.c_str());

        XMLElement* Element_LineString = xmlDoc.NewElement("LineString");
        Element_Placemark->InsertEndChild(Element_LineString);

        insertElementKML(xmlDoc, Element_LineString, "extrude", "true");
        insertElementKML(xmlDoc, Element_LineString, "tessellate", "true");
        insertElementKML(xmlDoc, Element_LineString, "altitudeMode", "clampToGround");

        string coordinates = "";
        for (int j = 0; j < 2; j++) {
            coordinates += to_string(nordschleife[i + j].longitude) + ","
                + to_string(nordschleife[i + j].latitude) + ","
                + to_string(nordschleife[i + j].elevation) + " ";
        }
        insertElementKML(xmlDoc, Element_LineString, "coordinates", coordinates);
    }
    pRoot->InsertEndChild(Element_Document);
    xmlDoc.InsertEndChild(pRoot);

    XMLError eResult = xmlDoc.SaveFile(filepath.c_str());
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

XMLError output_gpx(vector <node> nordschleife, string filepath) {
    time_t startTime;
    time(&startTime);
    string timestr;

    tinyxml2::XMLDocument xmlDoc;
    xmlDoc.LinkEndChild(xmlDoc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\""));
    XMLNode* pRoot = xmlDoc.NewElement("gpx");

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


    XMLError eResult = xmlDoc.SaveFile(filepath.c_str());
    return eResult;
}
