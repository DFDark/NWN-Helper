#ifndef SPELL_COLUMN_FORM_HPP
#define SPELL_COLUMN_FORM_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "../configuration-manager.hpp"
#include "FileFormats/2da.hpp"
#include "FileFormats/Tlk.hpp"

using namespace FileFormats;

class SpellColumnForm : public wxDialog
{
public:
    SpellColumnForm(wxWindow* parent, ConfigurationManager* _configuration);

private:
    void OnOk(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);

    ConfigurationManager* configuration;
    
    wxButton* ok_button;
    wxButton* cancel_button;

    wxDECLARE_EVENT_TABLE();
};

#endif
