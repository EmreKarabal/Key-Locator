#include "FileSelectFrame.h"
#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/filedlg.h>
#include <wx/fileconf.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>

const wxString CONFIG_FILE = "config.ini";

FileSelectFrame::FileSelectFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title){
	
	
	wxPanel* panel = new wxPanel(this, wxID_ANY);
	wxButton* selectFileButton = new wxButton(panel, wxID_ANY, "Select file", wxPoint(200,110), wxSize(100, 40));
	selectFileButton->Bind(wxEVT_BUTTON, &FileSelectFrame::OnSelectFile, this);
	
	wxLogMessage("Hello there! This is program is still WIP. Please select your .key file that you use in BMS. If you're using the alternative launcher you should make a copy of your save and select it!");
	
}

void FileSelectFrame::OnSelectFile(wxCommandEvent& evt) {

    wxFileDialog openFileDialog(this, _("Open file"), "", "",
        "All files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return; // User canceled the dialog

    wxString filePath = openFileDialog.GetPath();

    // Store the selected file path in the configuration file
    SaveFilePath(filePath);

    // Proceed to the main application after file selection
    LaunchMainApp(filePath);
}

void FileSelectFrame::LaunchMainApp(const wxString& filePath)
{
	this->Hide();

	MainFrame* mainFrame = new MainFrame(filePath, "BMS Key Locator by Buzzard & Hydra");
	mainFrame->SetClientSize(800, 600);
	mainFrame->Center();
	mainFrame->Show();

	this->Close();

}

void FileSelectFrame::SaveFilePath(const wxString& filePath)
{

	wxString configDir = wxStandardPaths::Get().GetUserDataDir();

	wxFileName::Mkdir(configDir, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);

	wxString configPath = configDir + "\\" + CONFIG_FILE;

	wxFileConfig config("KeyLocator", "Hydra", configPath);
	bool flag = config.Write("FilePath", filePath);

	if (!flag) {
		wxLogError("Cannot write to config file!");
	}
	if (!config.Flush()) {
		wxLogError("Failed to Flush()!");
	}


}
