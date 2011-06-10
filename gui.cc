#include "gui.h"

// ---------------------------------------------------------------------------//
// MyFrame
// ---------------------------------------------------------------------------//

// Event Table
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_MENU(wxID_OPEN, MyFrame::OnOpen)
	EVT_MENU(wxID_SAVE, MyFrame::OnSave)
	EVT_MENU(wxID_EXIT, MyFrame::OnExit)
	EVT_MENU(wxID_HELP_CONTENTS, MyFrame::OnHelpContents)
	EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
	//EVT_BUTTON(RUN_BUTTON_ID, MyFrame::OnRunButton)
	//EVT_BUTTON(CONT_BUTTON_ID, MyFrame::OnContButton)
	EVT_BUTTON(PLAY_BUTTON_ID, MyFrame::OnPlayButton)
	EVT_BUTTON(CLEAR_BUTTON_ID, MyFrame::OnClearButton)
	EVT_BUTTON(DEVICE_BUTTON_ID, MyFrame::OnDeviceButton)
	EVT_BUTTON(MONITOR_BUTTON_ID, MyFrame::OnMonitorButton)
END_EVENT_TABLE()

MyFrame::MyFrame(wxWindow *parent, const wxString& title, const wxPoint& pos,
	const wxSize& size, names *names_mod, devices *devices_mod,
	monitor *monitor_mod, network *network_mod, wxString deffilename,
	long style):	wxFrame(parent, wxID_ANY, title, pos, size, style)
  // Constructor - initialises pointers to names, devices and monitor classes,
  // lays out widgets using sizers
{
	CanvasClear = true;
	SetIcon(wxIcon(wx_icon_xpm));
	nmz = names_mod;
	dmz = devices_mod;
	mmz = monitor_mod;
	netz = network_mod;
	defname = deffilename;
	if (nmz == NULL || dmz == NULL || mmz == NULL || netz == NULL)
	{
		cout << "Cannot operate GUI without names, devices, monitor"
				<< "and network classes" << endl;
		exit(1);
	}

	// Define File menu
	wxMenu *fileMenu = new wxMenu;
	//fileMenu->Append(wxID_OPEN, wxT("&Open circuit definition file"));
	//fileMenu->Append(wxID_SAVE, wxT("&Save waveforms as picture"));
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
			sw = new MyScrolledWindow(this, wxID_ANY,
					wxPoint(0, 0), wxSize(400, 400), wxVSCROLL|wxHSCROLL, mmz, nmz);
			left_sizer->Add(sw, 1, wxEXPAND | wxALL, 10);
			// Button panel
			wxFlexGridSizer *button_sizer = new wxFlexGridSizer(2,0,0,0);
					// Run button
				//button_sizer->Add(new wxButton(this, RUN_BUTTON_ID, wxT("Run")), 0,
												//wxALIGN_CENTER_VERTICAL|wxALL, 10);
					// Play/Pause button
					play_pause = new wxButton(this, PLAY_BUTTON_ID, wxT("Play / Pause"));
					timer = new MyTimer();
					timer->frame = this;

				button_sizer->Add(play_pause, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 10);
					// Spinner (label)
				button_sizer->Add(new wxStaticText(this, wxID_ANY, wxT("  Simulation\ncycle period:")),
							0, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxLEFT, 10);
					// Spinner (device)
					spin = new wxSpinCtrl(this, MY_SPINCNTRL_ID, wxString(wxT("100")));
					spin->SetRange(10,1000);
				button_sizer->Add(spin, 0 , wxALIGN_CENTER_VERTICAL|wxALL, 10);
					// DEVICE button
				button_sizer->Add(new wxButton(this, DEVICE_BUTTON_ID,
									wxT("Change device properties")), 0,
									wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 10);
					// Continue button
				//button_sizer->Add(new wxButton(this, CONT_BUTTON_ID,
							//wxT("Continue")), 0, wxALIGN_CENTER_VERTICAL|wxALL, 10);
					// Clear button
				button_sizer->Add(new wxButton(this, CLEAR_BUTTON_ID,
							wxT("Reset simulation")), 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 10);
					// Static text (placeholder)
				button_sizer->Add(new wxStaticText(this, wxID_ANY, wxT("")), 0,
							wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxLEFT, 10);
					// Static text (placeholder)
				button_sizer->Add(new wxStaticText(this, wxID_ANY, wxT("")), 0,
								wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxLEFT, 10);
					// MONITOR button
				button_sizer->Add(new wxButton(this, MONITOR_BUTTON_ID,
								wxT("Change monitor points")), 0,
								wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 10);
			left_sizer->Add(button_sizer, 0, wxEXPAND | wxALL, 10);
	topsizer->Add(left_sizer, 1, wxEXPAND | wxALL, 10);
	//	//Right panel
	//	wxBoxSizer *right_sizer = new wxBoxSizer(wxVERTICAL);
	//	right_sizer->SetMinSize(wxSize(250,-1));
	//		wxScrolledWindow *sp = new wxScrolledWindow(this, wxID_ANY,
	//									wxPoint(-1, -1), wxSize(250, -1),
	//									wxVSCROLL|wxHSCROLL);
	//			const int p_inc = 40;		// Scroll increment
	//			const int px_size = 800;	// Canvas size
	//			const int py_size = 1000;	// Canvas size
	//			sp->SetScrollbars(p_inc, p_inc, px_size/p_inc, py_size/p_inc);
	//			//sp->Show();
	//			// Right hand panel
	//				wxBoxSizer *info_sizer = new wxBoxSizer(wxVERTICAL);
	//					// Filename
	//					wxBoxSizer *deffile_sizer = new wxBoxSizer(wxHORIZONTAL);
	//					deffile_sizer->Add(new wxStaticText(sp, wxID_ANY,
	//									wxT("Definition file name:")), 0,
	//									wxLEFT|wxRIGHT|wxALIGN_CENTER_VERTICAL ,5);
	//					deffile_sizer->Add(new wxStaticText(sp, wxID_ANY, defname),
	//									0, wxALIGN_CENTER_VERTICAL ,0);
	//				info_sizer->Add(deffile_sizer, 0, wxEXPAND|wxALL,0);
	//					// Devices
	//					wxBoxSizer *devices_sizer = new wxBoxSizer(wxVERTICAL);
	//					devices_sizer->Add(new wxStaticText(sp, wxID_ANY,
	//									wxT("DEVICES")), 0, wxEXPAND|wxALL,5);
	//						for (devlink d = netz->devicelist(); d != NULL;
	//									d = d->next)
	//							devices_sizer->Add(new wxStaticText(sp, wxID_ANY,
	//											DeviceProps(d)), 0, wxEXPAND|wxLEFT,25);
	//				info_sizer->Add(devices_sizer, 0, wxEXPAND|wxALL, 0);
	//					// Connections
	//					wxBoxSizer *connections_sizer = new wxBoxSizer(wxVERTICAL);
	//					connections_sizer->Add(new wxStaticText(panel, wxID_ANY,
	//										wxT("CONNECTIONS")), 0, wxEXPAND|wxALL,5);
	//				info_sizer->Add(connections_sizer, 0, wxEXPAND|wxALL, 5);
	//					// Monitors
	//					wxBoxSizer *monitors_sizer = new wxBoxSizer(wxVERTICAL);
	//					monitors_sizer->Add(new wxStaticText(panel, wxID_ANY,
	//									wxT("MONITORS")), 0, wxEXPAND|wxALL,5);
	//				info_sizer->Add(monitors_sizer, 0, wxEXPAND|wxALL, 5);
	//			sp->SetSizer(info_sizer);
	//	right_sizer->Add(sp, 1, wxEXPAND|wxALL, 10);
	//topsizer->Add(right_sizer, 0, wxEXPAND|wxALL, 10);
	SetSizeHints(800, 400);
	SetSizer(topsizer);
}

