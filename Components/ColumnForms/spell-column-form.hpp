#ifndef SPELL_COLUMN_FORM_HPP
#define SPELL_COLUMN_FORM_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "configuration-manager.hpp"
#include "FileFormats/2da.hpp"
#include "FileFormats/Tlk.hpp"

using namespace FileFormats;

class SpellColumnForm : public wxDialog
{
public:
    SpellColumnForm(wxWindow* parent, ConfigurationManager* _configuration);

private:
    void OnEnabledDoubleClick(wxCommandEvent& event);
    void OnDisabledDoubleClick(wxCommandEvent& event);
    void OnOk(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
    void SetInitialColumns();

    ConfigurationManager* configuration;

    wxListBox* enabled;
    wxListBox* disabled;

    wxStaticText* enabled_label;
    wxStaticText* disabled_label;

    wxButton* ok_button;
    wxButton* cancel_button;

    wxDECLARE_EVENT_TABLE();
};

#endif
