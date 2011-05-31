// The program can't start because MSVCP100.dll is missing from your computer.

#include "gui.h"
#include <GL/glut.h>
#include "wx_icon.xpm"
#include <iostream>

using namespace std;

// MyGLCanvas ////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(MyGLCanvas, wxGLCanvas)
	EVT_SIZE(MyGLCanvas::OnSize)
	EVT_PAINT(MyGLCanvas::OnPaint)
	EVT_MOUSE_EVENTS(MyGLCanvas::OnMouse)
END_EVENT_TABLE()
  
MyGLCanvas::MyGLCanvas(wxWindow *parent, wxWindowID id, monitor* monitor_mod, names* names_mod,
						const wxPoint& pos, const wxSize& size, long style, const wxString& name):
wxGLCanvas(parent, id, pos, size, style, name)
  // Constructor - initialises private variables
{
	mmz = monitor_mod;
	nmz = names_mod;
	init = false;
	cyclesdisplayed = -1;
}

void MyGLCanvas::Render(wxString example_text, int cycles)
  // Draws canvas contents - the following example writes the string "example text" onto the canvas
  // and draws a signal trace. The trace is artificial if the simulator has not yet been run.
  // When the simulator is run, the number of cycles is passed as a parameter and the first monitor
  // trace is displayed.
{
	float y;
	int i;
	asignal s;

	if (cycles >= 0)
		cyclesdisplayed = cycles;

	SetCurrent();
	if (!init)
	{
		InitGL();
		init = true;
	}
	glClear(GL_COLOR_BUFFER_BIT);

	if ((cyclesdisplayed >= 0) && (mmz->moncount() > 0))	// draw the first monitor signal, get trace from monitor class
	{
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_LINE_STRIP);
		for (i=0; i<cyclesdisplayed; i++)
		{
			if (mmz->getsignaltrace(0, i, s))
			{
				if (s==low)
					y = 10.0;
				if (s==high)
					y = 30.0;
				glVertex2f(20*i+10.0, y); 
				glVertex2f(20*i+30.0, y);
			}
		}
		glEnd();
	}

	else	// draw an artificial trace
	{
		glColor3f(0.0, 1.0, 0.0);
		glBegin(GL_LINE_STRIP);
		for (i=0; i<5; i++)
		{
			if (i%2)
				y = 10.0;
			else
				y = 30.0;
			glVertex2f(20*i+10.0, y); 
			glVertex2f(20*i+30.0, y);
		}
		glEnd();
	}

	// Example of how to use GLUT to draw text on the canvas
	glColor3f(0.0, 0.0, 1.0);
	glRasterPos2f(10, 100);
	for (unsigned int i = 0; i < example_text.Len(); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, example_text[i]);

	// We've been drawing to the back buffer, flush the graphics pipeline and swap the back buffer to the front
	glFlush();
	SwapBuffers();
}

