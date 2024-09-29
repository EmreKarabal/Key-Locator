#include "MainFrame.h"
#include <wx/wx.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <wx/joystick.h>
#ifdef __WXMSW__
#include <windows.h>
#endif

std::vector<wxJoystick*> joyList;
wxArrayString keyLines;
wxArrayString dxLines;
std::string description;
std::string hexCode;
int modifier;
std::string complexModifier;
std::string callback;
std::string dxCode;
std::string dxModifier;
bool isPovHat;


wxListBox* listBox;
wxButton* button;
wxJoystick* joyStick;
bool keyMode = true;





std::unordered_map<int, std::string> modifierMap{

    {0,""},
    {1,"Shift"},
    {2,"Ctrl"},
    {3,"Ctrl + Shift"},
    {4,"Alt"},
    {5,"Alt + Shift"},
    {6,"Ctrl + Alt"},
    {7,"Ctrl + Shift + Alt"}

};


std::unordered_map<int, std::string> keyMap{

    //Null key
    {0xFF, "No Key Assigned"},
    // Function keys
    {0x3B, "F1"}, {0x3C, "F2"}, {0x3D, "F3"}, {0x3E, "F4"},
    {0x3F, "F5"}, {0x40, "F6"}, {0x41, "F7"}, {0x42, "F8"},
    {0x43, "F9"}, {0x44, "F10"}, {0x57, "F11"}, {0x58, "F12"},

    // Number keys
    {0x2, "1"}, {0x3, "2"}, {0x4, "3"}, {0x5, "4"},
    {0x6, "5"}, {0x7, "6"}, {0x8, "7"}, {0x9, "8"},
    {0xA, "9"}, {0xB, "0"},

    // Alphabet keys
    {0x10, "Q"}, {0x11, "W"}, {0x12, "E"}, {0x13, "R"}, {0x14, "T"},
    {0x15, "Y"}, {0x16, "U"}, {0x17, "I"}, {0x18, "O"}, {0x19, "P"},
    {0x1E, "A"}, {0x1F, "S"}, {0x20, "D"}, {0x21, "F"}, {0x22, "G"},
    {0x23, "H"}, {0x24, "J"}, {0x25, "K"}, {0x26, "L"},
    {0x2C, "Z"}, {0x2D, "X"}, {0x2E, "C"}, {0x2F, "V"}, {0x30, "B"},
    {0x31, "N"}, {0x32, "M"},

    // Special characters and symbols
    {0xC, "-"}, {0xD, "="}, {0x1A, "["}, {0x1B, "]"},
    {0x27, ";"}, {0x28, "'"}, {0x29, "`"}, {0x2B, "\\"},
    {0x33, ","}, {0x34, "."}, {0x35, "/"},

    // Modifier keys
    {0x1D, "Ctrl"}, {0x2A, "Shift"}, {0x36, "Right Shift"},
    {0x38, "Alt"}, {0x1C, "Enter"}, {0x39, "Space"},
    {0xF, "Tab"}, {0xE, "Backspace"}, {0x1, "Escape"},
    {0x3A, "Caps Lock"}, {0x45, "Num Lock"}, {0x46, "Scroll Lock"},

    // Arrow keys and navigation keys
    {0xC8, "Up Arrow"}, {0xD0, "Down Arrow"},
    {0xCB, "Left Arrow"}, {0xCD, "Right Arrow"},
    {0xC7, "Home"}, {0xCF, "End"}, {0xC9, "Page Up"},
    {0xD1, "Page Down"}, {0xD2, "Insert"}, {0xD3, "Delete"},

    // Numpad
    {0x52, "Numpad 0"}, {0x4F, "Numpad 1"}, {0x50, "Numpad 2"},
    {0x51, "Numpad 3"}, {0x4B, "Numpad 4"}, {0x4C, "Numpad 5"},
    {0x4D, "Numpad 6"}, {0x47, "Numpad 7"}, {0x48, "Numpad 8"},
    {0x49, "Numpad 9"}, {0x37, "Numpad *"}, {0x4A, "Numpad -"},
    {0x4E, "Numpad +"}, {0x53, "Numpad ."}, {0x9C, "Numpad Enter"}
};

std::unordered_map<std::string, std::string> callbackMap;



