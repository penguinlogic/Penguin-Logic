#ifndef gui_h
#define gui_h

// Include necessary header files
#include <wx/wx.h>
#include <wx/glcanvas.h>
#include <wx/spinctrl.h>
#include <wx/textctrl.h>
#include <wx/propdlg.h> // Used for the property sheet dialog in Switch value changing
#include "names.h"
#include "devices.h"
#include "monitor.h"
#include "network.h"
#include "wx/help.h"			// Required for help file
#include "wx/fs_zip.h"			// Required for help file
#include "wx/image.h"			// Required for help file
#include "wx/html/helpfrm.h"	// Required for help file
#include "wx/html/helpctrl.h"	// Required for help file
#include "wx/filesys.h"			// Required for help file
#include "wx/wxprec.h"			// Required for help file
#include <wx/wxhtml.h>			// Required for help file
#include <GL/glut.h>
#include "wx_icon.xpm"
#include <iostream>

using namespace std;


// Declare classes
class MyFrame;
class DeviceConfigDialog;
class MyGLCanvas;


//----DEFINE DATA STRUCTURES------//
// Enumerated types used as widget identifiers
enum {MY_SPINCNTRL_ID = wxID_HIGHEST + 1,	MY_TEXTCTRL_ID,
	RUN_BUTTON_ID,	CONT_BUTTON_ID,	DEVICE_BUTTON_ID,
	MONITOR_BUTTON_ID,};

// switchProp - used in switch properties page
struct switchProp {wxString Name; wxString Value; int ID;
				wxStaticText* field_box; wxComboBox* value_box;};

// clockProp - used in clock properties page
struct clockProp {wxString Name; wxString Value; int ID;
				wxStaticText* field_box; wxSpinCtrl* value_box;};

// monProp - used in monitor selection box
struct monProp {int DevID;int OutID; wxString DevName;
				wxString OutName; int MonID; bool Selected;};

// Define statics for use as default parameters
static wxArrayString wxEmptyArrayString;
static vector<switchProp> DEFAULT_SWITCH;
static vector<clockProp> DEFAULT_CLOCK;




//----MY FRAME CLASS------//
class MyFrame: public wxFrame
{
public:
		// CONSTRUCTOR
	MyFrame(wxWindow *parent, const wxString& title, const wxPoint& pos, const wxSize& size, 
		names *names_mod = NULL, devices *devices_mod = NULL, monitor *monitor_mod = NULL, 
		network *network_mod = NULL, wxString deffilename = wxEmptyString, long style = wxDEFAULT_FRAME_STYLE); // constructor
private:
		// PRIVATE VARIABLES
	MyGLCanvas *canvas;								// GL drawing area widget to draw traces
	wxSpinCtrl *spin;								// control widget to select the number of cycles
	names *nmz;										// pointer to names class
	devices *dmz;									// pointer to devices class
	monitor *mmz;									// pointer to monitor class
	network *netz;									// pointer to network class
	wxHtmlHelpController help;						// Create help controller
	int cyclescompleted;							// how many simulation cycles have been completed
	wxString defname;								// name of definition files
		// MEMBER FUNCTIONS
	wxString MyFrame::DeviceProps(devlink d);
	void runnetwork(int ncycles);					// function to run the logic network
		// EVENT HANDLERS
	void OnDevelopment(wxCommandEvent& event);		// callback for functions under development
	void OnOpen(wxCommandEvent& event);				// callback for [File | Open] menu item
	void OnSave(wxCommandEvent& event);				// callback for [File | Save] menu item
	void OnExit(wxCommandEvent& event);				// callback for [File | Exit] menu item
	void OnHelpContents(wxCommandEvent& event);		// callback for [Help | View help] menu item
	void OnAbout(wxCommandEvent& event);			// callback for [Help | About] menu item
	void OnRunButton(wxCommandEvent& event);		// callback for push button (run)
	void OnContButton(wxCommandEvent& event);		// callback for push button (cont)
	void OnDeviceButton(wxCommandEvent& event);		// callback for push button (DEVICES)
	void OnMonitorButton(wxCommandEvent& event);	// callback for push button (MONITOR)
		// EVENT TABLE
	DECLARE_EVENT_TABLE()
};



//----GL CANVAS CLASS------//
class MyGLCanvas: public wxGLCanvas
{
public:
		// CONSTRUCTOR
	MyGLCanvas(wxWindow *parent, wxWindowID id = wxID_ANY, monitor* monitor_mod = NULL, names* names_mod = NULL,
			const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0,
			const wxString& name = wxT("MyGLCanvas")); // constructor
		// PUBLIC MEMBER FUNCTIONS
	void Render(int cycles = -1); // function to draw canvas contents
	void GetImage(char* &pixels);
private:
		// PRIVATE VARIABLES
	bool init;                         // has the GL context been initialised?
	int cyclesdisplayed;               // how many simulation cycles have been displayed
	monitor *mmz;                      // pointer to monitor class, used to extract signal traces
	names *nmz;                        // pointer to names class, used to extract signal names
		// PRIVATE MEMBER FUNCTIONS
	void InitGL();                     // function to initialise GL context
	void OnSize(wxSizeEvent& event);   // callback for when canvas is resized
	void OnPaint(wxPaintEvent& event); // callback for when canvas is exposed
	void OnMouse(wxMouseEvent& event); // callback for mouse events inside canvas
		// EVENT TABLE
	DECLARE_EVENT_TABLE()
};



//----DEVICE CONFIG DIALOG BOX CLASS------//
class MyDeviceConfigDialog: public wxPropertySheetDialog
{
public:
		// CONSTRUCTOR
	MyDeviceConfigDialog(wxWindow* parent, const wxString& title, long style, const wxString& name,
						names *names_mod = NULL, devices *devices_mod = NULL, monitor *monitor_mod = NULL, 
						network *network_mod = NULL, wxArrayString switchValue = wxEmptyArrayString,
						vector<switchProp> &switchTable = DEFAULT_SWITCH, vector<clockProp> &clockTable = DEFAULT_CLOCK);
private:
		// PRIVATE VARIABLES
	names *nmz;										// pointer to names class
	devices *dmz;									// pointer to devices class
	monitor *mmz;									// pointer to monitor class
	network *netz;									// pointer to network class
		// MEMBER FUNCTIONS
	wxPanel* CreateSwitchPropertiesPage(wxWindow* parent, wxArrayString switchValue, vector<switchProp> &switchTable);
    wxPanel* CreateClockPropertiesPage(wxWindow* parent, vector<clockProp> &clockTable);
		// EVENT TABLE
	DECLARE_EVENT_TABLE()
};

    
#endif /* gui_h */