void MyGLCanvas::InitGL()	// Function to initialise the GL context
{
	int w, h;

	GetClientSize(&w, &h);
	SetCurrent();
	glDrawBuffer(GL_BACK);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glViewport(0, 0, (GLint) w, (GLint) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h, -1, 1); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void MyGLCanvas::OnPaint(wxPaintEvent& event)	// Callback function for when the canvas is exposed
{
	int w, h;
	wxString text;

	wxPaintDC dc(this); // required for correct refreshing under MS windows
	GetClientSize(&w, &h);
	text.Printf(wxT("Canvas redrawn by OnPaint callback, canvas size is %d by %d"), w, h);
	Render(text);
}

void MyGLCanvas::OnSize(wxSizeEvent& event)	// Callback function for when the canvas is resized
{
	wxGLCanvas::OnSize(event); // required on some platforms
	init = false;
	Refresh(); // required by some buggy nvidia graphics drivers,
	Update();  // harmless on other platforms!
}

void MyGLCanvas::OnMouse(wxMouseEvent& event)	// Callback function for mouse events inside the GL canvas
{
	wxString text;
	int w, h;;

	GetClientSize(&w, &h);
	if (event.ButtonDown())
		text.Printf(wxT("Mouse button %d pressed at %d %d"), event.GetButton(), event.m_x, h-event.m_y);
	if (event.ButtonUp())
		text.Printf(wxT("Mouse button %d released at %d %d"), event.GetButton(), event.m_x, h-event.m_y);
	if (event.Dragging())
		text.Printf(wxT("Mouse dragged to %d %d"), event.m_x, h-event.m_y);
	if (event.Leaving())
		text.Printf(wxT("Mouse left window at %d %d"), event.m_x, h-event.m_y);

	if (event.ButtonDown() || event.ButtonUp() || event.Dragging() || event.Leaving())
		Render(text);
}

// MyFrame ///////////////////////////////////////////////////////////////////////////////////////


BEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_MENU(wxID_OPEN, MyFrame::OnOpen)
	EVT_MENU(wxID_SAVE, MyFrame::OnSave)
	EVT_MENU(wxID_EXIT, MyFrame::OnExit)
	EVT_MENU(wxID_HELP_CONTENTS, MyFrame::OnHelpContents)
	EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
	EVT_BUTTON(RUN_BUTTON_ID, MyFrame::OnRunButton)
	EVT_BUTTON(CONT_BUTTON_ID, MyFrame::OnContButton)
	EVT_BUTTON(SWITCH_BUTTON_ID, MyFrame::OnSwitchButton)
	EVT_BUTTON(MONITOR_BUTTON_ID, MyFrame::OnMonitorButton)
	EVT_SPINCTRL(MY_SPINCNTRL_ID, MyFrame::OnSpin)
	EVT_TEXT_ENTER(MY_TEXTCTRL_ID, MyFrame::OnText)
END_EVENT_TABLE()

MyFrame::MyFrame(wxWindow *parent, const wxString& title, const wxPoint& pos, const wxSize& size,
		 names *names_mod, devices *devices_mod, monitor *monitor_mod, wxHelpController *m_helpController, long style):
  wxFrame(parent, wxID_ANY, title, pos, size, style)
  // Constructor - initialises pointers to names, devices and monitor classes, lays out widgets
  // using sizers
{
	SetIcon(wxIcon(wx_icon_xpm));
	nmz = names_mod;
	dmz = devices_mod;
	mmz = monitor_mod;
	hpc = m_helpController;
	if (nmz == NULL || dmz == NULL || mmz == NULL || hpc == NULL)
	{
		cout << "Cannot operate GUI without names, devices and monitor classes (or help files)" << endl;
		exit(1);
	}

	// Define File menu
	wxMenu *fileMenu = new wxMenu;
	fileMenu->Append(wxID_OPEN, wxT("&Open circuit definition file"));
	fileMenu->Append(wxID_SAVE, wxT("&Save waveforms as picture"));
	fileMenu->Append(wxID_EXIT, wxT("&Exit"));

	// Define Help menu
	wxMenu *helpMenu = new wxMenu;
	helpMenu->Append(wxID_HELP_CONTENTS, wxT("&View Help"));
	helpMenu->Append(wxID_ABOUT, wxT("&About"));

	// Define Menu bar
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(fileMenu, wxT("&File"));
	menuBar->Append(helpMenu, wxT("&Help"));
	SetMenuBar(menuBar);

	// Create Status bar
	CreateStatusBar(3);
	SetStatusText(wxT("Ready"), 0);

	// Main window layout
	wxBoxSizer *topsizer = new wxBoxSizer(wxHORIZONTAL);
		// Left panel
		wxBoxSizer *left_sizer = new wxBoxSizer(wxVERTICAL);
			// Canvas
			wxScrolledWindow *sw = new wxScrolledWindow(this, wxID_ANY, wxPoint(0, 0), wxSize(400, 400), wxVSCROLL|wxHSCROLL);
				const int s_inc = 40;		// Scroll increment
				const int cx_size = 1400;	// Canvas size
				const int cy_size = 1000;	// Canvas size
				sw->SetScrollbars(s_inc, s_inc, cx_size/s_inc, cy_size/s_inc);
				canvas = new MyGLCanvas(sw, wxID_ANY, monitor_mod, names_mod, wxDefaultPosition, wxSize(cx_size,cy_size));
			left_sizer->Add(sw, 1, wxEXPAND | wxALL, 10);
			// Button panel
			wxFlexGridSizer *button_sizer = new wxFlexGridSizer(2,0,0,0);
					// Run button
				button_sizer->Add(new wxButton(this, RUN_BUTTON_ID, wxT("Run")), 0, wxALL, 10);
					// SWITCH button
				button_sizer->Add(new wxButton(this, SWITCH_BUTTON_ID, wxT("Change initial switch value")), 0, wxALL, 10);
					// Spinner (label)
				button_sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Cycles:")), 0, wxALIGN_RIGHT|wxTOP|wxLEFT, 10);
					// Spinner (device)
				spin = new wxSpinCtrl(this, MY_SPINCNTRL_ID, wxString(wxT("10")));
				button_sizer->Add(spin, 0 , wxALL, 10);
					// Continue button
				button_sizer->Add(new wxButton(this, CONT_BUTTON_ID, wxT("Continue")), 0, wxALL, 10);
					// MONITOR button
				button_sizer->Add(new wxButton(this, MONITOR_BUTTON_ID, wxT("Change monitor points")), 0, wxALL, 10);
					// Free text field (label)
				button_sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Text:")), 0, wxALIGN_RIGHT|wxTOP|wxLEFT, 10);
					// Free text field (device)
				button_sizer->Add(new wxTextCtrl(this, MY_TEXTCTRL_ID, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER), 0 , wxALL, 10);
			left_sizer->Add(button_sizer, 0, wxEXPAND | wxALL, 10);
		//Right panel
		wxBoxSizer *right_sizer = new wxBoxSizer(wxVERTICAL);
			// Button2 panel
			wxBoxSizer *button2_sizer = new wxBoxSizer(wxVERTICAL);
				// Buttons
			button2_sizer->Add(new wxButton(this, RUN_BUTTON_ID, wxT("Run")), 0, wxALL, 10);
			button2_sizer->Add(new wxButton(this, CONT_BUTTON_ID, wxT("Continue")), 0, wxALL, 10);
			button2_sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Cycles")), 0, wxTOP|wxLEFT|wxRIGHT, 10);
				// Spinner
			spin = new wxSpinCtrl(this, MY_SPINCNTRL_ID, wxString(wxT("10")));
			button2_sizer->Add(spin, 0 , wxALL, 10);
		right_sizer->Add(button2_sizer, 0, wxALIGN_CENTER);
	topsizer->Add(left_sizer, 1, wxEXPAND | wxALL, 10);
	topsizer->Add(right_sizer, 0, wxEXPAND | wxALL, 10);

	SetSizeHints(700, 400);
	SetSizer(topsizer);
}

void MyFrame::OnDevelopment(wxCommandEvent &event)
  // Produces a dialog saying that feature is under development and unavailable
{
	wxMessageDialog development(this, wxT("We apologise, but this feature is currently under development and not yet available."), wxT("Error: Feature unavailable"), wxICON_INFORMATION | wxOK);
	development.ShowModal();                  
}

void MyFrame::OnOpen(wxCommandEvent &event)
  // Callback for the [File | Open circuit definition file] menu item
{
	MyFrame::OnDevelopment(event);		// Placeholder until feature is developed
	/*--------------- Insert code here-----------------------*/
}

void MyFrame::OnSave(wxCommandEvent &event)
  // Callback for the [File | Save waveforms as picture] menu item
{
	MyFrame::OnDevelopment(event);		// Placeholder until feature is developed
	/*--------------- Insert code here-----------------------*/               
}

void MyFrame::OnExit(wxCommandEvent &event)
  // Callback for the [File | Exit] menu item
{
  Close(true);
}

void MyFrame::OnHelpContents(wxCommandEvent &event)
  // Callback for the [Help | View Help] menu item
{
	if (hpc->LoadFile() == false)
	{
		wxMessageDialog development(this, wxT("Help file cannot be found"), wxT("Error: Feature unavailable"), wxICON_INFORMATION | wxOK);
		development.ShowModal();    
	}
	else
	hpc->DisplayContents();
//  wxMessageDialog help(this, wxT("<Insert help here>"), wxT("Penguin Logic Help"), wxICON_INFORMATION | wxOK);
// help.ShowModal();                  
}

void MyFrame::OnAbout(wxCommandEvent &event)
  // Callback for the [Help | About] menu item
{
	wxMessageDialog about(this, wxT("Penguin Logic is a program to simulate simple logic circuits. \n\n Andy Marshall, James Robinson, Ollie Lambert\nMay 2011"), wxT("About Penguin Logic"), wxICON_INFORMATION | wxOK);
	about.ShowModal();                  
}

void MyFrame::OnRunButton(wxCommandEvent &event)
  // Callback for the push button
{
    int n, ncycles;
    cyclescompleted = 0;
    mmz->resetmonitor ();
    runnetwork(spin->GetValue());
	canvas->Render(wxT("Run button pressed"), cyclescompleted);
	/*
	rdnumber (ncycles, 1, maxcycles);
    if (cmdok)
	{
      mmz->resetmonitor ();
      cout << "Running for " << ncycles << " cycles" << endl;
      runnetwork(ncycles);
	}*/
}

void MyFrame::OnContButton(wxCommandEvent &event)
  // Callback for the push button
{
    int ncycles;
    mmz->resetmonitor ();
    runnetwork(spin->GetValue());
	canvas->Render(wxT("Continue button pressed"), cyclescompleted);
	/*
	if (cmdok)
	{
      if (cyclescompleted > 0)
	  {
        if ((ncycles + cyclescompleted) > maxcycles)
	      ncycles = maxcycles - cyclescompleted;
        cout << "Continuing for " << ncycles << " cycles" << endl;
        runnetwork (ncycles);
      }
	  else
        cout << "Error: nothing to continue!" << endl;
	}*/
}

void MyFrame::OnSwitchButton(wxCommandEvent &event)
  // Callback for the SWITCH button
{
	//MyFrame::OnDevelopment(event);		// Placeholder until feature is developed
	/*
	bool MyPropertySheetDialog::Create(...)
    {
        if (!wxPropertySheetDialog::Create(...))
            return false;

        CreateButtons(wxOK|wxCANCEL|wxHELP);

        // Add page
        wxPanel* panel = new wxPanel(GetBookCtrl(), ...);
        GetBookCtrl()->AddPage(panel, "General");

        LayoutDialog();
        return true;
    }
	
	wxString SwTit = wxT("Change initial switch value");
	wxPropertySheetDialog switches(this, wxID_ANY, SwTit);
	switches.Create(
	switches.CreateButtons(wxOK|wxCANCEL);
	wxPanel* panel = new wxPanel(switches.GetBookCtrl(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, wxT("My Panel"));
    switches.GetBookCtrl()->AddPage(panel, "General");
	switches.LayoutDialog();*/
}

void MyFrame::OnMonitorButton(wxCommandEvent &event)
  // Callback for the MONITOR BUTTON
{
	//  Add all potential monitor points (i.e. all outputs)
	wxArrayString choices;
	choices.Add(wxT("monitor1"));
	choices.Add(wxT("monitor2"));

	// Choose monitor points which are already selected
	wxArrayInt selections;
	selections.Empty();
	
	// Create dialog box
	wxString MonMsg = wxT("Please select the required monitor points and select OK");
	wxString MonCap = wxT("Change monitor points");
	wxMultiChoiceDialog monitor(this, MonMsg, MonCap, choices);
	monitor.SetSelections(selections);
	
	if(monitor.ShowModal() == wxID_OK)
	{
		monitor.GetSelections(); // Sets the selections which are required
	};
}

void MyFrame::OnSpin(wxSpinEvent &event)
  // Callback for the spin control
{
  wxString text;

  text.Printf(wxT("New spinctrl value %d"), event.GetPosition());
  canvas->Render(text);
}

void MyFrame::OnText(wxCommandEvent &event)
  // Callback for the text entry field
{
  wxString text;

  text.Printf(wxT("New text entered %s"), event.GetString().c_str());
  canvas->Render(text);
}

void MyFrame::runnetwork(int ncycles)
  // Function to run the network, derived from corresponding function in userint.cc
{
  bool ok = true;
  int n = ncycles;

  while ((n > 0) && ok) {
    dmz->executedevices (ok);
    if (ok) {
      n--;
      mmz->recordsignals ();
    } else
      cout << "Error: network is oscillating" << endl;
  }
  if (ok) cyclescompleted = cyclescompleted + ncycles;
  else cyclescompleted = 0;
}

//void MonitorFrame::OnOkButton(wxCommandEvent &event)
//{
//}
//
//void MonitorFrame::OnAddButton(wxCommandEvent &event);
//void OnAddButton(wxCommandEvent &event);
//void OnRemoveButton(wxCommandEvent &event);
//void OnMoveUpButton(wxCommandEvent &event);
//void OnMoveDownButton(wxCommandEvent &event);
//void OnOkButton(wxCommandEvent &event);
//void OnCancelButton(wxCommandEvent &event);

// Old code for monitor dialog
//{
//		wxFrame *MonitorFrame = new wxFrame(this, wxID_ANY, wxT("Change monitor points"), wxDefaultPosition, wxSize(300,200), wxDEFAULT_FRAME_STYLE);
//	MonitorFrame->SetBackgroundColour(wxColour(204, 204, 255));
//	MonitorFrame->Show(true);
//	wxBoxSizer *monsizer = new wxBoxSizer(wxVERTICAL);
//		wxFlexGridSizer *gridsizer = new wxFlexGridSizer(3,10,10);
//			gridsizer->Add(new wxStaticText(MonitorFrame, wxID_ANY, wxT("Available monitors")), 1, wxEXPAND|wxALL, 10);
//			gridsizer->Add(new wxStaticText(MonitorFrame, wxID_ANY, wxT("Options")), 1, wxEXPAND|wxALL, 10);
//			gridsizer->Add(new wxStaticText(MonitorFrame, wxID_ANY, wxT("Active monitors")), 1, wxEXPAND|wxALL, 10);
//			gridsizer->Add(new wxStaticText(MonitorFrame, wxID_ANY, wxT("Available monitors")), 1, wxEXPAND|wxALL, 10);
//			gridsizer->Add(new wxStaticText(MonitorFrame, wxID_ANY, wxT("Options")), 1, wxEXPAND|wxALL, 10);
//			gridsizer->Add(new wxStaticText(MonitorFrame, wxID_ANY, wxT("Active monitors")), 1, wxEXPAND|wxALL, 10);
//			gridsizer->Add(new wxStaticText(MonitorFrame, wxID_ANY, wxT("Available monitors")), 1, wxEXPAND|wxALL, 10);
//			gridsizer->Add(new wxStaticText(MonitorFrame, wxID_ANY, wxT("Options")), 1, wxEXPAND|wxALL, 10);
//			gridsizer->Add(new wxStaticText(MonitorFrame, wxID_ANY, wxT("Active monitors")), 1, wxEXPAND|wxALL, 10);
//		monsizer->Add(gridsizer, 1, wxEXPAND|wxALL, 10);
//		wxBoxSizer *OkCancelSizer = new wxBoxSizer(wxHORIZONTAL);
//			OkCancelSizer->Add(new wxButton(MonitorFrame, OK_BUTTON_ID, wxT(OK)), 1, wxEXPAND|wxALL, 10);
//			OkCancelSizer->Add(new wxButton(wxCANCEL), 1, wxEXPAND|wxALL, 10);
//		monsizer->Add(OkCancelSizer, 1, wxEXPAND|wxALL, 10);
//	SetSizeHints(300, 200);
//	SetSizer(monsizer);
//}