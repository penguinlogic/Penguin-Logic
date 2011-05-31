#include <windows.h>
#include <iostream>
#include <fstream>
#include <conio.h>
#include <stdio.h>

#ifndef _USE_OLD_OSTREAMS
	using namespace std;
#endif


#ifndef __GUICON_H__
	#define __GUICON_H__
	#ifdef _DEBUG
		void RedirectIOToConsole();
	#endif
#endif

#include <crtdbg.h>

/* End of File */
