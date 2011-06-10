#include "logsim.h"
#include "userint.h"
#include "gui.h"
#include <GL/glut.h>

#if defined(_WINDOWS) && defined(_DEBUG)
	#include "guicon.h"  
#endif

#define USE_GUI

IMPLEMENT_APP(MyApp)
  
bool MyApp::OnInit()
  // This function is automatically called when the application starts
{
  	
	#if defined(_WINDOWS) && defined(_DEBUG)
		RedirectIOToConsole();          // Required for cout in windows
	#endif

	if (wxString(argv[1]) == wxT("--help")) {
		wcout << "Usage:      " << argv[0] << " [filename]" << endl << endl;
		wcout << "[filename] must be in form defined by EBNF grammar: " << endl << endl;
		wcout << "LogicDefinitionGrammar = { section }" << endl <<
				 "section                = ( `DEVICES' `{' {devrule} `}' ) |" << endl <<
 				 "                         ( `CONNECTIONS' `{' {connrule} `}' ) |" << endl <<
				 "                         ( `MONITOR' `{' {monrule} `}' )" << endl <<
				 "devrule                = device `=' uname `;'" << endl <<
				 "connrule               = uname [ `.' outname ] `>' uname `.' inname `;'" << endl <<
				 "monrule                = uname [ `.' outname ] `;'" << endl <<
				 "uname                  = alphachar { alnumchar }" << endl <<
				 "device                 = ( `CLOCK' `-period' uint ) |" << endl <<
				 "                         ( `SWITCH' `-initialvalue' bool ) |" << endl <<
				 "                         ( `AND' `-numinputs' uint ) |" << endl <<
				 "                         ( `NAND' `-numinputs' uint ) |" << endl <<
				 " 	                 ( `OR' `-numinputs' uint ) |" << endl <<
				 "                         ( `NOR' `-numinputs' uint ) |" << endl <<
				 "                         ( `DTYPE' ) |" << endl <<
				 "                         ( `XOR' )" << endl <<
				 "outname                = `Q' | `QBAR'" << endl <<
				 "inname                 = `I1' | `I2' | `I3' | `I4' |" << endl <<
				 "                         `I5' | `I6' | `I7' | `I8' |" << endl <<
				 "                         `I9' | `I10' | `I11' | `I12' |" << endl <<
 				 "                        `I13' | `I14' | `I15' | `I16' |" << endl <<
				 "                         `DATA' |" << endl <<
				 "                         `SET' |" << endl <<
  				 "                       `CLEAR' |" << endl <<
				 "                         `CLK'" << endl;
		exit(0);
	}

	if (argc != 2) { // check we have one command line argument
		wcout << "Usage:      " << argv[0] << " [filename]" << endl;
		exit(1);
		}

  // Construct the six classes required by the innards of the logic simulator
  nmz = new names();
  netz = new network(nmz);
  dmz = new devices(nmz, netz);
  mmz = new monitor(nmz, netz);
  smz = new scanner(nmz, wxString(argv[1]).mb_str(), filecheck );
 if (!filecheck) exit(1);
  pmz = new parser(netz, dmz, mmz, smz, nmz);

  if (pmz->readin ()) { // check the logic file parsed correctly
#ifdef USE_GUI
    // glutInit cannot cope with Unicode command line arguments, so we pass
    // it some fake ASCII ones instead
	const char *a[2]={"\0", "\0"};
    char **tmp1=(char**)a; int tmp2 = 0; glutInit(&tmp2, tmp1);
    // Construct the GUI
    MyFrame *frame = new MyFrame(NULL, wxT("Logic simulator"), wxDefaultPosition,  wxSize(800, 600), nmz, dmz, mmz, netz, wxString(argv[1]));
	frame->SetBackgroundColour(wxColour(176, 196, 222));
    frame->Show(true);
    return(true); // enter the GUI event loop
#else
    // Construct the text-based interface
    userint umz(nmz, dmz, mmz, netz);
    umz.userinterface();
#endif /* USE_GUI */
  }
#if defined(_WINDOWS) && defined(_DEBUG)
		cout << "Press a letter and then <ENTER> to quit" <<endl;
		char a;
		cin >> a;
#endif

  return(false); // exit the application
}
