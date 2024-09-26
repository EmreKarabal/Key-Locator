#include "App.h"
#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/fileconf.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>


wxIMPLEMENT_APP(App);

const wxString CONFIG_FILE = "config.ini";
wxString filePath = "";



wxString App::LoadFilePath()
{
	wxString configDir = wxStandardPaths::Get().GetUserDataDir();

	wxString configPath = configDir + "\\" + CONFIG_FILE;
	

	wxFileConfig config("KeyLocator", "Hydra", configPath);

	

	if (wxFileName::FileExists(configPath)) {

		if (!config.Read("FilePath", &filePath)) {

			wxLogError("Read() has problems!");

		}

	}

	return filePath;

}

bool App::OnInit() {

	filePath = LoadFilePath();

	if (filePath.IsEmpty()) {

		FileSelectFrame* frame = new FileSelectFrame("Select your .key file");
		frame->SetClientSize(500, 200);
		frame->Center();
		frame->Show();
	}
	else {
		MainFrame* mainFrame = new MainFrame(filePath, "BMS Key Locator");
		mainFrame->SetClientSize(800, 600);
		mainFrame->Center();
		mainFrame->Show();
	}

	return true;
}
