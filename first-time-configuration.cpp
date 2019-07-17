#include "first-time-configuration.hpp"
#include <fstream>

wxBEGIN_EVENT_TABLE(FirstTimeConfiguration, wxDialog)
    EVT_MENU(MAIN_INI_BUTTON, FirstTimeConfiguration::OnMainIniClick)
    EVT_MENU(DATA_FOLDER_BUTTON, FirstTimeConfiguration::OnDataFolderClick)
    EVT_MENU(wxID_OK, FirstTimeConfiguration::OnOk)
wxEND_EVENT_TABLE()


FirstTimeConfiguration::FirstTimeConfiguration(const wxString& title, const wxSize& size)
    : wxDialog(NULL, wxID_ANY, title, wxDefaultPosition, size)
{
    panel = new wxPanel(this, wxID_ANY);

    data_folder_label = new wxStaticText(panel, wxID_ANY, wxString("Path to main data folder"),
        wxPoint(11, 15), wxDefaultSize, wxALIGN_LEFT);
    data_folder = new wxTextCtrl(panel, wxID_ANY, wxString(""), wxPoint(10, 35), wxSize(340, 20));
    data_folder_button = new wxButton(panel, DATA_FOLDER_BUTTON, wxString("Find"), wxPoint(350, 30), wxSize(45, 29));
    Connect(DATA_FOLDER_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(FirstTimeConfiguration::OnDataFolderClick));

    main_ini_label = new wxStaticText(panel, wxID_ANY, wxString("Path to nwn.ini file"),
       wxPoint(11, 60), wxDefaultSize, wxALIGN_LEFT);
    main_ini = new wxTextCtrl(panel, wxID_ANY, wxString(""), wxPoint(10, 80), wxSize(340, 20));
    main_ini_button = new wxButton(panel, MAIN_INI_BUTTON, wxString("Find"), wxPoint(350, 75), wxSize(45, 29));
    Connect(MAIN_INI_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(FirstTimeConfiguration::OnMainIniClick));

    ok_button = new wxButton(panel, wxID_OK, wxString("Ok"), wxPoint(295, 135), wxSize(100, 30));
    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(FirstTimeConfiguration::OnOk));
    cancel_button = new wxButton(panel, wxID_CANCEL, wxString("Cancel"), wxPoint(190, 135), wxSize(100, 30));
    Centre();
}

FirstTimeConfiguration::~FirstTimeConfiguration()
{
    delete main_ini;
    delete main_ini_button;
    delete main_ini_label;
    delete data_folder_label;
    delete data_folder_button;
    delete data_folder;
    delete ok_button;
    delete panel;

    main_ini = NULL;
    main_ini_button = NULL;
    main_ini_label = NULL;
    data_folder_button = NULL;
    data_folder_label = NULL;
    data_folder = NULL;
    ok_button = NULL;
    panel = NULL;
}

void FirstTimeConfiguration::OnOk(wxCommandEvent& event)
{
    std::string datapath = data_folder->GetValue().ToStdString() + std::string("/nwn_base.key");
    std::string inipath = main_ini->GetValue().ToStdString();

    // As for validation, we will check by trying to open "nwn.ini" and DATA_FOLDER/"nwn_base.key"
    std::fstream nwnini(inipath, std::fstream::in);
    if (!nwnini.is_open())
    {
        wxMessageBox(std::string("Unable to open file \"") + inipath + std::string("\"!"),
            "Error", wxOK | wxICON_ERROR );
        return;
    }
    nwnini.close();

    std::fstream nwnkey(datapath, std::fstream::in);
    if (!nwnkey.is_open())
    {
        wxMessageBox(std::string("Unable to open file \"") + inipath + std::string("\"!"),
            "Error", wxOK | wxICON_ERROR );
        return;
    }
    nwnkey.close();

    this->EndModal(wxID_OK);
}

void FirstTimeConfiguration::OnMainIniClick(wxCommandEvent& event)
{
    wxFileDialog ini_dialog(panel, wxString("Locate nwn.ini file"), "", "",
        "*.ini", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (ini_dialog.ShowModal() == wxID_CANCEL)
        return;

    main_ini->SetValue(ini_dialog.GetPath());
}

void FirstTimeConfiguration::OnDataFolderClick(wxCommandEvent& event)
{
    wxDirDialog data_dialog(panel, wxString("Locate neverwinter installation data folder"), "",
        wxDD_DIR_MUST_EXIST);

    if (data_dialog.ShowModal() == wxID_CANCEL)
        return;

    data_folder->SetValue(data_dialog.GetPath());
}

wxString FirstTimeConfiguration::GetDataFolderPath()
{
    return data_folder->GetValue();
}

wxString FirstTimeConfiguration::GetNWNIniPath()
{
    return main_ini->GetValue();
}