void process_line(const std::string& line) {

    

    size_t quoteStart = line.find_first_of('\"');
    size_t quoteEnd = line.find_last_of('\"');

    if (quoteStart != std::string::npos && quoteEnd != std::string::npos && quoteStart != quoteEnd) {

        std::vector<std::string> resultVector;

        // Extract the part within the quotes
        std::string quotedPart = line.substr(quoteStart, quoteEnd - quoteStart + 1);
        resultVector.insert(resultVector.begin(), quotedPart);

        // Get the part before the quoted text
        std::string preQuotedPart = line.substr(0, quoteStart - 1); // Strip trailing space before quotes

        // Tokenize the pre-quoted part by spaces
        std::stringstream ss(preQuotedPart);
        std::string word;
        while (ss >> word) {
            // Process each word as needed
            resultVector.push_back(word);
        }

        description = resultVector[0];
        callback = resultVector[1];
        hexCode = resultVector[4];
        modifier = std::stoi(resultVector[5]);
        complexModifier = resultVector[6];
        std::string strModifier;
        std::string strKey;

        if (hexCode == "0XFFFFFFFF") {
            hexCode = "0xFF";
        }

        std::string tempStr = "0x" + hexCode.substr(2,2);
        int hexValue = std::stoi(tempStr, nullptr, 16);

        strKey = keyMap[hexValue];

        if (complexModifier == "0X2E") {
            strModifier = "Alt + C:";
        }
        else {
            if (modifierMap[modifier] != "None") {
                strModifier = modifierMap[modifier];
            }
            else {
                strModifier = "";
            }
        }

        // map callback & description

        callbackMap.insert({ callback, description });

        


        std::string sortedLine = description + "   " + strModifier + "  " + strKey;

        keyLines.Add(sortedLine);

        resultVector.clear();


    }
    else {

        // This block handles non quoted parts so essentialy the lines that shows for the controller assignments.
        // I think this should be a seperate list or whatever in order to not mess with the keyListening & joyListening
        // This part so far only displays in the list

        std::vector<std::string> dxVector;

        std::string dxPart = line;

        std::stringstream ss(dxPart);
        std::string word;

       

        while (ss >> word) {
            dxVector.push_back(word);
        }

        if (dxVector.size() >= 3) {
            callback = dxVector.at(0);
            dxCode = dxVector.at(1);
            dxModifier = dxVector.at(2);
        }
        else {
            wxLogStatus("dxVector should have more than 3 elements!");
        }

        std::string print = callbackMap[callback] + "  " + dxCode;
        dxLines.Add(print);
    }
}




std::string splitKeyPart(std::string line) {

    size_t first = line.find_first_of('\"');
    size_t last = line.find_last_of('\"');

    if (first != std::string::npos && last != std::string::npos && first != last) {

        return line.substr(last + 1, line.size() - last);

    }

}


void findLine(std::string keyStr) {
   
    std::string line;
    std::string remainingPart;

    for (int i = 0; i < keyLines.size(); i++) {

        line = keyLines[i];
        remainingPart = splitKeyPart(line);

        remainingPart.erase(std::remove_if(remainingPart.begin(), remainingPart.end(), ::isspace), remainingPart.end());

        if (remainingPart == keyStr) {

            listBox->Select(i);
            wxLogStatus("%s", line);
            break;

        }
        


    }

   

}

void findLineDX(int buttonCode, int joyId) {
    
    std::vector<std::string> temp;
    std::string line;

    // Find which one is which
    if (joyId == 0) buttonCode += 128;

    for (int i = 0; i < dxLines.size(); i++) {
        
        line = splitKeyPart(dxLines[i].ToStdString());
        std::stringstream ss(line);
        std::string word;
        temp.clear();
        
        while (ss >> word) {

            temp.push_back(word);

        }
        
        if (temp.size() >= 2) {
            try {
                int checkCode = std::stoi(temp[1]);
                if (buttonCode == checkCode) {
                    listBox->Select(i);
                    wxLogStatus("%s", line);
                    break;
                }
            }
            catch (const std::exception e) {
                wxLogError("Error during conversion: %s", e.what());
            }

        }
        else {
            // this gets picked for whatever reason deal with this
            wxLogError("Invalid line format: %s", line);
        }


        temp.clear();

    }

}

