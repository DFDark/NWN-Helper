#ifndef SPELL_FORM_HPP
#define SPELL_FORM_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/tglbtn.h>

#include "configuration-manager.hpp"

#include "FileFormats/2da.hpp"
#include "FileFormats/Tlk.hpp"

using namespace FileFormats;

class SpellForm : public wxDialog
{
public:
    SpellForm(wxWindow* parent, ConfigurationManager* _configuration, std::uint32_t row_id);

private:
    void OnOk(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
    void SetSpellComponents();
    void SetSpellMetamagic();
    void SetSpellTargetType();
    void SetSpellLevels();
    void SetConjValues();
    void SetCastValues();
    void SetProjectionValues();
    void SetMiscellaneousValues();
    void LoadCategoryValues();
    void LoadMasterSubSpells();

    void InitFormValues();
    int GetSchoolSelection();
    int GetRangeSelection();
    int GetConjAnimSelection();
    int GetCastAnimSelection();
    int GetProjTypeSelection();
    int GetProjSpawnPointSelection();
    int GetProjOrientationSelection();
    int GetImmunityTypeSelection();
    int GetUserTypeSelection();

    // From wx components to 2da format
    std::string GetSchoolSelectionString();
    std::string GetRangeSelectionString();
    std::string GetSpellComponentsString();
    std::string GetMetaMagicString();
    std::string GetTargetTypeString();
    std::string GetSpellLevelString(wxCheckBox* chkbox, wxSlider* slider);
    std::string GetConjAnimString();
    std::string GetCastAnimString();
    std::string GetProjectileTypeString();
    std::string GetProjectileSpawnString();
    std::string GetProjectileOrientationString();
    std::string GetImmunityTypeString();
    std::string GetUserTypeString();
    std::string GetStrref(wxTextCtrl* component, const auto& column);
    std::string GetFeatId();

    void OnInnateChange(wxCommandEvent& event);
    void OnBardChange(wxCommandEvent& event);
    void OnClericChange(wxCommandEvent& event);
    void OnDruidChange(wxCommandEvent& event);
    void OnPaladinChange(wxCommandEvent& event);
    void OnRangerChange(wxCommandEvent& event);
    void OnWizSorcChange(wxCommandEvent& event);

    void OnInnateCheck(wxCommandEvent& event);
    void OnBardCheck(wxCommandEvent& event);
    void OnClericCheck(wxCommandEvent& event);
    void OnDruidCheck(wxCommandEvent& event);
    void OnPaladinCheck(wxCommandEvent& event);
    void OnRangerCheck(wxCommandEvent& event);
    void OnWizSorcCheck(wxCommandEvent& event);

    void OnMasterClick(wxCommandEvent& event);
    void OnSubSpell1(wxCommandEvent& event);
    void OnSubSpell2(wxCommandEvent& event);
    void OnSubSpell3(wxCommandEvent& event);
    void OnSubSpell4(wxCommandEvent& event);
    void OnSubSpell5(wxCommandEvent& event);

    void OnFeatClick(wxCommandEvent& event);
    void OnCounterClick1(wxCommandEvent& event);
    void OnCounterClick2(wxCommandEvent& event);

    void InvokeSpellSelection(wxButton* button, std::uint32_t& variable);
    void SetSpellValue(wxButton* button, std::uint32_t& variable, const std::string& value);

    ConfigurationManager* configuration;
    TwoDA::Friendly::TwoDARow* spell;

    wxPanel* panel;

    wxButton* ok_button;
    wxButton* cancel_button;

    /*
    * Form components
    */
    wxStaticText* label_label;
    wxStaticText* name_label;
    wxStaticText* icon_resref_label;
    wxStaticText* spellschool_label;
    wxStaticText* spellrange_label;
    wxStaticText* impact_script_label;

    wxCheckBox* spell_level_checkbox_bard;
    wxCheckBox* spell_level_checkbox_cleric;
    wxCheckBox* spell_level_checkbox_druid;
    wxCheckBox* spell_level_checkbox_paladin;
    wxCheckBox* spell_level_checkbox_ranger;
    wxCheckBox* spell_level_checkbox_wiz_sorc;
    wxCheckBox* spell_level_checkbox_innate;
    wxCheckBox* projectile;
    wxCheckBox* has_projectile;
    wxCheckBox* item_immunity;
    wxCheckBox* use_concentration;
    wxCheckBox* spontaneous_cast;
    wxCheckBox* hostile_setting;

    wxStaticText* spell_level_label_val_innate;
    wxStaticText* spell_level_label_val_bard;
    wxStaticText* spell_level_label_val_cleric;
    wxStaticText* spell_level_label_val_druid;
    wxStaticText* spell_level_label_val_paladin;
    wxStaticText* spell_level_label_val_ranger;
    wxStaticText* spell_level_label_val_wiz_sorc;

    wxStaticText* conj_time_label;
    wxStaticText* conj_anim_label;
    wxStaticText* conj_head_visual_label;
    wxStaticText* conj_hand_visual_label;
    wxStaticText* conj_ground_visual_label;
    wxStaticText* conj_sound_vfx_label;
    wxStaticText* conj_sound_male_label;
    wxStaticText* conj_sound_female_label;
    wxStaticText* cast_time_label;
    wxStaticText* cast_anim_label;
    wxStaticText* cast_head_visual_label;
    wxStaticText* cast_hand_visual_label;
    wxStaticText* cast_ground_visual_label;
    wxStaticText* cast_sound_label;
    wxStaticText* projectile_label; // Needed as placeholder
    wxStaticText* projectile_model_label;
    wxStaticText* projectile_type_label;
    wxStaticText* projectile_spawn_point_label;
    wxStaticText* projectile_sound_label;
    wxStaticText* projectile_orientation_label;
    wxStaticText* immunity_type_label;
    wxStaticText* sub_rad_spell_1_label;
    wxStaticText* sub_rad_spell_2_label;
    wxStaticText* sub_rad_spell_3_label;
    wxStaticText* sub_rad_spell_4_label;
    wxStaticText* sub_rad_spell_5_label;
    wxStaticText* category_label;
    wxStaticText* master_label;
    wxStaticText* user_type_label;
    wxStaticText* description_label;
    wxStaticText* alt_message_label;
    wxStaticText* feat_label;
    wxStaticText* counter_1_label;
    wxStaticText* counter_2_label;


    wxStaticBox* spell_components;
    wxStaticBox* metamagic_staticbox;
    wxStaticBox* target_staticbox;
    wxStaticBox* spell_levels;
    wxStaticBox* conj_settings;
    wxStaticBox* cast_settings;
    wxStaticBox* proj_settings;
    wxStaticBox* master_sub_spells;

    wxTextCtrl* label;
    wxTextCtrl* name;
    wxTextCtrl* icon_resref;
    wxTextCtrl* impact_script;
    wxTextCtrl* conj_time;
    wxTextCtrl* conj_head_visual;
    wxTextCtrl* conj_hand_visual;
    wxTextCtrl* conj_ground_visual;
    wxTextCtrl* conj_sound_vfx;
    wxTextCtrl* conj_sound_male;
    wxTextCtrl* conj_sound_female;
    wxTextCtrl* cast_time;
    wxTextCtrl* cast_head_visual;
    wxTextCtrl* cast_hand_visual;
    wxTextCtrl* cast_ground_visual;
    wxTextCtrl* cast_sound;
    wxTextCtrl* projectile_sound;
    wxTextCtrl* projectile_model;
    wxTextCtrl* description;
    wxTextCtrl* alt_message;

    wxComboBox* spell_school;
    wxComboBox* spell_range;
    wxComboBox* conj_anim;
    wxComboBox* cast_anim;
    wxComboBox* projectile_type;
    wxComboBox* projectile_spawn_point;
    wxComboBox* projectile_orientation;
    wxComboBox* immunity_type;
    wxComboBox* user_type;
    wxComboBox* category;

    wxButton* feat;
    wxButton* counter_1;
    wxButton* counter_2;

    wxButton* master;
    wxButton* sub_rad_spell_1;
    wxButton* sub_rad_spell_2;
    wxButton* sub_rad_spell_3;
    wxButton* sub_rad_spell_4;
    wxButton* sub_rad_spell_5;

    std::uint32_t master_id;
    std::uint32_t sub_rad_spell_1_id;
    std::uint32_t sub_rad_spell_2_id;
    std::uint32_t sub_rad_spell_3_id;
    std::uint32_t sub_rad_spell_4_id;
    std::uint32_t sub_rad_spell_5_id;
    std::uint32_t feat_id;
    std::uint32_t counter_1_id;
    std::uint32_t counter_2_id;

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
