#include "guitest.h"
#include "gui.h"
#include <GL/glut.h>

IMPLEMENT_APP(MyApp)
  
bool MyApp::OnInit()
  // This function is automatically called when the application starts
{
	// Construct the four classes required by the innards of the logic simulator
	nmz = new names();
	netz = new network(nmz);
	dmz = new devices(nmz, netz);
	mmz = new monitor(nmz, netz);

  	// Add Help files
	wxFileSystem::AddHandler(new wxZipFSHandler);
	m_helpController = new wxHelpController;
	m_helpController->Initialize("help.chm");

	// glutInit cannot cope with Unicode command line arguments, so we pass
	// it some fake ASCII ones instead
	char *a[2]={"dummy","dummy"};
	char **tmp1=(char**)a; int tmp2 = 0; glutInit(&tmp2, tmp1);
	// Construct the GUI
	MyFrame *frame = new MyFrame(NULL, wxT("Penguin Logic"), wxDefaultPosition,  wxSize(800, 600), nmz, dmz, mmz, m_helpController, wxDEFAULT_FRAME_STYLE);
	frame->SetBackgroundColour(wxColour(204, 204, 255));
	frame->Show(true);

	return(true); // enter the GUI event loop
}