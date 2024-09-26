#pragma once
#include <wx/wx.h>
class FileSelectFrame : public wxFrame{
public:

	FileSelectFrame(const wxString& title);


private:

	void OnSelectFile(wxCommandEvent& evt);
	void LaunchMainApp(const wxString& filePath);
	void SaveFilePath(const wxString& filePath);



};

