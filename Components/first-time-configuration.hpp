#ifndef FIRST_TIME_CONFIGURATION_HPP
#define FIRST_TIME_CONFIGURATION_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

enum
{
    MAIN_INI_BUTTON = 1,
    DATA_FOLDER_BUTTON,
};

class FirstTimeConfiguration : public wxDialog
{
public:
    FirstTimeConfiguration(const wxString& title, const wxSize& size);

    wxString GetDataFolderPath();
    wxString GetNWNIniPath();
private:
    void OnOk(wxCommandEvent& event);
    void OnMainIniClick(wxCommandEvent& event);
    void OnDataFolderClick(wxCommandEvent& event);


    wxPanel* panel;

    wxTextCtrl* data_folder;
    wxButton* data_folder_button;
    wxStaticText* data_folder_label;

    wxTextCtrl* main_ini;
    wxButton* main_ini_button;
    wxStaticText* main_ini_label;

    wxButton* ok_button;
    wxButton* cancel_button;

    wxDECLARE_EVENT_TABLE();
};

#endif