wxString MyFrame::DeviceProps(devlink d)
{
	int numinputs =0;
	wxString wxStr = wxString(nmz->getname(d->id).c_str(), wxConvUTF8);
	wxStr.Append(wxT("  (type="));
	switch(d->kind)
	{
	case aswitch:
		wxStr.Append(wxT("SWITCH, state="));
		switch(d->swstate)
		{
		case low: wxStr.Append(wxT("LOW)")); break;
		case high: wxStr.Append(wxT("HIGH)")); break;
		}
		break;
	case aclock:
		wxStr.Append(wxT("CLOCK, period="));
		wxStr << d->frequency;
		wxStr.Append(wxT(")"));
		break;
	case andgate:
		wxStr.Append(wxT("AND, inputs="));
		for(inplink i=d->ilist; i!=NULL; i=i->next)
			numinputs++;
		wxStr << numinputs;
		wxStr.Append(wxT(")"));
		break;
	case nandgate:
		wxStr.Append(wxT("NAND, inputs="));
		for(inplink i=d->ilist; i!=NULL; i=i->next)
			numinputs++;
		wxStr << numinputs;
		wxStr.Append(wxT(")"));
		break;
	case orgate:
		wxStr.Append(wxT("OR, inputs="));
		for(inplink i=d->ilist; i!=NULL; i=i->next)
			numinputs++;
		wxStr << numinputs;
		wxStr.Append(wxT(")"));
		break;
	case norgate:
		wxStr.Append(wxT("NOR, inputs="));
		for(inplink i=d->ilist; i!=NULL; i=i->next)
			numinputs++;
		wxStr << numinputs;
		wxStr.Append(wxT("), inputs="));
		break;
	case xorgate:
		wxStr.Append(wxT("XOR)"));
		break;
	case dtype:
		wxStr.Append(wxT("DTYPE)"));
		break;
	}
	return wxStr;
}

