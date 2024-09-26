#pragma once
#include "FileSelectFrame.h"
#include "MainFrame.h"
#include <wx/wx.h>



class App : public wxApp{

private:
	wxString LoadFilePath();

public:
	virtual bool OnInit();

};


