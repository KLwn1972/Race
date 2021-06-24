/*
* Team NASA Datendownload SRTM - Petra Stedile, Manuel Marks
* Zweck der Datei: Header zur Definition der notwendigen Konstanten
*/


#pragma once 

#include <string>

/*
* Wichtige Info f�r Eindung NASA Funktionalit�t:
* Alle Funktionen in namespace NASA
* Usage Beispiel:
*	double long_stuttgart = 9.206802;
*	double lat_stuttgart = 48.742211;
*	cout << NASA::HGT_ElevationCalculator::getElevationFromSRTM_SIRCdata(long_stuttgart, lat_stuttgart) << endl;
*
* ACHTUNG !!! Anpassen von Dateipfad f�r Download vor Ausf�hrung notwendig !!!
* 			string nasa_download_zielpfad
*			string nasa_download_zielpfad_win
* ACHTUNG 2 !!! Nutzung auf Linux: Anpassung f�r Unzip notwendig !!!
* Schalten auf Linux-Kommandos �ber USE_WINDOS Flag
*
* Falls CURL nicht installiert ist, Lib zun�chst Installieren.
* Anleitung: https://stackoverflow.com/questions/53861300/how-do-you-properly-install-libcurl-for-use-in-visual-studio-2017
* Wichtig: Anleitung enth�lt zwei Optionen, Die Option am Ende der Seite ist die einfachere
* Ausf�hrung ohne CURL:
* In Race.h CURL_ON Flag auskommentieren, hgt f�r N�rburgring steht in Solution bereit
*
* Usage Beispiele:
*
* Herunterladen aller hgt f�r Deutschland:
* 	NASA::NASADataFileHandler filehandle;
*	filehandle.downloadElevationDataofGermany_NASA_SIRC();
*
* Beispiel f�r einen Startpunkt auf N�rburg-Ring
* 	double long_nuerburgringstart = 6.966279;
*	double lat_nuerburgringstart = 50.346094;
*	cout << NASA::HGT_ElevationCalculator::getElevationFromSRTM_SIRCdata(long_nuerburgringstart, lat_nuerburgringstart) << endl;#
*
* R�ckgabe:
*	- H�he in Metern wenn hgt file vorhanden
*	- 0, wenn innerhalb m�glicher Daten, aber File nicht vorhanden (Meer),
*	- -32768 falls Anfrage au�erhalb NR Quadrant
*/



using namespace std;
//namespace NASA {

//Noch ungl�cklich L�sung: Generelle Variablen als Klassenkonstanten und Code in header 
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