void MyFrame::runnetwork(int ncycles)
  // Function to run the network
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

void MyFrame::OnDevelopment(wxCommandEvent &event)
  // Produces a dialog saying that feature is under development and unavailable
{
	wxMessageDialog development(this, wxT("We apologise, but this feature is currently under development and not yet available."), wxT("Error: Feature unavailable"), wxICON_INFORMATION | wxOK);
	development.ShowModal();                  
}

void MyFrame::OnOpen(wxCommandEvent &event)
  // Callback for the [File | Open circuit definition file] menu item
{
	wxFileDialog dialog(this,_T("Open circuit definition file"),wxEmptyString,
		wxEmptyString,_T("Logic definition files (*.txt;*.ldf)|*.txt;*.ldf"));

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
  // Callback for the [File | Save waveintforms as picture] menu item
{
	wxFileDialog dialog(this,_T("Save waveforms as picture"),wxEmptyString,
			wxEmptyString,_T("Portable network graphics (*.png)|*.png"),
			wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	
	dialog.SetFilterIndex(1);
	if (dialog.ShowModal() == wxID_OK)
	{
		// Get size of canvas
		wxSize ImSize = sw->canvas->GetSize();
		unsigned int ImHeight = ImSize.GetHeight();
		unsigned int ImWidth = ImSize.GetWidth();

		// Get image data from canvas
		GLubyte* mirror_image = new GLubyte[ImWidth * ImHeight * 3];
		GLubyte* image = new GLubyte[ImWidth * ImHeight * 3];
		sw->canvas->GetImage(mirror_image);
		
		// Flip image to compensate for image being upside down
		int rev_val=ImHeight-1;
		int bytesPerPixel = 3;
		for(unsigned int x=0; x<ImWidth; x++)
		{
			for(unsigned int y=0; y<ImHeight; y++)
			{
				//image[x, y,  = mirror_image[x, ImHeight-y];
				image[(x+y*ImWidth)*bytesPerPixel+0] = 
								mirror_image[(x+(rev_val-y)*ImWidth)*3];
				image[(x+y*ImWidth)*bytesPerPixel+1] = 
								mirror_image[(x+(rev_val-y)*ImWidth)*3 + 1];
				image[(x+y*ImWidth)*bytesPerPixel+2] = 
								mirror_image[(x+(rev_val-y)*ImWidth)*3 + 2];
			}
		}
		
		// Store image in wxImage format
		wxImage CanvasImage = wxImage(ImWidth, ImHeight, image, false);
		CanvasImage.AddHandler(new wxPNGHandler);

		// Save file and display success notification
		CanvasImage.SaveFile(dialog.GetPath(), wxBITMAP_TYPE_PNG);
		SetStatusText(wxT("File saved successfully"),1);
	}
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

/*void MyFrame::OnRunButton(wxCommandEvent &event)
  // Callback for the push button
{
    if(spin->GetValue() <= maxcycles)
	{
		cyclescompleted = 0;
		mmz->resetmonitor();
		runnetwork(spin->GetValue());
		sw->canvas->Render(cyclescompleted);
		SetStatusText(wxT("Run button pressed"), 1);
	}
	else
	{
		wxMessageDialog *error = new wxMessageDialog(this, wxT("Error: max number of cycles exceeded, please reduce the number of cycles using the spinner"));
		error->ShowModal();
	}
}*/

/*void MyFrame::OnContButton(wxCommandEvent &event)
  // Callback for the push button
{
   int ncycles = spin->GetValue();
	SetStatusText(wxT("Continue button pressed"), 1);
	if (cyclescompleted > 0 && cyclescompleted + spin->GetValue()<=maxcycles)
	{
		if ((ncycles + cyclescompleted) > maxcycles)
			ncycles = maxcycles - cyclescompleted;
		runnetwork (spin->GetValue());
    }
	else if(cyclescompleted + spin->GetValue()>maxcycles)
	{
		wxMessageDialog *error = new wxMessageDialog(this, wxT("Error: max number of cycles exceeded, please start again by clicking the 'Run' button"));
		error->ShowModal();
	}
	else
		SetStatusText(wxT("Error: nothing to continue!"), 1);
	sw->canvas->Render(cyclescompleted);
}*/

void MyFrame::OnPlayButton(wxCommandEvent &event)
{
	if(timer->IsRunning())
	{
		timer->Stop();
		SetStatusText(wxT("Simulation paused"),1);
	}
	else
	{
		if(CanvasClear)
		{
			mmz->resetmonitor();
			timer->cycles = 0;
			CanvasClear = false;
		}
		timer->Start(spin->GetValue(),wxTIMER_CONTINUOUS);
		SetStatusText(wxT("Simulation running"),1);
	}
}

void MyFrame::OnClearButton(wxCommandEvent &event)
{
	if(!CanvasClear)
	{
		if(timer->IsRunning())
			timer->Stop();
		CanvasClear = true;
		sw->canvas->Clear();
		timer->cycles = 0;
		SetStatusText(wxT("Canvas has been cleared"),1);
	}
	else
		SetStatusText(wxT("Nothing to clear!"),1);
}

void MyFrame::OnDeviceButton(wxCommandEvent &event)
  // Callback for the SWITCH button
{
	bool running;
	if(timer->IsRunning())
	{
		timer->Stop();
		running = true;
	}
	else
		running = false;

	// Get list of devices
	devlink d;
	
	//-----SWITCHES----//
	// Define options for values of switches
	wxArrayString switchValue;
		switchValue.Add(wxT("Off"));
		switchValue.Add(wxT("On"));
	
	wxString postSwitch = wxString(":", wxConvUTF8);
	vector<switchProp> switchTable;
	switchTable.empty();
	// Create switchTable
	switchProp switchtemp;
	for (d = netz->devicelist(); d != NULL; d = d->next)
	{
		if (d->kind == aswitch)
		{
			switchtemp.Name = wxString(nmz->getname(d->id).c_str(), wxConvUTF8);
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
	wxString postClock = wxString(":", wxConvUTF8);
	vector<clockProp> clockTable;

	// Create clockTable
	clockProp clocktemp;
	for (d = netz->devicelist(); d != NULL; d = d->next)
	{
		if (d->kind == aclock)
		{
			clocktemp.Name = wxString(nmz->getname(d->id).c_str(), wxConvUTF8);
			clocktemp.Name.Append(postClock);
			clocktemp.Value = wxString::Format(wxT("%i"),d->frequency);
			clocktemp.ID = d->id;
			clockTable.push_back(clocktemp);
		}
	}


	//----CREATE DIALOG-----//
	MyDeviceConfigDialog dialog(this, _("Device Configuration"),
							wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER,
							_("Device Configuration"), nmz, dmz, mmz, netz,
							switchValue, switchTable, clockTable);
    	
	//----GET RETURN VALUES----//
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
			dmz->setclock(clockTable[i].ID,
							clockTable[i].value_box->GetValue(), ok);
			if(!ok)
				cout << "Error setting clock value" << endl;
		}
	}
	
	if(running)
		timer->Start();
}

void MyFrame::OnMonitorButton(wxCommandEvent &event)
  // Callback for the MONITOR BUTTON
{
	bool running;
	if(timer->IsRunning())
	{
		cout << "Timer is running" << endl;
		wxCommandEvent redraw_event;
		OnPlayButton(redraw_event);
		running = true;
	}
	else
		running = false;
	
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
			montemp.DevName = wxString(nmz->getname(montemp.DevID).c_str(),
										wxConvUTF8);
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
			monTable[i].OutName = wxString(nmz->
							getname(monTable[i].OutID).c_str(), wxConvUTF8);
			str.Append(wxT("."));
			str.Append(monTable[i].OutName);
		}
		choices.Add(str);
	}

	// Find location of each monitor in "choices" and add to "selections"
	for(int i=0; i<(mmz->moncount()); i++)
	{
		// Get details of nth monitor
		name DevID, OutID;
		mmz->getmonname(i, DevID, OutID);
		// Compute monitor name in "choices" format
		str.Empty();
		str = wxString(nmz->getname(DevID).c_str(), wxConvUTF8);
		if(OutID !=-1)
		{
			str.Append(wxT("."));
			str.Append(wxString(nmz->getname(OutID).c_str(), wxConvUTF8));
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
	wxString MonMsg =
		wxT("Please select the required monitor points and select OK");
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
					mmz->makemonitor(monTable[monIndex].DevID,
										monTable[monIndex].OutID,ok);
					if(!ok)
					{
						wxMessageDialog *error = new wxMessageDialog(this,
											wxT("Failed to make monitor"));
						error->ShowModal();
					}
				}
			}
			else
			{// If it was selected and shouldn't be now, delete the monitor
				if(monTable[monIndex].Selected == true)
				{
					ok = false;
					mmz->remmonitor(monTable[monIndex].DevID,
									monTable[monIndex].OutID,ok);
					if(!ok)
					{
						wxMessageDialog *error = new wxMessageDialog(this,
										wxT("Failed to remove monitor"));
						error->ShowModal();
					}
				}
			}
		}
		// Clear the canvas
		wxCommandEvent clear_event;
		OnClearButton(clear_event);
	}
	if(running)
	{
		wxCommandEvent redraw_event;
		OnPlayButton(redraw_event);
	}
}


