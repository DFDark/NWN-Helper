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

    void InitFormValues();
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
    wxStaticText* impact_script_label;
    wxStaticText* spell_level_label_bard;
    wxStaticText* spell_level_label_cleric;
    wxStaticText* spell_level_label_druid;
    wxStaticText* spell_level_label_paladin;
    wxStaticText* spell_level_label_ranger;
    wxStaticText* spell_level_label_wiz_sorc;
    wxStaticText* spell_level_label_innate;

    wxStaticBox* spell_components;
    wxStaticBox* metamagic_staticbox;
    wxStaticBox* target_staticbox;
    wxStaticBox* spell_levels;

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

    wxSlider* spell_level_bard;
    wxSlider* spell_level_cleric;
    wxSlider* spell_level_druid;
    wxSlider* spell_level_paladin;
    wxSlider* spell_level_ranger;
    wxSlider* spell_level_wiz_sorc;
    wxSlider* spell_level_innate;

    wxDECLARE_EVENT_TABLE();
};

#endif
