#ifndef SPELL_FORM_HPP
#define SPELL_FORM_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "FileFormats/2da.hpp"

using namespace FileFormats;

class SpellForm : public wxDialog
{
public:
    SpellForm(wxWindow* parent, TwoDA::Friendly::TwoDARow* row);
    ~SpellForm();

private:
    void OnOk(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);

    TwoDA::Friendly::TwoDARow* spell;

    wxPanel* panel;

    wxButton* ok_button;
    wxButton* cancel_button;

    /*
    * Form components
    */
    wxStaticText* label_label;
    wxStaticText* name_label;
    wxStaticText* spellschool_label;
    wxStaticText* spellrange_label;

    wxTextCtrl* label;
    wxTextCtrl* name;

    wxComboBox* spell_school;
    wxComboBox* spell_range;

    wxDECLARE_EVENT_TABLE();
};

#endif