// ---------------------------------------------------------------------------//
// MyScrolledWindow
// ---------------------------------------------------------------------------//

// Event Table
BEGIN_EVENT_TABLE(MyScrolledWindow, wxScrolledWindow)
	//EVT_SCROLLWIN(MyScrolledWindow::OnScroll)
	//wxEVT_SCROLLWIN(wxScrollWinEventFunction)
END_EVENT_TABLE()

MyScrolledWindow::MyScrolledWindow(wxWindow *parent, wxWindowID winid,
					const wxPoint& pos, const wxSize& size, long style,
					monitor* monitor_mod, names* names_mod):
					wxScrolledWindow(parent, winid, pos, size, style)
{
	const int s_inc = 40;		// Scroll increment
	const int cx_size = 3000;	// Canvas size
	const int cy_size = 2000;	// Canvas size
	SetScrollbars(s_inc, s_inc, cx_size/s_inc, cy_size/s_inc);
	canvas = new MyGLCanvas(this, wxID_ANY, monitor_mod, names_mod, 
						wxDefaultPosition, wxSize(cx_size,cy_size));
}

void MyScrolledWindow::OnScroll(wxScrollWinEvent& event)
{
	/*cout << "Scroll request detected" << endl;
	int *Hpix, Vpix;
	GetScrollPixelsPerUnit(Hpix, Vpix);
	cout << "Got scroll pixels" << endl;
	cout << Hpix << Vpix << endl;
	cout << *Hpix << *Vpix << endl;
	cout << &Hpix << &Vpix << endl;
	switch(event.GetOrientation())
	{
	case 4:	// Horizontal scrolling
		Scroll(event.GetPosition()*(*Hpix),-1);
		break;
	case 8: // Vertical scrolling
		Scroll(-1,event.GetPosition()*(*Vpix));
		break;
	default:
		cout << "ERROR: scrolling failed" << endl;
		break;
	}
	cout << "Orientation = " << event.GetOrientation() << endl;;
	cout << "Position = " << event.GetPosition() << endl;

	// Refresh canvas
	canvas->OnScroll(event);*/
}



