// VerInfoLibTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "VerInfoLibTest.h"
#include "..\VerInfoLib\All.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

void ShowUsage()
{
	_tprintf(_T("Test Application for Version Info Library Denis Zabavchik 2006 \n"));
	_tprintf(_T("USAGE:\n"));
	_tprintf(_T("\tVerInfoLibTest.exe -system\n\tPrints out version numbers of all dlls in Windows system folder\n\n"));
	_tprintf(_T("\tVerInfoLibTest.exe -f <FileName>\n\tPrints out version info string tables of a specified module\n\n"));
	_tprintf(_T("\tVerInfoLibTest.exe -u <FileName> <StringName> \"<Value>\"\n\tModifies the StringName value in the first\n\tString Table in module version information\n\n"));
	_tprintf(_T("EXAMPLES:\n\tVerInfoLibTest.exe -u mydll.dll CompanyName \"New company name\"\n\tVerInfoLibTest.exe -u mydll.dll Comments \"Updated comment\"\n\tVerInfoLibTest.exe -f mydll.dll\n"));
	
}

void ShowSystemFilesVersions()
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
		
	TCHAR szBuf[MAX_PATH];
	::GetSystemDirectory(szBuf, MAX_PATH);
	CString strSystemFolder = szBuf;

	hFind = FindFirstFile(strSystemFolder + "\\*.dll", &FindFileData);
	
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		printf ("Invalid File Handle. Get Last Error reports %d\n", GetLastError ());
	} 
	else 
	{
		do
		{
			CString strModulePath = strSystemFolder + "\\" + FindFileData.cFileName;

			const CVersionInfo vi(strModulePath);
			
			if (vi.IsValid())
			{
				_tprintf(_T("%s\t%s\n"), FindFileData.cFileName, vi["FileVersion"]);
			}
			
		}
		while (FindNextFile(hFind, &FindFileData));
		
		FindClose(hFind);
	}
}

void TestVersionInfoLib(int argc, TCHAR* argv[], TCHAR* envp[])
{
	if (2 == argc)
	{
		if (!_tcscmp(argv[1], _T("-system")))
		{
			ShowSystemFilesVersions();
		}
		else
			ShowUsage();
	}
	else if (3 == argc)
	{
		if (!_tcscmp(argv[1], _T("-f")))
		{
			// 2nd argument is the file path
			CString strFilePath(argv[2]);

			const CVersionInfo vi(strFilePath);
			if (vi.IsValid())
			{
				POSITION posTable = vi.GetStringFileInfo().GetFirstStringTablePosition();
				while (posTable)
				{
					const CStringTable &st = *vi.GetStringFileInfo().GetNextStringTable(posTable);
					
					_tprintf(_T("String table %s\n------------------------------\n"), st.GetKey());
					
					POSITION posString = st.GetFirstStringPosition();
					
					while (posString)
					{
						const CVersionInfoString &vistr = *st.GetNextString(posString);
						_tprintf(_T("%s=%s\n"), vistr.GetKey(), vistr.GetValue());
					}
					_tprintf(_T("------------------------------\n"));
				}
			}
			else
			{
				_tprintf(_T("Failed to get module version information for %s\n"), strFilePath);
			}
		}
		else
			ShowUsage();

	}
	else if (argc > 4)
	{
		if (!_tcscmp(argv[1], _T("-u")))
		{
			// 2nd argument is the file path
			CString strFilePath(argv[2]);
			CVersionInfo vi(strFilePath);

			vi[argv[3]] = argv[4];
			vi.Save();
		}
		else
			ShowUsage();

	}
	else
		ShowUsage();


}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		TestVersionInfoLib(argc, argv, envp);
	}

	return nRetCode;
}


