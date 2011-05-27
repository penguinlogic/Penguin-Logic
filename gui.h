#ifndef gui_h
#define gui_h

#include <wx/wx.h>
#include <wx/glcanvas.h>
#include <wx/spinctrl.h>
#include <wx/textctrl.h>
#include "names.h"
#include "devices.h"
#include "monitor.h"

enum { 
  MY_SPINCNTRL_ID = wxID_HIGHEST + 1,
  MY_TEXTCTRL_ID,
  RUN_BUTTON_ID,
  CONT_BUTTON_ID,
  SWITCH_BUTTON_ID,
  MONITOR_BUTTON_ID
}; // widget identifiers

class MyGLCanvas;

class MyFrame: public wxFrame
{
	public:
		MyFrame(wxWindow *parent, const wxString& title, const wxPoint& pos, const wxSize& size, 
			names *names_mod = NULL, devices *devices_mod = NULL, monitor *monitor_mod = NULL,
			long style = wxDEFAULT_FRAME_STYLE); // constructor
	private:
			// PRIVATE VARIABLES
		MyGLCanvas *canvas;								// GL drawing area widget to draw traces
		wxSpinCtrl *spin;								// control widget to select the number of cycles
		names *nmz;										// pointer to names class
		devices *dmz;									// pointer to devices class
		monitor *mmz;									// pointer to monitor class
		int cyclescompleted;							// how many simulation cycles have been completed
			// EVENT HANDLERS
		void runnetwork(int ncycles);					// function to run the logic network
		void OnDevelopment(wxCommandEvent& event);		// callback for functions under development
		void OnOpen(wxCommandEvent& event);				// callback for [File | Open] menu item
		void OnSave(wxCommandEvent& event);				// callback for [File | Save] menu item
		void OnExit(wxCommandEvent& event);				// callback for [File | Exit] menu item
		void OnHelpContents(wxCommandEvent& event);				// callback for [Help | View help] menu item
		void OnAbout(wxCommandEvent& event);			// callback for [Help | About] menu item
		void OnRunButton(wxCommandEvent& event);		// callback for push button (run)
		void OnContButton(wxCommandEvent& event);		// callback for push button (cont)
		void OnSwitchButton(wxCommandEvent& event);		// callback for push button (SWITCH)
		void OnMonitorButton(wxCommandEvent& event);	// callback for push button (MONITOR)
		void OnSpin(wxSpinEvent& event);				// callback for spin control
		void OnText(wxCommandEvent& event);				// callback for text entry field
			// EVENT TABLE
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
  void OnScroll(wxSizeEvent& event);   // callback for when canvas is scrolled
  void OnPaint(wxPaintEvent& event); // callback for when canvas is exposed
  void OnMouse(wxMouseEvent& event); // callback for mouse events inside canvas
  DECLARE_EVENT_TABLE()
};
    
#endif /* gui_h */
