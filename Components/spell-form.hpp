#ifndef SPELL_FORM_HPP
#define SPELL_FORM_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/tglbtn.h>

#include "FileFormats/2da.hpp"
#include "FileFormats/Tlk.hpp"

using namespace FileFormats;

class SpellForm : public wxDialog
{
public:
    SpellForm(wxWindow* parent, TwoDA::Friendly::TwoDARow* row, Tlk::Friendly::Tlk* _tlk);

private:
    void OnOk(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
    void SetSpellComponents();
    void SetSpellMetamagic();
    void SetSpellTargetType();

    int GetSchoolSelection();
    int GetRangeSelection();
    unsigned int GetUIntFromHex(const std::string& hex) const;

    TwoDA::Friendly::TwoDARow* spell;
    Tlk::Friendly::Tlk* tlk;

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
    wxTextCtrl* impact_script;

    wxComboBox* spell_school;
    wxComboBox* spell_range;

    wxToggleButton* verbal;
    wxToggleButton* somatic;

    wxToggleButton* metamagic_empower;
    wxToggleButton* metamagic_extend;
    wxToggleButton* metamagic_maximize;
    wxToggleButton* metamagic_quicken;
    wxToggleButton* metamagic_silent;
    wxToggleButton* metamagic_still;
    
    wxToggleButton* target_self;
    wxToggleButton* target_creature;
    wxToggleButton* target_areaground;
    wxToggleButton* target_items;
    wxToggleButton* target_doors;
    wxToggleButton* target_placeables;
    wxToggleButton* target_triggers;
    
    wxDECLARE_EVENT_TABLE();
};

#endif
