#include "Race.h"

using namespace std;

HRESULT DownloadFile(wchar_t* URL, wchar_t* OutFile) {
	//HRESULT result = URLDownloadToFile(NULL, _T("https://www.openstreetmap.org/api/0.6/relation/38566.json"), _T("C://Temp/38566.json"), NULL, NULL);
	HRESULT result = URLDownloadToFile(NULL, URL, OutFile, NULL, NULL);
	switch (result) {
		case S_OK: cout << "Alles OK" << endl; break;
		case E_OUTOFMEMORY: cout << "Nicht genug freier Speicher oder ungültiger Puffer" << endl; break;
		case INET_E_DOWNLOAD_FAILURE: cout << "Fehler beim Download" << endl; break;
	}
	return result;
}
