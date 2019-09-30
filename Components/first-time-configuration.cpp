#include "first-time-configuration.hpp"
#include <fstream>

wxBEGIN_EVENT_TABLE(FirstTimeConfiguration, wxDialog)
    EVT_BUTTON(MAIN_INI_BUTTON, FirstTimeConfiguration::OnMainIniClick)
    EVT_BUTTON(DATA_FOLDER_BUTTON, FirstTimeConfiguration::OnDataFolderClick)
    EVT_BUTTON(wxID_OK, FirstTimeConfiguration::OnOk)
wxEND_EVENT_TABLE()


FirstTimeConfiguration::FirstTimeConfiguration(const wxString& title, const wxSize& size)
    : wxDialog(NULL, wxID_ANY, title, wxDefaultPosition, size)
{
    panel = new wxPanel(this, wxID_ANY);

    data_folder_sb = new wxStaticBox(panel, wxID_ANY, wxString("Path to main data folder"));
    // data_folder_label = new wxStaticText(panel, wxID_ANY, wxString("Path to main data folder"));
    data_folder = new wxTextCtrl(data_folder_sb, wxID_ANY, wxString(""));
    data_folder_button = new wxButton(data_folder_sb, DATA_FOLDER_BUTTON, wxString("Find"));

    main_ini_sb = new wxStaticBox(panel, wxID_ANY, wxString("Path to nwn.ini file"));
    // main_ini_label = new wxStaticText(panel, wxID_ANY, wxString("Path to nwn.ini file"));
    main_ini = new wxTextCtrl(main_ini_sb, wxID_ANY, wxString(""));
    main_ini_button = new wxButton(main_ini_sb, MAIN_INI_BUTTON, wxString("Find"));

    ok_button = new wxButton(panel, wxID_OK, wxString("Ok"));
    cancel_button = new wxButton(panel, wxID_CANCEL, wxString("Cancel"));
    Centre();

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

    wxStaticBoxSizer* data_folder_sb_sizer = new wxStaticBoxSizer(data_folder_sb, wxHORIZONTAL);

    data_folder_sb_sizer->Add(data_folder, 1);
    data_folder_sb_sizer->Add(data_folder_button);

    wxStaticBoxSizer* main_ini_sb_sizer = new wxStaticBoxSizer(main_ini_sb, wxHORIZONTAL);

    main_ini_sb_sizer->Add(main_ini, 1);
    main_ini_sb_sizer->Add(main_ini_button);

    wxBoxSizer* control_button_sizer = new wxBoxSizer(wxHORIZONTAL);

    control_button_sizer->Add(cancel_button);
    control_button_sizer->Add(ok_button);

    main_sizer->Add(data_folder_sb_sizer, 0, wxEXPAND);
    main_sizer->Add(main_ini_sb_sizer, 1, wxEXPAND);
    main_sizer->Add(control_button_sizer, 0, wxALIGN_RIGHT|wxRIGHT|wxBOTTOM, 2);

    panel->SetSizer(main_sizer);
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
