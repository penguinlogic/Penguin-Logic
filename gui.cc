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
	float y=0;
	//int i;
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

	const int CANVAS_HEIGHT=1000;
	const int CANVAS_WIDTH =1400;
	const int SIG_LOW = 10;
	const int SIG_HIGH = 30;
	const int SEPARATION = 50;
	const int INDENT = 100;
	const int LENGTH = 20;


	if ((cyclesdisplayed >= 0) && (mmz->moncount() > 0))	// draw the first monitor signal, get trace from monitor class
	{
		for(int m=mmz->moncount()-1; m>=0; m--)
		{
			// Get name of monitor
			name n1, n2;
			mmz->getmonname(m,n1, n2);
			wxString mon_name = wxString(nmz->getname(n1));
			if(n2 != -1)	// If device has more than 1 output, append output name
			{
				mon_name.Append(".");
				mon_name.Append(nmz->getname(n2));
			}

			// Display name of monitor
			glColor3f(0.0, 0.0, 1.0);
			glRasterPos2f(5, CANVAS_HEIGHT-(m+1)*SEPARATION + SEPARATION/2);
			for (unsigned int i = 0; i < mon_name.Len(); i++) // Display each character in turn
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, mon_name[i]);
			
			// Plot waveform for monitor
			glColor3f(1.0, 0.0, 0.0);
			glBegin(GL_LINE_STRIP);
			for (int i=0; i<cyclesdisplayed; i++)
			{
				if (mmz->getsignaltrace(m, i, s))
				{
					if (s==low)
						y = CANVAS_HEIGHT-(m+1)*SEPARATION + SIG_LOW;
					else if (s==high)
						y = CANVAS_HEIGHT-(m+1)*SEPARATION + SIG_HIGH;
					else
						cout << "signal neither high nor low" << endl;
					glVertex2f(LENGTH*i+INDENT, y); 
					glVertex2f(LENGTH*i+LENGTH+INDENT, y);
				}
				else
					cout << "Couldn't get signal trace" << endl;
			}
			glEnd();
		}
	}

	else	// draw an artificial trace
	{
		glColor3f(0.0, 1.0, 0.0);
		glBegin(GL_LINE_STRIP);
		for (int i=0; i<5; i++)
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
	EVT_BUTTON(DEVICE_BUTTON_ID, MyFrame::OnDeviceButton)
	EVT_BUTTON(MONITOR_BUTTON_ID, MyFrame::OnMonitorButton)
	EVT_SPINCTRL(MY_SPINCNTRL_ID, MyFrame::OnSpin)
	EVT_TEXT_ENTER(MY_TEXTCTRL_ID, MyFrame::OnText)
END_EVENT_TABLE()

MyFrame::MyFrame(wxWindow *parent, const wxString& title, const wxPoint& pos, const wxSize& size,
		 names *names_mod, devices *devices_mod, monitor *monitor_mod, network *network_mod, long style):
  wxFrame(parent, wxID_ANY, title, pos, size, style)
  // Constructor - initialises pointers to names, devices and monitor classes, lays out widgets
  // using sizers
{
	SetIcon(wxIcon(wx_icon_xpm));
	nmz = names_mod;
	dmz = devices_mod;
	mmz = monitor_mod;
	netz = network_mod;
	if (nmz == NULL || dmz == NULL || mmz == NULL)
	{
		cout << "Cannot operate GUI without names, devices and monitor classes" << endl;
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

	// Create Help file
	wxInitAllImageHandlers();
	wxFileSystem::AddHandler(new wxZipFSHandler);
	help.UseConfig(wxConfig::Get());
    bool ret;
    help.SetTempDir(wxT("."));
	#ifdef _WINDOWS
		wxString h_filename = wxT("C:/Users/Oliver Lambert/Documents/Cambridge/Project/02_Software/Penguin-Logic/help.htb");
	#else
		wxString h_filename = wxT("help.htb");
	#endif
	ret = help.AddBook(wxFileName(h_filename, wxPATH_NATIVE));
    if (! ret)
        wxMessageBox(wxT("Failed adding help file (help.htb) - has this file been moved or deleted?"));

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
				button_sizer->Add(new wxButton(this, RUN_BUTTON_ID, wxT("Run")), 0, wxALIGN_CENTER_VERTICAL|wxALL, 10);
					// Spinner (label)
				button_sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Cycles:")), 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxLEFT, 10);
					// Spinner (device)
					spin = new wxSpinCtrl(this, MY_SPINCNTRL_ID, wxString(wxT("10")));
				button_sizer->Add(spin, 0 , wxALIGN_CENTER_VERTICAL|wxALL, 10);
					// DEVICE button
				button_sizer->Add(new wxButton(this, DEVICE_BUTTON_ID, wxT("Change device properties")), 0, wxALIGN_CENTER_VERTICAL|wxALL, 10);
					// Continue button
				button_sizer->Add(new wxButton(this, CONT_BUTTON_ID, wxT("Continue")), 0, wxALIGN_CENTER_VERTICAL|wxALL, 10);
					// Static text (placeholder)
				button_sizer->Add(new wxStaticText(this, wxID_ANY, wxT("")), 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxLEFT, 10);
					// Static text (placeholder)
				button_sizer->Add(new wxStaticText(this, wxID_ANY, wxT("")), 0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxLEFT, 10);
					// MONITOR button
				button_sizer->Add(new wxButton(this, MONITOR_BUTTON_ID, wxT("Change monitor points")), 0, wxALIGN_CENTER_VERTICAL|wxALL, 10);
			left_sizer->Add(button_sizer, 0, wxEXPAND | wxALL, 10);
	topsizer->Add(left_sizer, 1, wxEXPAND | wxALL, 10);
		//Right panel
		wxBoxSizer *right_sizer = new wxBoxSizer(wxVERTICAL);
			wxScrolledWindow *sp = new wxScrolledWindow(this, wxID_ANY, wxPoint(-1, -1), wxSize(-1, -1), wxVSCROLL|wxHSCROLL)/*this, wxID_ANY, wxDefaultPosition, wxSize(100, 600), wxVSCROLL|wxHSCROLL)*/;
				const int p_inc = 40;		// Scroll increment
				const int px_size = 400;	// Canvas size
				const int py_size = 1000;	// Canvas size
				sp->SetScrollbars(p_inc, p_inc, px_size/p_inc, py_size/p_inc);
				wxPanel* panel = new wxPanel(sp, wxID_ANY);
				// Button2 panel
					wxBoxSizer *info_sizer = new wxBoxSizer(wxVERTICAL);
						// Buttons
						wxBoxSizer *deffile_sizer = new wxBoxSizer(wxHORIZONTAL);
							wxStaticText* label = new wxStaticText(panel, wxID_ANY, wxT("Definition file name:"));
						deffile_sizer->Add(label, 1,wxRIGHT|wxALIGN_CENTER_VERTICAL ,5);
							wxTextCtrl* value = new wxTextCtrl(panel, MY_TEXTCTRL_ID, wxT("grammar_eg1.txt"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
						deffile_sizer->Add(value, 1, wxALIGN_CENTER_VERTICAL ,0);
					info_sizer->Add(deffile_sizer, 0, wxEXPAND|wxALL,5);
					// Insert tree structure here
				panel->SetSizer(info_sizer);
					info_sizer->Fit(panel);
		right_sizer->Add(sp, 1, wxALL, 10);
	topsizer->Add(right_sizer, 0, wxEXPAND|wxALL, 10);
	
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
	wxFileDialog dialog(this,_T("Open circuit definition file"),wxEmptyString,wxEmptyString,_T("Logic definition files (*.txt;*.ldf)|*.txt;*.ldf"));

    dialog.SetDirectory(wxGetHomeDir());
    dialog.CentreOnParent();

    if (dialog.ShowModal() == wxID_OK)
    {
        wxString info;
        info.Printf(_T("Full file name: %s\n")
                    _T("Path: %s\n")
                    _T("Name: %s"),
                    dialog.GetPath().c_str(),
                    dialog.GetDirectory().c_str(),
                    dialog.GetFilename().c_str());
        //wxMessageDialog dialog2(this, info, _T("Selected file"));
        //dialog2.ShowModal();
    }
}

void MyFrame::OnSave(wxCommandEvent &event)
  // Callback for the [File | Save waveforms as picture] menu item
{
	wxFileDialog dialog(this,_T("Save waveforms as picture"),wxEmptyString,wxEmptyString,_T("Portable network graphics (*.png)|*.png"),wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	
	dialog.SetFilterIndex(1);

	if (dialog.ShowModal() == wxID_OK)
		/*wxLogMessage(_T("%s, filter %d"),dialog.GetPath().c_str(), dialog.GetFilterIndex())*/;
}

void MyFrame::OnExit(wxCommandEvent &event)
  // Callback for the [File | Exit] menu item
{
  Close(true);
}

void MyFrame::OnHelpContents(wxCommandEvent &event)
  // Callback for the [Help | View Help] menu item
{
	help.DisplayContents();
}

void MyFrame::OnAbout(wxCommandEvent &event)
  // Callback for the [Help | About] menu item
{
	wxMessageDialog about(this, wxT("Penguin Logic is a program to simulate simple logic circuits. \n\nAndy Marshall, James Robinson, Ollie Lambert\nMay 2011"), wxT("About Penguin Logic"), wxICON_INFORMATION | wxOK);
	about.ShowModal();                  
}

void MyFrame::OnRunButton(wxCommandEvent &event)
  // Callback for the push button
{
    int n, ncycles;
    cyclescompleted = 0;
    mmz->resetmonitor();
    runnetwork(spin->GetValue());
	canvas->Render(wxT("Run button pressed"), cyclescompleted);
	SetStatusText(wxT("Run button pressed"), 1);
}

void MyFrame::OnContButton(wxCommandEvent &event)
  // Callback for the push button
{
   int ncycles = spin->GetValue();
	SetStatusText(wxT("Continue button pressed"), 1);
	if (cyclescompleted > 0)
	{
		if ((ncycles + cyclescompleted) > maxcycles)
			ncycles = maxcycles - cyclescompleted;
		runnetwork (spin->GetValue());
    }
	else
		SetStatusText(wxT("Error: nothing to continue!"), 1);
	canvas->Render(wxT("Continue button pressed"), cyclescompleted);
}

void MyFrame::OnDeviceButton(wxCommandEvent &event)
  // Callback for the SWITCH button
{
	// Get list of devices
	devlink d;
	
	//-----SWITCHES----//
	// Define options for values of switches
	wxArrayString switchValue;
		switchValue.Add(wxT("Off"));
		switchValue.Add(wxT("On"));
	
	wxString postSwitch = ":";
	vector<switchProp> switchTable;
	switchTable.empty();
	// Create switchTable
	switchProp switchtemp;
	for (d = netz->devicelist(); d != NULL; d = d->next)
	{
		if (d->kind == aswitch)
		{
			switchtemp.Name = wxString(nmz->getname(d->id));
			switchtemp.Name.Append(postSwitch);
			if(d->swstate==low)
				switchtemp.Value = switchValue.Item(0);
			else if(d->swstate==high)
				switchtemp.Value = switchValue.Item(1);
			switchtemp.ID = d->id;
			switchTable.push_back(switchtemp);
		}
	}
	

	//-----CLOCKS----//
	wxString postClock = ":";
	vector<clockProp> clockTable;

	// Create clockTable
	clockProp clocktemp;
	for (d = netz->devicelist(); d != NULL; d = d->next)
	{
		if (d->kind == aclock)
		{
			clocktemp.Name = wxString(nmz->getname(d->id));
			clocktemp.Name.Append(postClock);
			clocktemp.Value = wxString::Format(wxT("%i"),d->frequency);
			clocktemp.ID = d->id;
			clockTable.push_back(clocktemp);
		}
	}


	//----CREATE DIALOG-----//
	MyDeviceConfigDialog dialog(this, wxID_ANY, _("Device Configuration"), wxDefaultPosition,
					wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER,
					_("Device Configuration"), nmz, dmz, mmz, netz, switchTable, clockTable);
    
	
	// Get return values
	if(dialog.ShowModal()==wxID_OK)
	{
		SetStatusText(wxT("Device config settings applied"), 1);
		
		// Set new switch values
		bool ok=false;
		for(unsigned int i=0; i<switchTable.size(); i++)
		{
			if(switchTable[i].value_box->GetValue()==switchValue.Item(0))
			{
				dmz->setswitch(switchTable[i].ID, low, ok);
				if(!ok)
					cout << "Error setting switch value" << endl;
			}
			else if (switchTable[i].value_box->GetValue()==switchValue.Item(1))
			{
				dmz->setswitch(switchTable[i].ID, high, ok);
				if(!ok)
					cout << "Error setting switch value" << endl;
			}
			else
				cout << "Error setting switch value" << endl;
		}

		ok=false;
		// Set new clock values
		for(unsigned int i=0; i<clockTable.size(); i++)
		{
			dmz->setclock(clockTable[i].ID, clockTable[i].value_box->GetValue(), ok);
			if(!ok)
				cout << "Error setting clock value" << endl;
		}
	}
}

void MyFrame::OnMonitorButton(wxCommandEvent &event)
  // Callback for the MONITOR BUTTON
{
	// Define structure for table of outputs
	struct monProp
	{
		int DevID;
		int OutID;
		wxString DevName;
		wxString OutName;
		int MonID;
		bool Selected;
	};
	
	//  Create table of all outputs
	devlink d;
	vector<monProp> monTable;
	monProp montemp;
	outplink o;
	for (d = netz->devicelist(); d != NULL; d = d->next)
	{
		o = d->olist;
		while (o != NULL)
		{
			montemp.DevID = d->id;
			montemp.OutID = o->id;
			montemp.DevName = nmz->getname(montemp.DevID);
			montemp.Selected = false;
			monTable.push_back(montemp);
			o = o->next;
		}
	}

	// Put relavent details into array string to feed into checkbox dialog
	wxArrayString choices; // Array of possible monitors
	wxArrayInt selections; // Array of selected monitors
	wxString str;
	for (int i=monTable.size()-1; i>-1; i--)
	{
		// Add all elements in monTable to choices
		str.Empty();
		str = monTable[i].DevName;
		if(monTable[i].OutID !=-1)
		{
			monTable[i].OutName = nmz->getname(monTable[i].OutID);
			str.Append(wxT("."));
			str.Append(monTable[i].OutName);
		}
		choices.Add(str);
	}

	// Find location of each current monitor in "choices" and add to "selections"
	for(int i=0; i<(mmz->moncount()); i++)
	{
		// Get details of nth monitor
		name DevID, OutID;
		mmz->getmonname(i, DevID, OutID);
		// Compute monitor name in "choices" format
		str.Empty();
		str = nmz->getname(DevID);
		if(OutID !=-1)
		{
			str.Append(wxT("."));
			str.Append(nmz->getname(OutID));
		}
		// Find number of monitors in "choices" array
		for (unsigned int i=0; i<choices.size(); i++)
		{
			if (!str.compare(choices[i]))
			{
				selections.push_back(i);
				monTable[monTable.size()-i-1].Selected = true;
			}
		}
	}
	
	// Create dialog box
	wxString MonMsg = wxT("Please select the required monitor points and select OK");
	wxString MonCap = wxT("Change monitor points");
	wxMultiChoiceDialog monitor(this, MonMsg, MonCap, choices);
	monitor.SetSelections(selections);
	
	// If OK is clicked, process results
	bool Selected, ok;
	int monIndex;
	if(monitor.ShowModal() == wxID_OK)
	{
		selections = monitor.GetSelections(); // Gets the new selections
		for(unsigned int i=0; i<choices.size(); i++)
		{
			// Find out if it's meant to be selected now
			Selected = false;
			for(unsigned int j=0; j<selections.size();j++)
			{
				if(selections[j]==i)
					Selected = true;
			}
			// Find out if it was selected before
			monIndex = monTable.size()-i-1;
			if(Selected)
			{// If it wasn't selected and should be now, make a new monitor
				if(monTable[monIndex].Selected == false)
				{
					ok = false;
					mmz->makemonitor(monTable[monIndex].DevID,monTable[monIndex].OutID,ok);
					if(!ok)
						cout << "Failed to make monitor" << endl;
				}
			}
			else
			{// If it was selected and shouldn't be now, delete the monitor
				if(monTable[monIndex].Selected == true)
				{
					ok = false;
					mmz->remmonitor(monTable[monIndex].DevID,monTable[monIndex].OutID,ok);
					if(!ok)
						cout << "Failed to remove monitor" << endl;
				}
			}
		}
	}
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

// ----------------------------------------------------------------------------
// MyDeviceConfigDialog
// ----------------------------------------------------------------------------

//IMPLEMENT_CLASS(MyDeviceConfigDialog, wxPropertySheetDialog)

BEGIN_EVENT_TABLE(MyDeviceConfigDialog, wxPropertySheetDialog)
END_EVENT_TABLE()

MyDeviceConfigDialog::MyDeviceConfigDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos,
						const wxSize& sz, long style, const wxString& name,
						names *names_mod, devices *devices_mod, monitor *monitor_mod,
						network *network_mod,vector<switchProp> &switchTable, vector<clockProp> &clockTable):
  wxPropertySheetDialog(parent, wxID_ANY, title, pos, sz, style)
{
	nmz = names_mod;
	dmz = devices_mod;
	mmz = monitor_mod;
	netz = network_mod;

    CreateButtons(wxOK|wxCANCEL);

    wxBookCtrlBase* notebook = GetBookCtrl();
		wxPanel* switchProperties = CreateSwitchPropertiesPage(notebook, switchTable);
	notebook->AddPage(switchProperties, _("Switch Properties"));
		wxPanel* clockProperties = CreateClockPropertiesPage(notebook, clockTable);
	notebook->AddPage(clockProperties, _("Clock Properties"));

    LayoutDialog();
}

wxPanel* MyDeviceConfigDialog::CreateSwitchPropertiesPage(wxWindow* parent,vector<switchProp> &switchTable)
{
	wxArrayString switchValue;
		switchValue.Add(wxT("Off"));
		switchValue.Add(wxT("On"));

	// Create layout
	wxPanel* panel = new wxPanel(parent, wxID_ANY);
	wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
		wxGridSizer *item0 = new wxGridSizer(2, 10, 10);
		// Create switch options
		for(int i=switchTable.size()-1; i>=0; i--)
		{
				switchTable[i].field_box = new wxStaticText(panel, wxID_ANY,switchTable[i].Name);
			item0->Add(switchTable[i].field_box, 0, wxALL, 5);
				switchTable[i].value_box = new wxComboBox(panel, ID_SWITCH, switchTable[i].Value, wxDefaultPosition, wxDefaultSize, switchValue, wxCB_READONLY);
			item0->Add(switchTable[i].value_box, 0, wxALL, 0);
		}
	topSizer->Add( item0, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5 );
	panel->SetSizer(topSizer);
    topSizer->Fit(panel);

    return panel;
}

wxPanel* MyDeviceConfigDialog::CreateClockPropertiesPage(wxWindow* parent, vector<clockProp> &clockTable)
{
	// Create layout
	wxPanel* panel = new wxPanel(parent, wxID_ANY);
	wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
		wxGridSizer *item0 = new wxGridSizer(2, 10, 10);
			// Create clock options
			for(int i=clockTable.size()-1; i>=0; i--)
			{
					clockTable[i].field_box = new wxStaticText(panel, wxID_ANY, clockTable[i].Name);
				item0->Add(clockTable[i].field_box, 0, wxALL, 5);			
					clockTable[i].value_box = new wxSpinCtrl(panel, ID_FONT_SIZE, clockTable[i].Value, wxDefaultPosition, wxDefaultSize);
				item0->Add(clockTable[i].value_box, 0, wxALL, 5);
			}
	topSizer->Add( item0, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5 );
	topSizer->AddSpacer(5);

    panel->SetSizer(topSizer);
    topSizer->Fit(panel);

    return panel;
}