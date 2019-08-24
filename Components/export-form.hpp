#ifndef EXPORT_FORM_HPP
#define EXPORT_FORM_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "configuration-manager.hpp"

#include "FileFormats/2da.hpp"
#include "FileFormats/Tlk.hpp"

class ExportForm : public wxDialog
{
public:
    ExportForm(wxWindow* parent, ConfigurationManager* _configuration);

private:
    void OnOk(wxCommandEvent& event);
    void OnFindDirectoryClick(wxCommandEvent& event);


    ConfigurationManager* configuration;

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
