#ifndef EXPORT_FORM_HPP
#define EXPORT_FORM_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "FileFormats/2da.hpp"
#include "FileFormats/Tlk.hpp"

class ExportForm : public wxDialog
{
public:
    ExportForm(wxWindow* parent);

    std::string GetProjectName();
    std::string GetBasePath();
    std::string GetTLKName();
private:
    void OnOk(wxCommandEvent& event);
    void OnFindDirectoryClick(wxCommandEvent& event);

    wxStaticText* project_name_label;
    wxTextCtrl* project_name;

    wxStaticText* destination_label;
    wxTextCtrl* destination;

    wxStaticText* tlk_filename_label;
    wxTextCtrl* tlk_filename;

    wxButton* destination_button;

    wxButton* ok_button;
    wxButton* cancel_button;

    wxDECLARE_EVENT_TABLE();
};

#endif