// ---------------------------------------------------------------------------//
// MyGLCanvas
// ---------------------------------------------------------------------------//

// Event Table
BEGIN_EVENT_TABLE(MyGLCanvas, wxGLCanvas)
	EVT_SIZE(MyGLCanvas::OnSize)
	EVT_PAINT(MyGLCanvas::OnPaint)
	EVT_MOUSE_EVENTS(MyGLCanvas::OnMouse)
	EVT_SCROLLWIN(MyGLCanvas::OnScroll)
END_EVENT_TABLE()
  
MyGLCanvas::MyGLCanvas(wxWindow *parent, wxWindowID id, monitor* monitor_mod,
					names* names_mod, const wxPoint& pos, const wxSize& size,
					long style, const wxString& name):
					wxGLCanvas(parent, id, pos, size, style, name)
  // Constructor - initialises private variables
{
	mmz = monitor_mod;
	nmz = names_mod;
	init = false;
	cyclesdisplayed = -1;
}

void MyGLCanvas::Render(int cycles)
  // Draws canvas contents
{
	float y1, y2;
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

	wxSize CanvasSize = GetSize();
	const unsigned int CANVAS_HEIGHT=CanvasSize.GetHeight();
	const unsigned int CANVAS_WIDTH =CanvasSize.GetWidth();
	const int SIG_LOW = 10;
	const int SIG_HIGH = 30;
	const int SEPARATION = 50;
	const int INDENT = 100;
	const int LENGTH = 20;
	const int MAX_NAME_LENGTH = 10;

		// draw the first monitor signal, get trace from monitor class
	if ((cyclesdisplayed >= 0) && (mmz->moncount() > 0))
	{
		for(int m=mmz->moncount()-1; m>=0; m--)
		{
			// Get name of monitor
			name n1, n2;
			mmz->getmonname(m,n1, n2);
			wxString mon_name = wxString(nmz->getname(n1).c_str(),
										wxConvUTF8);
			if(n2 != -1)	// Append output name if necessary
			{
				mon_name.Append(wxString(".", wxConvUTF8));
				mon_name.Append(wxString(nmz->getname(n2).c_str(),
										wxConvUTF8));
			}

			// Truncate name of monitor if too long
			if(mon_name.Len()>=MAX_NAME_LENGTH)
			{
				mon_name.Truncate(MAX_NAME_LENGTH-2);
				mon_name.Append(wxT(".."));
			}
			
			// Display name of monitor
			glColor3ub(0, 100, 0);
			glRasterPos2f(5, CANVAS_HEIGHT-(m+1)*SEPARATION + 15);
				// Display each character in turn
			for (unsigned int i = 0; (i < mon_name.Len()); i++)
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, mon_name[i]);
			
			// Draw axes
			glColor3ub(0, 0, 0);
			glBegin(GL_LINE_STRIP);
				glVertex2f(INDENT-1,CANVAS_HEIGHT-(m+1)*SEPARATION+SIG_HIGH+6);
				glVertex2f(INDENT-1,CANVAS_HEIGHT-(m+1)*SEPARATION+SIG_LOW-1);
				glVertex2f(LENGTH*cyclesdisplayed+INDENT,
							CANVAS_HEIGHT-(m+1)*SEPARATION+SIG_LOW-1);
			glEnd();
			
			// Draw tickmarks
			glColor3ub(128, 128, 128);
			glBegin(GL_LINES);
			for(int i=0; i<cyclesdisplayed; i++)
			{
				glVertex2f(LENGTH*(i+1)+INDENT,
							CANVAS_HEIGHT-(m+1)*SEPARATION+SIG_LOW-1);
				glVertex2f(LENGTH*(i+1)+INDENT,
							CANVAS_HEIGHT-(m+1)*SEPARATION+SIG_LOW-6);
			}
			glEnd();
						
			// Plot waveform for monitor
			
			glColor3ub(255, 0, 0);
			glBegin(GL_LINE_STRIP);
			for (int i=0; i<cyclesdisplayed; i++)
			{
				if (mmz->getsignaltrace(m, i, s))
				{
					// Define beginning and end height of line					
					if (s==low)
					{
						y1 = CANVAS_HEIGHT-(m+1)*SEPARATION + SIG_LOW;
						y2 = y1;
					}   
					else if (s==high)
					{
						y1 = CANVAS_HEIGHT-(m+1)*SEPARATION + SIG_HIGH;
						y2 = y1;
					}
					else if (s==rising)
					{
						y1 = CANVAS_HEIGHT-(m+1)*SEPARATION + SIG_LOW;
						y2 = CANVAS_HEIGHT-(m+1)*SEPARATION + SIG_HIGH;
					}
					else if (s==falling)
					{
						y1 = CANVAS_HEIGHT-(m+1)*SEPARATION + SIG_HIGH;
						y2 = CANVAS_HEIGHT-(m+1)*SEPARATION + SIG_LOW;
					}
					else
						break;

					// Plot line
					glVertex2f(LENGTH*(cyclesdisplayed-i-1)+INDENT+LENGTH, y1); 
					glVertex2f(LENGTH*(cyclesdisplayed-i-1)+INDENT, y2);
				}
				else
					cout << "Couldn't get signal trace" << endl;
			}
			glEnd();
		}
	}

	// Flush the graphics pipeline and swap the back buffer to the front
	glFlush();
	SwapBuffers();
}

