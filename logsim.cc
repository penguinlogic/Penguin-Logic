#include "logsim.h"
#include "userint.h"
#include "gui.h"
#include <GL/glut.h>
//#include "wx/help.h" // Required for help file
//#include "wx/fs_zip.h"	// Required for help file


#if defined(_WINDOWS) && defined(_DEBUG)
	#include "guicon.h"	
#endif

#define USE_GUI

IMPLEMENT_APP(MyApp)
  
bool MyApp::OnInit()
  // This function is automatically called when the application starts
{
	#if defined(_WINDOWS) && defined(_DEBUG)
		RedirectIOToConsole();					// Required for cout in windows
	#endif

	if (argc != 2) { // check we have one command line argument
    wcout << "Usage:      " << argv[0] << " [filename]" << endl;
    exit(1);
  }

  // Construct the six classes required by the innards of the logic simulator
  nmz = new names();
  netz = new network(nmz);
  dmz = new devices(nmz, netz);
  mmz = new monitor(nmz, netz);
  smz = new scanner(nmz, wxString(argv[1]).mb_str());
cout << "pre parser constructor" << endl;
  pmz = new parser(/*netz, dmz, mmz,*/ smz);
cout << "finished constructing parser" << endl;

  if (pmz->readin ()) { // check the logic file parsed correctly
#ifdef USE_GUI
    // glutInit cannot cope with Unicode command line arguments, so we pass
    // it some fake ASCII ones instead
    char *a[2]={"dummy","dummy"};
	char **tmp1=(char**)a; int tmp2 = 0; glutInit(&tmp2, tmp1);
    // Construct the GUI
    MyFrame *frame = new MyFrame(NULL, wxT("Penguin Logic"), wxDefaultPosition,  wxSize(800, 600), nmz, dmz, mmz, m_helpController, wxDEFAULT_FRAME_STYLE);
	frame->SetBackgroundColour(wxColour(204, 204, 255));
	frame->Show(true); // enter the GUI event loop
#else
    // Construct the text-based interface
    userint umz(nmz, dmz, mmz);
    umz.userinterface();
#endif /* USE_GUI */
  }
  return(false); // exit the application
}


//#include <windows.h>
//#include <iostream>
//#include <fstream>
//#include <conio.h>
//#include <stdio.h>
//
//#ifndef _USE_OLD_OSTREAMS
//	using namespace std;
//#endif
//
//#include "guicon.h"
//#include <crtdbg.h>
//
//int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR lpCmdLine,int nCmdShow)
//{
//	#ifdef _DEBUG
//		RedirectIOToConsole();
//	#endif
//	
//	int iVar;
//	
//	// test stdio
//	fprintf(stdout, "Test output to stdout\n");
//	fprintf(stderr, "Test output to stderr\n");
//	fprintf(stdout, "Enter an integer to test stdin: ");
//	scanf("%d", &iVar);
//	printf("You entered %d\n", iVar);
//
//	//test iostreams
//	cout << "Test output to cout" << endl;
//	cerr << "Test output to cerr" << endl;
//	clog << "Test output to clog" << endl;
//	cout << "Enter an integer to test cin: ";
//	cin >> iVar;
//	cout << "You entered " << iVar << endl;
//
//	#ifndef _USE_OLD_IOSTREAMS
//		// test wide iostreams
//		wcout << L"Test output to wcout" << endl;
//		wcerr << L"Test output to wcerr" << endl;
//		wclog << L"Test output to wclog" << endl;
//		wcout << L"Enter an integer to test wcin: ";
//		wcin >> iVar;
//		wcout << L"You entered " << iVar << endl;
//	#endif
//
//	// test CrtDbg output
//	_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE );
//	_CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDERR );
//	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE );
//	_CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDERR);
//	_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE );
//	_CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDERR);
//	_RPT0(_CRT_WARN, "This is testing _CRT_WARN output\n");
//	_RPT0(_CRT_ERROR, "This is testing _CRT_ERROR output\n");
//	_ASSERT( 0 && "testing _ASSERT" );
//	_ASSERTE( 0 && "testing _ASSERTE" );
//	
//	Sleep(2000);
//	return 0;
//}
////End of File
