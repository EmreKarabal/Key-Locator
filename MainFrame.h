#pragma once
#include <wx/wx.h>

class MainFrame : public wxFrame{
private:
	void OnKeyDown(wxKeyEvent& evt);
	void OnKeyUp(wxKeyEvent& evt);
	void ResetModifiers();
	void readFile(std::string filePath);


	

	bool shift = false;
	bool ctrl = false;
	bool alt = false;




public:
	MainFrame(const wxString& filePath, const wxString& title);

	



};

