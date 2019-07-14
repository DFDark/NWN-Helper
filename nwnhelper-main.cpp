#include "nwnhelper-main.hpp"
#include "spell-form.hpp"

wxBEGIN_EVENT_TABLE(NWNHelperMain, wxFrame)
    EVT_MENU(wxID_EXIT,  NWNHelperMain::OnExit)
    // EVT_MENU(wxID_ABOUT, NWNHelperMain::OnAbout)
    //EVT_MENU(2, NWNHelperMain::Test)
wxEND_EVENT_TABLE()

NWNHelperMain::NWNHelperMain(const wxString& title, const wxPoint& position, const wxSize& size) :
    wxFrame(NULL, wxID_ANY, title, position, size)
{
    menu_file = new wxMenu;
    menu_file->Append(2, "Spells", "sdfsdf");
    menu_file->Append(wxID_EXIT);// , "Exit", "Shuts down the application");
    menu_bar = new wxMenuBar;
    menu_bar->Append(menu_file, "&File");
    SetMenuBar(menu_bar);

    CreateStatusBar();
    SetStatusText("NWNHelperMain status bar");
}

NWNHelperMain::~NWNHelperMain()
{
    delete menu_file;
    delete menu_bar;

    menu_file = NULL;
    menu_bar = NULL;
}

void NWNHelperMain::OnExit(wxCommandEvent& event)
{
    Close(true);
}
