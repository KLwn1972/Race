/*
* Team NASA Datendownload SRTM - Petra Stedile, Manuel Marks
* Zweck der Datei: Header zur Definition der notwendigen Konstanten
*/


#pragma once 

#include <string>

/*
* Wichtige Info für Eindung NASA Funktionalität:
* Alle Funktionen in namespace NASA
* Usage Beispiel:
*	double long_stuttgart = 9.206802;
*	double lat_stuttgart = 48.742211;
*	cout << NASA::HGT_ElevationCalculator::getElevationFromSRTM_SIRCdata(long_stuttgart, lat_stuttgart) << endl;
*
* ACHTUNG !!! Anpassen von Dateipfad für Download vor Ausführung notwendig !!!
* 			string nasa_download_zielpfad
*			string nasa_download_zielpfad_win
* ACHTUNG 2 !!! Nutzung auf Linux: Anpassung für Unzip notwendig !!!
* Schalten auf Linux-Kommandos über USE_WINDOS Flag
*
* Falls CURL nicht installiert ist, Lib zunächst Installieren.
* Anleitung: https://stackoverflow.com/questions/53861300/how-do-you-properly-install-libcurl-for-use-in-visual-studio-2017
* Wichtig: Anleitung enthält zwei Optionen, Die Option am Ende der Seite ist die einfachere
* Ausführung ohne CURL:
* In Race.h CURL_ON Flag auskommentieren, hgt für Nürburgring steht in Solution bereit
*
* Usage Beispiele:
*
* Herunterladen aller hgt für Deutschland:
* 	NASA::NASADataFileHandler filehandle;
*	filehandle.downloadElevationDataofGermany_NASA_SIRC();
*
* Beispiel für einen Startpunkt auf Nürburg-Ring
* 	double long_nuerburgringstart = 6.966279;
*	double lat_nuerburgringstart = 50.346094;
*	cout << NASA::HGT_ElevationCalculator::getElevationFromSRTM_SIRCdata(long_nuerburgringstart, lat_nuerburgringstart) << endl;#
*
* Rückgabe:
*	- Höhe in Metern wenn hgt file vorhanden
*	- 0, wenn innerhalb möglicher Daten, aber File nicht vorhanden (Meer),
*	- -32768 falls Anfrage außerhalb NR Quadrant
*/



using namespace std;
//namespace NASA {

//Noch unglücklich Lösung: Generelle Variablen als Klassenkonstanten und Code in header 
//static const string nasa_download_zielpfad = "D:/MtM/M_Projekt2/Git/Race/Load/";
static const string nasa_relative_download_zielpfad = "Race\\Load\\";
static const string nasa_project_compilefolder = "\\Debug";
#ifdef USE_WINDOWS
static const string nasa_download_zielpfad_win = "D:\\MtM\\M_Projekt2\\Git\\Race\\Load\\";
#endif
//static const string nasa_download_zielpfad = "../Load/";
static const string nasa_url_base = "https://e4ftl01.cr.usgs.gov/MEASURES/SRTMGL1.003/2000.02.11/";
static const string nasa_url_addon = ".SRTMGL1.hgt.zip";
static const string nasa_usr_namepwd = "MMarks:MtMProjekt2";
static const int srtm_size = 3601;

//limits in den NASA Maps
static const int longitude_max = 59;
static const int longitude_min = 0;
static const int latitude_max = 179;
static const int latitude_min = 0;
//}