void MyGLCanvas::Clear()
{
	cyclesdisplayed = -1;
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
	SwapBuffers();
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
	SwapBuffers();
}

void MyGLCanvas::GetImage(unsigned char* &pixels)
{
	wxSize CanvasSize = GetSize();
	glReadBuffer(GL_FRONT);
	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	glReadPixels(0, 0, CanvasSize.GetWidth(), CanvasSize.GetHeight(),
					GL_RGB, GL_UNSIGNED_BYTE, pixels);
	cout << "Pixels" << pixels << endl;
}

void MyGLCanvas::OnScroll(wxScrollWinEvent& event)
{
	cout << "Scroll event detected" << endl;
	//wxPaintDC dc(this); // required for correct refreshing under MS windows
	//Render();
}

void MyGLCanvas::InitGL()	// Function to initialise the GL context
{
	int w, h;

	GetClientSize(&w, &h);
	SetCurrent();
	glDrawBuffer(GL_BACK);
	glClearColor(1.0/255*255, 1.0/255*255, 1.0/255*255, 0.0);
	glViewport(0, 0, (GLint) w, (GLint) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h, -1, 1); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void MyGLCanvas::OnSize(wxSizeEvent& event)
	// Callback function for when the canvas is resized
{
	wxGLCanvas::OnSize(event); // required on some platforms
	init = false;
	Refresh(); // required by some buggy nvidia graphics drivers,
	Update();  // harmless on other platforms!
}

void MyGLCanvas::OnPaint(wxPaintEvent& event)
	// Callback function for when the canvas is exposed
{
	wxPaintDC dc(this); // required for correct refreshing under MS windows
	Render();
}

void MyGLCanvas::OnMouse(wxMouseEvent& event)
	// Callback function for mouse events inside the GL canvas
{
	Render();
	//init = false;
	//Refresh(); // required by some buggy nvidia graphics drivers,
	//Update();  // harmless on other platforms!
}




// ---------------------------------------------------------------------------//
// MyDeviceConfigDialog
// ---------------------------------------------------------------------------//

// Event Table
BEGIN_EVENT_TABLE(MyDeviceConfigDialog, wxPropertySheetDialog)
END_EVENT_TABLE()

MyDeviceConfigDialog::MyDeviceConfigDialog(wxWindow* parent,
						const wxString& title, long style,
						const wxString& name, names *names_mod,
						devices *devices_mod, monitor *monitor_mod,
						network *network_mod, wxArrayString switchValue,
						vector<switchProp> &switchTable,
						vector<clockProp> &clockTable):
						wxPropertySheetDialog(parent, wxID_ANY, title,
							wxDefaultPosition, wxSize(400,200), style)
{
	nmz = names_mod;
	dmz = devices_mod;
	mmz = monitor_mod;
	netz = network_mod;

    CreateButtons(wxOK|wxCANCEL);

    wxBookCtrlBase* notebook = GetBookCtrl();
		wxPanel* switchProperties =
			CreateSwitchPropertiesPage(notebook, switchValue, switchTable);
	notebook->AddPage(switchProperties, _("Switch Properties"));
		wxPanel* clockProperties =
			CreateClockPropertiesPage(notebook, clockTable);
	notebook->AddPage(clockProperties, _("Clock Properties"));
	
    LayoutDialog();
}

wxPanel* MyDeviceConfigDialog::CreateSwitchPropertiesPage(wxWindow* parent,
				wxArrayString switchValue, vector<switchProp> &switchTable)
{
	// Create layout
	wxPanel* panel = new wxPanel(parent, wxID_ANY);
		wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
			wxGridSizer *item0 = new wxGridSizer(2, 10, 10);
			// Create labels and combo boxes for all switches
			for(int i=switchTable.size()-1; i>=0; i--)
			{
					switchTable[i].field_box =
						new wxStaticText(panel, wxID_ANY,switchTable[i].Name);
				item0->Add(switchTable[i].field_box, 0, wxALL, 5);
					switchTable[i].value_box = new wxComboBox(panel, wxID_ANY,
						switchTable[i].Value, wxDefaultPosition, wxDefaultSize,
						switchValue, wxCB_READONLY);
				item0->Add(switchTable[i].value_box, 0, wxALL, 0);
			}
		topSizer->Add( item0, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5);
		topSizer->AddSpacer(5);
		topSizer->Fit(panel);
	panel->SetSizer(topSizer);
	
	return panel;
}

wxPanel* MyDeviceConfigDialog::CreateClockPropertiesPage(wxWindow* parent,
						vector<clockProp> &clockTable)
{
	// Create layout
	wxPanel* panel = new wxPanel(parent, wxID_ANY);
		wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
			wxGridSizer *item0 = new wxGridSizer(2, 10, 10);
				// Create lables and spinners for all clocks
				for(int i=clockTable.size()-1; i>=0; i--)
				{
						clockTable[i].field_box = new wxStaticText(panel,
												wxID_ANY, clockTable[i].Name);
					item0->Add(clockTable[i].field_box, 0, wxALL, 5);			
						clockTable[i].value_box = new wxSpinCtrl(panel,
												wxID_ANY, clockTable[i].Value);
					item0->Add(clockTable[i].value_box, 0, wxALL, 5);
				}
		topSizer->Add( item0, 1, wxGROW|wxALIGN_CENTRE|wxALL, 5 );
		topSizer->AddSpacer(5);
		topSizer->Fit(panel);
	panel->SetSizer(topSizer);
	
    return panel;
}


// ---------------------------------------------------------------------------//
// MyTimer
// ---------------------------------------------------------------------------//

void MyTimer::Notify()
{
	cycles++;
	frame->runnetwork(1);
	frame->sw->canvas->Render(cycles);
}
