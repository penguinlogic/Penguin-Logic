#ifndef gui_h
#define gui_h

#include <wx/wx.h>
#include <wx/glcanvas.h>
#include <wx/spinctrl.h>
#include <wx/textctrl.h>
#include <wx/propdlg.h> // Used for the property sheet dialog in Switch value changing
#include "names.h"
#include "devices.h"
#include "monitor.h"
#include "wx/help.h"			// Required for help file
#include "wx/fs_zip.h"			// Required for help file
#include "wx/image.h"			// Required for help file
#include "wx/html/helpfrm.h"	// Required for help file
#include "wx/html/helpctrl.h"	// Required for help file
#include "wx/filesys.h"			// Required for help file
#include "wx/wxprec.h"			// Required for help file
#include <wx/wxhtml.h>			// Required for help file

class DeviceConfigDialog;
class MyGLCanvas;

enum
{ 
  MY_SPINCNTRL_ID = wxID_HIGHEST + 1,
  MY_TEXTCTRL_ID,
  RUN_BUTTON_ID,
  CONT_BUTTON_ID,
  DEVICE_BUTTON_ID,
  MONITOR_BUTTON_ID,
}; // widget identifiers

class MyFrame: public wxFrame
{
public:
	MyFrame(wxWindow *parent, const wxString& title, const wxPoint& pos, const wxSize& size, 
		names *names_mod = NULL, devices *devices_mod = NULL, monitor *monitor_mod = NULL, /*wxHelpController *m_helpController = NULL,*/
		long style = wxDEFAULT_FRAME_STYLE); // constructor
private:
		// PRIVATE VARIABLES
	MyGLCanvas *canvas;								// GL drawing area widget to draw traces
	wxSpinCtrl *spin;								// control widget to select the number of cycles
	names *nmz;										// pointer to names class
	devices *dmz;									// pointer to devices class
	monitor *mmz;									// pointer to monitor class
	wxHtmlHelpController help;						// Create help controller
	int cyclescompleted;							// how many simulation cycles have been completed
		// EVENT HANDLERS
	void runnetwork(int ncycles);					// function to run the logic network
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
	void OnSpin(wxSpinEvent& event);				// callback for spin control
	void OnText(wxCommandEvent& event);				// callback for text entry field
		// EVENT TABLE
	DECLARE_EVENT_TABLE()
};

enum
{
	// 
	ID_SWITCH,
	
	ID_APPLY_SETTINGS_TO,
	ID_BACKGROUND_STYLE,
	ID_FONT_SIZE
};

class DeviceConfigDialog: public wxPropertySheetDialog
{
//DECLARE_CLASS(DeviceConfigDialog)
public:
    DeviceConfigDialog(wxWindow* parent);
	wxPanel* CreateSwitchPropertiesPage(wxWindow* parent);
    wxPanel* CreateClockPropertiesPage(wxWindow* parent);
private:
	DECLARE_EVENT_TABLE()
};
    
class MyGLCanvas: public wxGLCanvas
{
public:
	MyGLCanvas(wxWindow *parent, wxWindowID id = wxID_ANY, monitor* monitor_mod = NULL, names* names_mod = NULL,
			const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0,
			const wxString& name = wxT("MyGLCanvas")); // constructor
	void Render(wxString example_text = wxT(""), int cycles = -1); // function to draw canvas contents
private:
	bool init;                         // has the GL context been initialised?
	int cyclesdisplayed;               // how many simulation cycles have been displayed
	monitor *mmz;                      // pointer to monitor class, used to extract signal traces
	names *nmz;                        // pointer to names class, used to extract signal names
	void InitGL();                     // function to initialise GL context
	void OnSize(wxSizeEvent& event);   // callback for when canvas is resized
	void OnPaint(wxPaintEvent& event); // callback for when canvas is exposed
	void OnMouse(wxMouseEvent& event); // callback for mouse events inside canvas
	DECLARE_EVENT_TABLE()
};
    
#endif /* gui_h */
