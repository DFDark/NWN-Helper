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

    void OnOk(wxCommandEvent& event);
private:
    ConfigurationManager* configuration;

    wxButton* ok_button;
    wxButton* cancel_button;

    wxDECLARE_EVENT_TABLE();
};

#endif