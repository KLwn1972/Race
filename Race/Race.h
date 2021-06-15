#include <iostream>
#include <tchar.h>
#include <urlmon.h>
#pragma comment (lib, "Urlmon.lib")

HRESULT DownloadFile(wchar_t* URL, wchar_t* OutFile);