void MainFrame::ResetModifiers()
{
    shift = false;
    ctrl = false;
    alt = false;
}



void MainFrame::readFile(std::string filePath)
{

    std::fstream file;
    std::string line;


    file.open(filePath, std::ios::in);

    if (file.is_open()) {

        while (!file.eof()) {

            std::getline(file, line);
            process_line(line);



        }


        file.close();


    }
}





bool isModifierKey(int keyCode) {
    return keyCode == WXK_SHIFT || keyCode == WXK_CONTROL || keyCode == WXK_ALT;
}







void MainFrame::OnKeyDown(wxKeyEvent& evt)
{
    if (keyMode) {

        int keyCode = evt.GetKeyCode();

        if (evt.ShiftDown()) shift = true;
        if (evt.AltDown()) alt = true;
        if (evt.ControlDown()) ctrl = true;

        if (!isModifierKey(keyCode)) {
            #ifdef __WXMSW__
            int virtualCode = evt.GetRawKeyCode();
            UINT scanCode = MapVirtualKey(virtualCode, MAPVK_VK_TO_VSC);

            #endif

            int count = 0;

            if (shift) count += 1;
            if (ctrl) count += 2;
            if (alt) count += 4;

            std::string modifiers = modifierMap[count];

            std::string finalKey = modifiers + keyMap[scanCode];

            finalKey.erase(std::remove_if(finalKey.begin(), finalKey.end(), ::isspace), finalKey.end());



            findLine(finalKey);

            ResetModifiers();

        }

        evt.Skip();

   }


}





void MainFrame::OnKeyUp(wxKeyEvent& evt) {
    if (keyMode) {

        int keyCode = evt.GetKeyCode();
        if (keyCode == WXK_SHIFT) shift = false;
        if (keyCode == WXK_ALT) alt = false;
        if (keyCode == WXK_CONTROL) ctrl = false;

        evt.Skip();


    }
    

}


void MainFrame::OnButtonClicked(wxCommandEvent& evt)
{
    if (keyMode) {
        keyMode = false;
    }
    else {
        keyMode = true;
    }

    wxLogStatus("");

    if (listBox->GetString(0) == keyLines[0]) {
        wxLogMessage("Switched to Controller Mode");
        listBox->Clear();
        listBox->Append(dxLines);
    }
    else if (listBox->GetString(0) == dxLines[0]) {
        wxLogMessage("Switched to Keyboard Mode");
        listBox->Clear();
        listBox->Append(keyLines);
    }
    listBox->SetFocus();
}

void MainFrame::OnJoyButtonDown(wxJoystickEvent& evt)
{
    if (!keyMode) {

        int joyId = evt.GetJoystick();
        int buttonCode = evt.GetButtonOrdinal();

        findLineDX(buttonCode, joyId);

    }

}









MainFrame::MainFrame(const wxString& filePath,const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {

 
    

    readFile(filePath.ToStdString());
    CreateStatusBar();

    wxPanel* panel = new wxPanel(this);
    button = new wxButton(panel, wxID_ANY, "Switch Mode", wxPoint(580, 80), wxSize(200, 40));
    listBox = new wxListBox(panel, wxID_ANY, wxPoint(50, 50), wxSize(500, 300), keyLines, wxWANTS_CHARS);
    
    
    


    Bind(wxEVT_JOY_BUTTON_DOWN, &MainFrame::OnJoyButtonDown, this);

    button->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClicked, this);
    listBox->Bind(wxEVT_KEY_DOWN, &MainFrame::OnKeyDown, this);
    listBox->Bind(wxEVT_KEY_UP, &MainFrame::OnKeyUp, this);

    int numberOfDevices = wxJoystick::GetNumberJoysticks();

    for (int i = 0; i < numberOfDevices; ++i) {

        wxJoystick* joystick = new wxJoystick(i);
        joystick->SetCapture(this, 10);
        joyList.push_back(joystick);

    }


    listBox->SetFocus();
    
    ResetModifiers();
	
	


	



}

MainFrame::~MainFrame()
{

    for (auto joystick : joyList) {

        joystick->ReleaseCapture();
        delete joystick;

    }


}


