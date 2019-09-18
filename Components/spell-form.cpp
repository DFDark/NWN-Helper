#include "spell-form.hpp"
#include "constants.hpp"
#include "functions.hpp"
#include "spell-selection-form.hpp"
#include "feat-selection-form.hpp"

enum
{
    SP_SLIDER_INNATE = wxID_HIGHEST + 1,
    SP_SLIDER_BARD,
    SP_SLIDER_CLERIC,
    SP_SLIDER_DRUID,
    SP_SLIDER_PALADIN,
    SP_SLIDER_RANGER,
    SP_SLIDER_WIZ_SORC,
    SP_CHECKBOX_INNATE,
    SP_CHECKBOX_BARD,
    SP_CHECKBOX_CLERIC,
    SP_CHECKBOX_DRUID,
    SP_CHECKBOX_PALADIN,
    SP_CHECKBOX_RANGER,
    SP_CHECKBOX_WIZ_SORC,
    SP_BUTTON_MASTER,
    SP_BUTTON_SUB_SPELL_1,
    SP_BUTTON_SUB_SPELL_2,
    SP_BUTTON_SUB_SPELL_3,
    SP_BUTTON_SUB_SPELL_4,
    SP_BUTTON_SUB_SPELL_5,
    SP_BUTTON_FEAT,
    SP_BUTTON_COUNTER_1,
    SP_BUTTON_COUNTER_2
};

wxBEGIN_EVENT_TABLE(SpellForm, wxDialog)
    EVT_MENU(wxID_OK, SpellForm::OnOk)
    EVT_MENU(wxID_CANCEL, SpellForm::OnCancel)
    EVT_SLIDER(SP_SLIDER_INNATE, SpellForm::OnInnateChange)
    EVT_SLIDER(SP_SLIDER_BARD, SpellForm::OnBardChange)
    EVT_SLIDER(SP_SLIDER_CLERIC, SpellForm::OnClericChange)
    EVT_SLIDER(SP_SLIDER_DRUID, SpellForm::OnDruidChange)
    EVT_SLIDER(SP_SLIDER_PALADIN, SpellForm::OnPaladinChange)
    EVT_SLIDER(SP_SLIDER_RANGER, SpellForm::OnRangerChange)
    EVT_SLIDER(SP_SLIDER_WIZ_SORC, SpellForm::OnWizSorcChange)
    EVT_CHECKBOX(SP_CHECKBOX_INNATE, SpellForm::OnInnateCheck)
    EVT_CHECKBOX(SP_CHECKBOX_BARD, SpellForm::OnBardCheck)
    EVT_CHECKBOX(SP_CHECKBOX_CLERIC, SpellForm::OnClericCheck)
    EVT_CHECKBOX(SP_CHECKBOX_DRUID, SpellForm::OnDruidCheck)
    EVT_CHECKBOX(SP_CHECKBOX_PALADIN, SpellForm::OnPaladinCheck)
    EVT_CHECKBOX(SP_CHECKBOX_RANGER, SpellForm::OnRangerCheck)
    EVT_CHECKBOX(SP_CHECKBOX_WIZ_SORC, SpellForm::OnWizSorcCheck)
    EVT_BUTTON(SP_BUTTON_MASTER, SpellForm::OnMasterClick)
    EVT_BUTTON(SP_BUTTON_SUB_SPELL_1, SpellForm::OnSubSpell1)
    EVT_BUTTON(SP_BUTTON_SUB_SPELL_2, SpellForm::OnSubSpell2)
    EVT_BUTTON(SP_BUTTON_SUB_SPELL_3, SpellForm::OnSubSpell3)
    EVT_BUTTON(SP_BUTTON_SUB_SPELL_4, SpellForm::OnSubSpell4)
    EVT_BUTTON(SP_BUTTON_SUB_SPELL_5, SpellForm::OnSubSpell5)
    EVT_BUTTON(SP_BUTTON_FEAT, SpellForm::OnFeatClick)
    EVT_BUTTON(SP_BUTTON_COUNTER_1, SpellForm::OnCounterClick1)
    EVT_BUTTON(SP_BUTTON_COUNTER_2, SpellForm::OnCounterClick2)
wxEND_EVENT_TABLE()

SpellForm::SpellForm(wxWindow* parent, ConfigurationManager* _configuration, std::uint32_t row_id)
    : wxDialog(parent, wxID_ANY, wxString("Spell Form"), wxDefaultPosition, wxSize(1024, 768))
{
    panel = new wxPanel(this, wxID_ANY);
    configuration = _configuration;
    spell = configuration->Get2daRow("spells", row_id);

    /*
    * FORM LABELS
    */
    label_label = new wxStaticText(panel, wxID_ANY, wxString("Label:"));
    name_label = new wxStaticText(panel, wxID_ANY, wxString("Name:"));
    icon_resref_label = new wxStaticText(panel, wxID_ANY, wxString("Icon ResRef:"));
    spellschool_label = new wxStaticText(panel, wxID_ANY, wxString("Spell School:"));
    spellrange_label = new wxStaticText(panel, wxID_ANY, wxString("Spell Range:"));
    impact_script_label = new wxStaticText(panel, wxID_ANY, wxString("Impact Script:"));

    /*
    * FORM TEXT CONTROLS
    */
    label = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    name = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    icon_resref = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    impact_script = new wxTextCtrl(panel, wxID_ANY, wxString(""));

    /*
    * FORM SELECT
    */
    spell_school = new wxComboBox(panel, wxID_ANY, wxString(""));
    spell_range = new wxComboBox(panel, wxID_ANY, wxString(""));

    spell_components = new wxStaticBox(panel, wxID_ANY, wxString("Spell Components"));
    metamagic_staticbox = new wxStaticBox(panel, wxID_ANY, wxString("Metamagic"));
    target_staticbox = new wxStaticBox(panel, wxID_ANY, wxString("Target"));
    spell_levels = new wxStaticBox(panel, wxID_ANY, wxString("Spell levels"));
    conj_settings = new wxStaticBox(panel, wxID_ANY, wxString("Conjuration settings"));
    cast_settings = new wxStaticBox(panel, wxID_ANY, wxString("Cast settings"));
    proj_settings = new wxStaticBox(panel, wxID_ANY, wxString("Projectile settings"));
    master_sub_spells = new wxStaticBox(panel, wxID_ANY, wxString("Master/Sub spells"));

    verbal = new wxToggleButton(spell_components, wxID_ANY, wxString("Verbal"));
    somatic = new wxToggleButton(spell_components, wxID_ANY, wxString("Somatic"));

    /*
    * MetaMagic components
    */
    metamagic_empower = new wxToggleButton(metamagic_staticbox, wxID_ANY, wxString("Empower"), wxDefaultPosition, wxDefaultSize);
    metamagic_extend = new wxToggleButton(metamagic_staticbox, wxID_ANY, wxString("Extend"), wxDefaultPosition, wxDefaultSize);
    metamagic_maximize = new wxToggleButton(metamagic_staticbox, wxID_ANY, wxString("Maximize"), wxDefaultPosition, wxDefaultSize);
    metamagic_quicken = new wxToggleButton(metamagic_staticbox, wxID_ANY, wxString("Quicken"), wxDefaultPosition, wxDefaultSize);
    metamagic_silent = new wxToggleButton(metamagic_staticbox, wxID_ANY, wxString("Silent"), wxDefaultPosition, wxDefaultSize);
    metamagic_still = new wxToggleButton(metamagic_staticbox, wxID_ANY, wxString("Still"), wxDefaultPosition, wxDefaultSize);

    /*
    * Target components
    */
    target_self = new wxToggleButton(target_staticbox, wxID_ANY, wxString("Self"));
    target_creature = new wxToggleButton(target_staticbox, wxID_ANY, wxString("Creature"));
    target_areaground = new wxToggleButton(target_staticbox, wxID_ANY, wxString("Area/Ground"));
    target_items = new wxToggleButton(target_staticbox, wxID_ANY, wxString("Items"));
    target_doors = new wxToggleButton(target_staticbox, wxID_ANY, wxString("Doors"));
    target_placeables = new wxToggleButton(target_staticbox, wxID_ANY, wxString("Placeables"));
    target_triggers = new wxToggleButton(target_staticbox, wxID_ANY, wxString("Triggers"));

    /*
    * Spell Level components
    */
    spell_level_checkbox_bard = new wxCheckBox(spell_levels, SP_CHECKBOX_BARD, wxString("Bard:      "));
    spell_level_checkbox_cleric = new wxCheckBox(spell_levels, SP_CHECKBOX_CLERIC, wxString("Cleric:    "));
    spell_level_checkbox_druid = new wxCheckBox(spell_levels, SP_CHECKBOX_DRUID, wxString("Druid:     "));
    spell_level_checkbox_paladin = new wxCheckBox(spell_levels, SP_CHECKBOX_PALADIN, wxString("Paladin:   "));
    spell_level_checkbox_ranger = new wxCheckBox(spell_levels, SP_CHECKBOX_RANGER, wxString("Ranger:    "));
    spell_level_checkbox_wiz_sorc = new wxCheckBox(spell_levels, SP_CHECKBOX_WIZ_SORC, wxString("Wiz./Sorc.:"));
    spell_level_checkbox_innate = new wxCheckBox(spell_levels, SP_CHECKBOX_INNATE, wxString("Innate:    "));

    spell_level_label_val_innate = new wxStaticText(spell_levels, wxID_ANY, wxString("0"));
    spell_level_label_val_bard = new wxStaticText(spell_levels, wxID_ANY, wxString("0"));
    spell_level_label_val_cleric = new wxStaticText(spell_levels, wxID_ANY, wxString("0"));
    spell_level_label_val_druid = new wxStaticText(spell_levels, wxID_ANY, wxString("0"));
    spell_level_label_val_paladin = new wxStaticText(spell_levels, wxID_ANY, wxString("0"));
    spell_level_label_val_ranger = new wxStaticText(spell_levels, wxID_ANY, wxString("0"));
    spell_level_label_val_wiz_sorc = new wxStaticText(spell_levels, wxID_ANY, wxString("0"));

    spell_level_bard = new wxSlider(spell_levels, SP_SLIDER_BARD, 0, 0, 9);
    spell_level_cleric = new wxSlider(spell_levels, SP_SLIDER_CLERIC, 0, 0, 9);
    spell_level_druid = new wxSlider(spell_levels, SP_SLIDER_DRUID, 0, 0, 9);
    spell_level_paladin = new wxSlider(spell_levels, SP_SLIDER_PALADIN, 0, 0, 9);
    spell_level_ranger = new wxSlider(spell_levels, SP_SLIDER_RANGER, 0, 0, 9);
    spell_level_wiz_sorc = new wxSlider(spell_levels, SP_SLIDER_WIZ_SORC, 0, 0, 9);
    spell_level_innate = new wxSlider(spell_levels, SP_SLIDER_INNATE, 0, 0, 9);

    /*
    * Spell Conjuration components
    */
    conj_time_label = new wxStaticText(conj_settings, wxID_ANY, wxString("Time"));
    conj_anim_label = new wxStaticText(conj_settings, wxID_ANY, wxString("Animation"));
    conj_head_visual_label = new wxStaticText(conj_settings, wxID_ANY, wxString("Head visual"));
    conj_hand_visual_label = new wxStaticText(conj_settings, wxID_ANY, wxString("Hand visual"));
    conj_ground_visual_label = new wxStaticText(conj_settings, wxID_ANY, wxString("Ground visual"));
    conj_sound_vfx_label = new wxStaticText(conj_settings, wxID_ANY, wxString("Sound VFX"));
    conj_sound_male_label = new wxStaticText(conj_settings, wxID_ANY, wxString("Sound Male"));
    conj_sound_female_label = new wxStaticText(conj_settings, wxID_ANY, wxString("Sound Female"));

    conj_time = new wxTextCtrl(conj_settings, wxID_ANY, wxString(""));
    conj_anim = new wxComboBox(conj_settings, wxID_ANY, wxString(""));
    conj_head_visual = new wxTextCtrl(conj_settings, wxID_ANY, wxString(""));
    conj_hand_visual = new wxTextCtrl(conj_settings, wxID_ANY, wxString(""));
    conj_ground_visual = new wxTextCtrl(conj_settings, wxID_ANY, wxString(""));
    conj_sound_vfx = new wxTextCtrl(conj_settings, wxID_ANY, wxString(""));
    conj_sound_male = new wxTextCtrl(conj_settings, wxID_ANY, wxString(""));
    conj_sound_female = new wxTextCtrl(conj_settings, wxID_ANY, wxString(""));

    /*
    * Spell Cast components
    */
    cast_time_label = new wxStaticText(cast_settings, wxID_ANY, wxString("Time"));
    cast_anim_label = new wxStaticText(cast_settings, wxID_ANY, wxString("Animation"));
    cast_head_visual_label = new wxStaticText(cast_settings, wxID_ANY, wxString("Head visual"));
    cast_hand_visual_label = new wxStaticText(cast_settings, wxID_ANY, wxString("Hand visual"));
    cast_ground_visual_label = new wxStaticText(cast_settings, wxID_ANY, wxString("Ground visual"));
    cast_sound_label = new wxStaticText(cast_settings, wxID_ANY, wxString("Sound"));

    cast_time = new wxTextCtrl(cast_settings, wxID_ANY, wxString(""));
    cast_anim = new wxComboBox(cast_settings, wxID_ANY, wxString(""));
    cast_head_visual = new wxTextCtrl(cast_settings, wxID_ANY, wxString(""));
    cast_hand_visual = new wxTextCtrl(cast_settings, wxID_ANY, wxString(""));
    cast_ground_visual = new wxTextCtrl(cast_settings, wxID_ANY, wxString(""));
    cast_sound = new wxTextCtrl(cast_settings, wxID_ANY, wxString(""));

    /*
    * Projectile components
    */
    projectile_label = new wxStaticText(proj_settings, wxID_ANY, wxString(""));
    projectile_model_label = new wxStaticText(proj_settings, wxID_ANY, wxString("Model"));
    projectile_type_label = new wxStaticText(proj_settings, wxID_ANY, wxString("Type"));
    projectile_spawn_point_label = new wxStaticText(proj_settings, wxID_ANY, wxString("Spawn point"));
    projectile_sound_label = new wxStaticText(proj_settings, wxID_ANY, wxString("Sound"));
    projectile_orientation_label = new wxStaticText(proj_settings, wxID_ANY, wxString("Orientation"));

    projectile = new wxCheckBox(proj_settings, wxID_ANY, wxString("Is Projectile"));
    projectile_model = new wxTextCtrl(proj_settings, wxID_ANY, wxString(""));
    projectile_type = new wxComboBox(proj_settings, wxID_ANY, wxString(""));
    projectile_spawn_point = new wxComboBox(proj_settings, wxID_ANY, wxString(""));
    projectile_sound = new wxTextCtrl(proj_settings, wxID_ANY, wxString(""));
    projectile_orientation = new wxComboBox(proj_settings, wxID_ANY, wxString(""));
    has_projectile = new wxCheckBox(proj_settings, wxID_ANY, wxString("Has Projectile"));

    /*
     * Spell settings components (various)
    */
    category_label = new wxStaticText(panel, wxID_ANY, wxString("Category"));
    immunity_type_label = new wxStaticText(panel, wxID_ANY, wxString("Immunity type"));

    category = new wxComboBox(panel, wxID_ANY, wxString(""));
    immunity_type = new wxComboBox(panel, wxID_ANY, wxString(""));
    item_immunity = new wxCheckBox(panel, wxID_ANY, wxString("Item Immunity"));
    use_concentration = new wxCheckBox(panel, wxID_ANY, wxString("Use concentration"));
    spontaneous_cast = new wxCheckBox(panel, wxID_ANY, wxString("Spontaneous cast"));
    hostile_setting = new wxCheckBox(panel, wxID_ANY, wxString("Hostile"));

    sub_rad_spell_1_label = new wxStaticText(panel, wxID_ANY, wxString("SubRad Spell 1"));
    sub_rad_spell_2_label = new wxStaticText(panel, wxID_ANY, wxString("SubRad Spell 2"));
    sub_rad_spell_3_label = new wxStaticText(panel, wxID_ANY, wxString("SubRad Spell 3"));
    sub_rad_spell_4_label = new wxStaticText(panel, wxID_ANY, wxString("SubRad Spell 4"));
    sub_rad_spell_5_label = new wxStaticText(panel, wxID_ANY, wxString("SubRad Spell 5"));

    master = new wxButton(panel, SP_BUTTON_MASTER, wxString("None"));
    sub_rad_spell_1 = new wxButton(panel, SP_BUTTON_SUB_SPELL_1, wxString("None"));
    sub_rad_spell_2 = new wxButton(panel, SP_BUTTON_SUB_SPELL_2, wxString("None"));
    sub_rad_spell_3 = new wxButton(panel, SP_BUTTON_SUB_SPELL_3, wxString("None"));
    sub_rad_spell_4 = new wxButton(panel, SP_BUTTON_SUB_SPELL_4, wxString("None"));
    sub_rad_spell_5 = new wxButton(panel, SP_BUTTON_SUB_SPELL_5, wxString("None"));

    master_label = new wxStaticText(panel, wxID_ANY, wxString("Master"));
    user_type_label = new wxStaticText(panel, wxID_ANY, wxString("User Type"));
    description_label = new wxStaticText(panel, wxID_ANY, wxString("Description"));
    alt_message_label = new wxStaticText(panel, wxID_ANY, wxString("Alt. Message"));
    feat_label = new wxStaticText(panel, wxID_ANY, wxString("Feat"));
    counter_1_label = new wxStaticText(panel, wxID_ANY, wxString("Counter 1"));
    counter_2_label = new wxStaticText(panel, wxID_ANY, wxString("Counter 2"));

    user_type = new wxComboBox(panel, wxID_ANY, wxString(""));
    description = new wxTextCtrl(panel, wxID_ANY, wxString(""), wxDefaultPosition, wxSize(450, -1), wxTE_MULTILINE);
    alt_message = new wxTextCtrl(panel, wxID_ANY, wxString(""));

    feat = new wxButton(panel, SP_BUTTON_FEAT, wxString("None"));
    counter_1 = new wxButton(panel, SP_BUTTON_COUNTER_1, wxString("None"));
    counter_2 = new wxButton(panel, SP_BUTTON_COUNTER_2, wxString("None"));

    ok_button = new wxButton(panel, wxID_OK, wxString("Ok"), wxPoint(695, 535), wxSize(100, 30));
    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SpellForm::OnOk));

    cancel_button = new wxButton(panel, wxID_CANCEL, wxString("Cancel"), wxPoint(590, 535), wxSize(100, 30));

    Centre();

    // Sizers
    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* first_row_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* second_row_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* third_row_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* fourth_row_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* fifth_row_sizer = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer* second_row_sizer_p1 = new wxBoxSizer(wxHORIZONTAL);
    wxStaticBoxSizer* second_row_sizer_p2 = new wxStaticBoxSizer(spell_levels, wxVERTICAL);

    wxBoxSizer* label_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* name_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* school_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* range_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* icon_resref_sizer = new wxBoxSizer(wxVERTICAL);

    label_sizer->Add(label_label);
    label_sizer->Add(label, 1, wxEXPAND|wxALL);
    name_sizer->Add(name_label);
    name_sizer->Add(name, 1, wxEXPAND|wxALL);
    school_sizer->Add(spellschool_label);
    school_sizer->Add(spell_school, 1, wxEXPAND|wxALL);
    range_sizer->Add(spellrange_label);
    range_sizer->Add(spell_range, 1, wxEXPAND|wxALL);
    icon_resref_sizer->Add(icon_resref_label);
    icon_resref_sizer->Add(icon_resref, 1, wxEXPAND);

    first_row_sizer->Add(label_sizer, 1, wxEXPAND|wxALL);
    first_row_sizer->Add(name_sizer, 1, wxEXPAND|wxALL);
    first_row_sizer->Add(school_sizer, 1, wxEXPAND|wxALL);
    first_row_sizer->Add(range_sizer, 1, wxEXPAND|wxALL);
    first_row_sizer->Add(icon_resref_sizer);

    wxStaticBoxSizer* meta_sizer = new wxStaticBoxSizer(metamagic_staticbox, wxVERTICAL);

    meta_sizer->Add(metamagic_empower);
    meta_sizer->Add(metamagic_extend);
    meta_sizer->Add(metamagic_maximize);
    meta_sizer->Add(metamagic_quicken);
    meta_sizer->Add(metamagic_silent);
    meta_sizer->Add(metamagic_still);

    wxStaticBoxSizer* target_sizer = new wxStaticBoxSizer(target_staticbox, wxVERTICAL);

    target_sizer->Add(target_self);
    target_sizer->Add(target_creature);
    target_sizer->Add(target_areaground);
    target_sizer->Add(target_items);
    target_sizer->Add(target_doors);
    target_sizer->Add(target_placeables);
    target_sizer->Add(target_triggers);

    wxBoxSizer* spell_desc_sizer = new wxBoxSizer(wxVERTICAL);

    spell_desc_sizer->Add(description_label);
    spell_desc_sizer->Add(description, 1, wxEXPAND|wxALL);

    wxBoxSizer* spell_settings = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* immunity_type_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* item_immunity_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* user_type_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* cast_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* impact_script_sizer = new wxBoxSizer(wxVERTICAL);

    item_immunity_sizer->Add(item_immunity);
    item_immunity_sizer->Add(hostile_setting);
    cast_sizer->Add(use_concentration);
    cast_sizer->Add(spontaneous_cast);
    user_type_sizer->Add(user_type_label);
    user_type_sizer->Add(user_type, 1, wxEXPAND|wxALL);
    immunity_type_sizer->Add(immunity_type_label);
    immunity_type_sizer->Add(immunity_type, 1, wxEXPAND|wxALL);
    impact_script_sizer->Add(impact_script_label);
    impact_script_sizer->Add(impact_script, 1, wxEXPAND|wxALL);

    spell_settings->Add(immunity_type_sizer, 0, wxEXPAND);
    spell_settings->Add(impact_script_sizer, 0, wxEXPAND);
    spell_settings->Add(user_type_sizer, 0, wxEXPAND);
    spell_settings->Add(item_immunity_sizer, 0, wxEXPAND);
    spell_settings->Add(cast_sizer, 0, wxEXPAND);

    second_row_sizer_p1->Add(meta_sizer);
    second_row_sizer_p1->Add(target_sizer);
    second_row_sizer_p1->Add(spell_settings);
    second_row_sizer_p1->Add(spell_desc_sizer, 1, wxEXPAND);
    second_row_sizer->Add(second_row_sizer_p1, 1, wxEXPAND);

    wxBoxSizer* sp_levels_innate_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sp_levels_bard_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sp_levels_cleric_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sp_levels_druid_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sp_levels_paladin_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sp_levels_ranger_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sp_levels_wiz_sorc_sizer = new wxBoxSizer(wxHORIZONTAL);

    sp_levels_innate_sizer->Add(spell_level_checkbox_innate);
    sp_levels_innate_sizer->Add(spell_level_label_val_innate);
    sp_levels_innate_sizer->Add(spell_level_innate, 1, wxALL);
    sp_levels_bard_sizer->Add(spell_level_checkbox_bard);
    sp_levels_bard_sizer->Add(spell_level_label_val_bard);
    sp_levels_bard_sizer->Add(spell_level_bard, 1, wxALL);
    sp_levels_cleric_sizer->Add(spell_level_checkbox_cleric);
    sp_levels_cleric_sizer->Add(spell_level_label_val_cleric);
    sp_levels_cleric_sizer->Add(spell_level_cleric, 1, wxALL);
    sp_levels_druid_sizer->Add(spell_level_checkbox_druid);
    sp_levels_druid_sizer->Add(spell_level_label_val_druid);
    sp_levels_druid_sizer->Add(spell_level_druid, 1, wxALL);
    sp_levels_paladin_sizer->Add(spell_level_checkbox_paladin);
    sp_levels_paladin_sizer->Add(spell_level_label_val_paladin);
    sp_levels_paladin_sizer->Add(spell_level_paladin, 1, wxALL);
    sp_levels_ranger_sizer->Add(spell_level_checkbox_ranger);
    sp_levels_ranger_sizer->Add(spell_level_label_val_ranger);
    sp_levels_ranger_sizer->Add(spell_level_ranger, 1, wxALL);
    sp_levels_wiz_sorc_sizer->Add(spell_level_checkbox_wiz_sorc);
    sp_levels_wiz_sorc_sizer->Add(spell_level_label_val_wiz_sorc);
    sp_levels_wiz_sorc_sizer->Add(spell_level_wiz_sorc, 1, wxALL);

    second_row_sizer_p2->Add(sp_levels_innate_sizer, 1, wxEXPAND|wxALL);
    second_row_sizer_p2->Add(sp_levels_bard_sizer, 1,  wxEXPAND|wxALL);
    second_row_sizer_p2->Add(sp_levels_cleric_sizer, 1, wxEXPAND|wxALL);
    second_row_sizer_p2->Add(sp_levels_druid_sizer, 1, wxEXPAND|wxALL);
    second_row_sizer_p2->Add(sp_levels_paladin_sizer, 1, wxEXPAND|wxALL);
    second_row_sizer_p2->Add(sp_levels_ranger_sizer, 1, wxEXPAND|wxALL);
    second_row_sizer_p2->Add(sp_levels_wiz_sorc_sizer, 1, wxEXPAND|wxALL);

    second_row_sizer->Add(second_row_sizer_p2, 1, wxEXPAND|wxALL, 5);

    wxBoxSizer* conj_time_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* conj_anim_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* conj_head_visual_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* conj_hand_visual_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* conj_ground_visual_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* conj_sound_vfx_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* conj_sound_male_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* conj_sound_female_sizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* conj_settings_sizer_r1 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* conj_settings_sizer_r2 = new wxBoxSizer(wxHORIZONTAL);

    wxStaticBoxSizer* conj_settings_sizer = new wxStaticBoxSizer(conj_settings, wxHORIZONTAL);

    conj_time_sizer->Add(conj_time_label);
    conj_time_sizer->Add(conj_time, 1, wxEXPAND|wxALL);
    conj_anim_sizer->Add(conj_anim_label);
    conj_anim_sizer->Add(conj_anim, 1, wxEXPAND|wxALL);
    conj_head_visual_sizer->Add(conj_head_visual_label);
    conj_head_visual_sizer->Add(conj_head_visual, 1, wxEXPAND|wxALL);
    conj_hand_visual_sizer->Add(conj_hand_visual_label);
    conj_hand_visual_sizer->Add(conj_hand_visual, 1, wxEXPAND|wxALL);
    conj_ground_visual_sizer->Add(conj_ground_visual_label);
    conj_ground_visual_sizer->Add(conj_ground_visual, 1, wxEXPAND|wxALL);
    conj_sound_vfx_sizer->Add(conj_sound_vfx_label);
    conj_sound_vfx_sizer->Add(conj_sound_vfx, 1, wxEXPAND|wxALL);
    conj_sound_male_sizer->Add(conj_sound_male_label);
    conj_sound_male_sizer->Add(conj_sound_male, 1, wxEXPAND|wxALL);
    conj_sound_female_sizer->Add(conj_sound_female_label);
    conj_sound_female_sizer->Add(conj_sound_female, 1, wxEXPAND|wxALL);

    conj_settings_sizer_r1->Add(conj_time_sizer, 1, wxEXPAND|wxALL);
    conj_settings_sizer_r1->Add(conj_sound_vfx_sizer, 1, wxEXPAND|wxALL);
    conj_settings_sizer_r1->Add(conj_sound_male_sizer, 1, wxEXPAND|wxALL);
    conj_settings_sizer_r1->Add(conj_sound_female_sizer, 1, wxEXPAND|wxALL);
    conj_settings_sizer_r2->Add(conj_anim_sizer, 1, wxEXPAND|wxALL);
    conj_settings_sizer_r2->Add(conj_head_visual_sizer, 1, wxEXPAND|wxALL);
    conj_settings_sizer_r2->Add(conj_hand_visual_sizer, 1, wxEXPAND|wxALL);
    conj_settings_sizer_r2->Add(conj_ground_visual_sizer, 1, wxEXPAND|wxALL);

    conj_settings_sizer->Add(conj_settings_sizer_r1, 1, wxEXPAND);
    conj_settings_sizer->Add(conj_settings_sizer_r2, 1, wxEXPAND);

    wxBoxSizer* cast_anim_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* cast_time_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* cast_head_visual_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* cast_hand_visual_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* cast_ground_visual_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* cast_sound_sizer = new wxBoxSizer(wxVERTICAL);

    wxStaticBoxSizer* cast_settings_sizer = new wxStaticBoxSizer(cast_settings, wxHORIZONTAL);

    cast_time_sizer->Add(cast_time_label);
    cast_time_sizer->Add(cast_time, 1, wxEXPAND|wxALL);
    cast_anim_sizer->Add(cast_anim_label);
    cast_anim_sizer->Add(cast_anim, 1, wxEXPAND|wxALL);
    cast_head_visual_sizer->Add(cast_head_visual_label);
    cast_head_visual_sizer->Add(cast_head_visual, 1, wxEXPAND|wxALL);
    cast_hand_visual_sizer->Add(cast_hand_visual_label);
    cast_hand_visual_sizer->Add(cast_hand_visual, 1, wxEXPAND|wxALL);
    cast_ground_visual_sizer->Add(cast_ground_visual_label);
    cast_ground_visual_sizer->Add(cast_ground_visual, 1, wxEXPAND|wxALL);
    cast_sound_sizer->Add(cast_sound_label);
    cast_sound_sizer->Add(cast_sound, 1, wxEXPAND|wxALL);

    cast_settings_sizer->Add(cast_time_sizer, 1, wxEXPAND);
    cast_settings_sizer->Add(cast_anim_sizer, 1, wxEXPAND);
    cast_settings_sizer->Add(cast_head_visual_sizer, 1, wxEXPAND);
    cast_settings_sizer->Add(cast_hand_visual_sizer, 1, wxEXPAND);
    cast_settings_sizer->Add(cast_ground_visual_sizer, 1, wxEXPAND);
    cast_settings_sizer->Add(cast_sound_sizer, 1, wxEXPAND);

    third_row_sizer->Add(cast_settings_sizer, 1, wxEXPAND);

    wxBoxSizer* projectile_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* projectile_model_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* projectile_type_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* projectile_spawn_point_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* projectile_sound_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* projectile_orientation_sizer = new wxBoxSizer(wxVERTICAL);

    wxStaticBoxSizer* proj_settings_sizer = new wxStaticBoxSizer(proj_settings, wxHORIZONTAL);

    projectile_sizer->Add(projectile, 1, wxEXPAND|wxALL);
    projectile_sizer->Add(has_projectile, 1, wxEXPAND|wxALL);
    projectile_model_sizer->Add(projectile_model_label);
    projectile_model_sizer->Add(projectile_model, 1, wxEXPAND|wxALL);
    projectile_type_sizer->Add(projectile_type_label);
    projectile_type_sizer->Add(projectile_type, 1, wxEXPAND|wxALL);
    projectile_spawn_point_sizer->Add(projectile_spawn_point_label);
    projectile_spawn_point_sizer->Add(projectile_spawn_point, 1, wxEXPAND|wxALL);
    projectile_sound_sizer->Add(projectile_sound_label);
    projectile_sound_sizer->Add(projectile_sound, 1, wxEXPAND|wxALL);
    projectile_orientation_sizer->Add(projectile_orientation_label);
    projectile_orientation_sizer->Add(projectile_orientation, 1, wxEXPAND|wxALL);

    proj_settings_sizer->Add(projectile_sizer, 1, wxEXPAND|wxALL);
    proj_settings_sizer->Add(projectile_model_sizer, 1, wxEXPAND|wxALL);
    proj_settings_sizer->Add(projectile_type_sizer, 1, wxEXPAND|wxALL);
    proj_settings_sizer->Add(projectile_spawn_point_sizer, 1, wxEXPAND|wxALL);
    proj_settings_sizer->Add(projectile_sound_sizer, 1, wxEXPAND|wxALL);
    proj_settings_sizer->Add(projectile_orientation_sizer, 1, wxEXPAND|wxALL);

    wxStaticBoxSizer* master_sub_spells_sizer = new wxStaticBoxSizer(master_sub_spells, wxHORIZONTAL);

    wxBoxSizer* master_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sub_rad_spell_1_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sub_rad_spell_2_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sub_rad_spell_3_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sub_rad_spell_4_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sub_rad_spell_5_sizer = new wxBoxSizer(wxVERTICAL);

    master_sizer->Add(master_label);
    master_sizer->Add(master, 1, wxEXPAND|wxALL);
    sub_rad_spell_1_sizer->Add(sub_rad_spell_1_label);
    sub_rad_spell_1_sizer->Add(sub_rad_spell_1, 1, wxEXPAND|wxALL);
    sub_rad_spell_2_sizer->Add(sub_rad_spell_2_label);
    sub_rad_spell_2_sizer->Add(sub_rad_spell_2, 1, wxEXPAND|wxALL);
    sub_rad_spell_3_sizer->Add(sub_rad_spell_3_label);
    sub_rad_spell_3_sizer->Add(sub_rad_spell_3, 1, wxEXPAND|wxALL);
    sub_rad_spell_4_sizer->Add(sub_rad_spell_4_label);
    sub_rad_spell_4_sizer->Add(sub_rad_spell_4, 1, wxEXPAND|wxALL);
    sub_rad_spell_5_sizer->Add(sub_rad_spell_5_label);
    sub_rad_spell_5_sizer->Add(sub_rad_spell_5, 1, wxEXPAND|wxALL);

    master_sub_spells_sizer->Add(master_sizer, 1, wxEXPAND|wxALL);
    master_sub_spells_sizer->Add(sub_rad_spell_1_sizer, 1, wxEXPAND|wxALL);
    master_sub_spells_sizer->Add(sub_rad_spell_2_sizer, 1, wxEXPAND|wxALL);
    master_sub_spells_sizer->Add(sub_rad_spell_3_sizer, 1, wxEXPAND|wxALL);
    master_sub_spells_sizer->Add(sub_rad_spell_4_sizer, 1, wxEXPAND|wxALL);
    master_sub_spells_sizer->Add(sub_rad_spell_5_sizer, 1, wxEXPAND|wxALL);

    wxBoxSizer* alt_message_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* category_sizer = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* spell_components_sizer = new wxStaticBoxSizer(spell_components, wxHORIZONTAL);

    spell_components_sizer->Add(verbal);
    spell_components_sizer->Add(somatic);
    category_sizer->Add(category_label);
    category_sizer->Add(category, 1, wxEXPAND);
    alt_message_sizer->Add(alt_message_label);
    alt_message_sizer->Add(alt_message, 1, wxEXPAND|wxALL);

    fourth_row_sizer->Add(spell_components_sizer);
    fourth_row_sizer->Add(category_sizer, 1, wxEXPAND|wxALL);
    fourth_row_sizer->Add(alt_message_sizer, 1, wxEXPAND|wxALL);

    wxBoxSizer* feat_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* counter_1_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* counter_2_sizer = new wxBoxSizer(wxVERTICAL);

    feat_sizer->Add(feat_label);
    feat_sizer->Add(feat, 1, wxEXPAND|wxALL);
    counter_1_sizer->Add(counter_1_label);
    counter_1_sizer->Add(counter_1, 1, wxEXPAND|wxALL);
    counter_2_sizer->Add(counter_2_label);
    counter_2_sizer->Add(counter_2, 1, wxEXPAND|wxALL);

    fifth_row_sizer->Add(feat_sizer, 1, wxEXPAND|wxALL);
    fifth_row_sizer->Add(counter_1_sizer, 1, wxEXPAND|wxALL);
    fifth_row_sizer->Add(counter_2_sizer, 1, wxEXPAND|wxALL);

    main_sizer->Add(first_row_sizer, 0, wxEXPAND);
    main_sizer->Add(fourth_row_sizer, 0, wxEXPAND);
    main_sizer->Add(second_row_sizer, 0, wxEXPAND);
    main_sizer->Add(conj_settings_sizer, 0, wxEXPAND);
    main_sizer->Add(third_row_sizer, 0, wxEXPAND);
    main_sizer->Add(proj_settings_sizer, 0, wxEXPAND);
    main_sizer->Add(master_sub_spells_sizer, 0, wxEXPAND);
    main_sizer->Add(fifth_row_sizer, 0, wxEXPAND);

    wxBoxSizer* control_button_sizer = new wxBoxSizer(wxHORIZONTAL);

    control_button_sizer->Add(ok_button);
    control_button_sizer->Add(cancel_button);

    main_sizer->Add(control_button_sizer, 0, wxALIGN_RIGHT|wxRIGHT|wxBOTTOM, 2);

    panel->SetSizer(main_sizer);

    InitFormValues();
}

void SpellForm::OnOk(wxCommandEvent& event)
{
    // TODO: Add validation
    (*spell)[GETIDX(SPELL_2DA::Label)].m_Data = GetStringFromTextCtrl(label);
    (*spell)[GETIDX(SPELL_2DA::Name)].m_Data = GetNameStrRefString();
    (*spell)[GETIDX(SPELL_2DA::IconResRef)].m_Data = GetStringFromTextCtrl(icon_resref);
    (*spell)[GETIDX(SPELL_2DA::School)].m_Data = GetSchoolSelectionString();
    (*spell)[GETIDX(SPELL_2DA::Range)].m_Data = GetRangeSelectionString();
    (*spell)[GETIDX(SPELL_2DA::VS)].m_Data = GetSpellComponentsString();

    (*spell)[GETIDX(SPELL_2DA::MetaMagic)].m_Data = GetMetaMagicString();
    (*spell)[GETIDX(SPELL_2DA::TargetType)].m_Data = GetTargetTypeString();
    (*spell)[GETIDX(SPELL_2DA::ImpactScript)].m_Data = GetStringFromTextCtrl(impact_script);

    // sliders
    (*spell)[GETIDX(SPELL_2DA::Bard)].m_Data = GetSpellLevelString(spell_level_checkbox_bard, spell_level_bard);
    (*spell)[GETIDX(SPELL_2DA::Cleric)].m_Data = GetSpellLevelString(spell_level_checkbox_cleric, spell_level_cleric);
    (*spell)[GETIDX(SPELL_2DA::Druid)].m_Data = GetSpellLevelString(spell_level_checkbox_druid, spell_level_druid);
    (*spell)[GETIDX(SPELL_2DA::Paladin)].m_Data = GetSpellLevelString(spell_level_checkbox_paladin, spell_level_paladin);
    (*spell)[GETIDX(SPELL_2DA::Ranger)].m_Data = GetSpellLevelString(spell_level_checkbox_ranger, spell_level_ranger);
    (*spell)[GETIDX(SPELL_2DA::Wiz_Sorc)].m_Data = GetSpellLevelString(spell_level_checkbox_wiz_sorc, spell_level_wiz_sorc);
    (*spell)[GETIDX(SPELL_2DA::Innate)].m_Data = GetSpellLevelString(spell_level_checkbox_innate, spell_level_innate);

    (*spell)[GETIDX(SPELL_2DA::ConjAnim)].m_Data = GetConjAnimString();
    (*spell)[GETIDX(SPELL_2DA::ConjTime)].m_Data = GetStringFromTextCtrl(conj_time);
    (*spell)[GETIDX(SPELL_2DA::ConjHeadVisual)].m_Data = GetStringFromTextCtrl(conj_head_visual);
    (*spell)[GETIDX(SPELL_2DA::ConjHandVisual)].m_Data = GetStringFromTextCtrl(conj_hand_visual);
    (*spell)[GETIDX(SPELL_2DA::ConjGrndVisual)].m_Data = GetStringFromTextCtrl(conj_ground_visual);
    (*spell)[GETIDX(SPELL_2DA::ConjSoundVFX)].m_Data = GetStringFromTextCtrl(conj_sound_vfx);
    (*spell)[GETIDX(SPELL_2DA::ConjSoundMale)].m_Data = GetStringFromTextCtrl(conj_sound_male);
    (*spell)[GETIDX(SPELL_2DA::ConjSoundFemale)].m_Data = GetStringFromTextCtrl(conj_sound_female);

    (*spell)[GETIDX(SPELL_2DA::CastAnim)].m_Data = GetCastAnimString();
    (*spell)[GETIDX(SPELL_2DA::CastTime)].m_Data = GetStringFromTextCtrl(cast_time);
    (*spell)[GETIDX(SPELL_2DA::CastHeadVisual)].m_Data = GetStringFromTextCtrl(cast_head_visual);
    (*spell)[GETIDX(SPELL_2DA::CastHandVisual)].m_Data = GetStringFromTextCtrl(cast_hand_visual);
    (*spell)[GETIDX(SPELL_2DA::CastGrndVisual)].m_Data = GetStringFromTextCtrl(cast_ground_visual);
    (*spell)[GETIDX(SPELL_2DA::CastSound)].m_Data = GetStringFromTextCtrl(cast_sound);

    (*spell)[GETIDX(SPELL_2DA::Proj)].m_Data = std::string(projectile->GetValue() ? "1" : "0");
    (*spell)[GETIDX(SPELL_2DA::ProjModel)].m_Data = GetStringFromTextCtrl(projectile_model);
    (*spell)[GETIDX(SPELL_2DA::ProjType)].m_Data = GetProjectileTypeString();
    (*spell)[GETIDX(SPELL_2DA::ProjSpwnPoint)].m_Data = GetProjectileSpawnString();
    (*spell)[GETIDX(SPELL_2DA::ProjSound)].m_Data = GetStringFromTextCtrl(projectile_sound);
    (*spell)[GETIDX(SPELL_2DA::ProjOrientation)].m_Data = GetProjectileOrientationString();

    (*spell)[GETIDX(SPELL_2DA::ImmunityType)].m_Data = GetImmunityTypeString();
    (*spell)[GETIDX(SPELL_2DA::ItemImmunity)].m_Data = std::string(item_immunity->GetValue() ? "1" : "0");

    (*spell)[GETIDX(SPELL_2DA::Master)].m_Data = master_id > 0 ? std::to_string(master_id - 1) : std::string("****");
    (*spell)[GETIDX(SPELL_2DA::SubRadSpell1)].m_Data = sub_rad_spell_1_id > 0 ? std::to_string(sub_rad_spell_1_id - 1) : std::string("****");
    (*spell)[GETIDX(SPELL_2DA::SubRadSpell2)].m_Data = sub_rad_spell_2_id > 0 ? std::to_string(sub_rad_spell_2_id - 1) : std::string("****");
    (*spell)[GETIDX(SPELL_2DA::SubRadSpell3)].m_Data = sub_rad_spell_3_id > 0 ? std::to_string(sub_rad_spell_3_id - 1) : std::string("****");
    (*spell)[GETIDX(SPELL_2DA::SubRadSpell4)].m_Data = sub_rad_spell_4_id > 0 ? std::to_string(sub_rad_spell_4_id - 1) : std::string("****");
    (*spell)[GETIDX(SPELL_2DA::SubRadSpell5)].m_Data = sub_rad_spell_5_id > 0 ? std::to_string(sub_rad_spell_5_id - 1) : std::string("****");

    (*spell)[GETIDX(SPELL_2DA::Category)].m_Data = std::to_string(category->GetSelection());
    (*spell)[GETIDX(SPELL_2DA::UserType)].m_Data = GetUserTypeString();
    (*spell)[GETIDX(SPELL_2DA::SpellDesc)].m_Data = GetDescriptionStrRefString();
    (*spell)[GETIDX(SPELL_2DA::UseConcentration)].m_Data = std::string(use_concentration->GetValue() ? "1" : "0");
    (*spell)[GETIDX(SPELL_2DA::SpontaneouslyCast)].m_Data = std::string(spontaneous_cast->GetValue() ? "1" : "0");
    (*spell)[GETIDX(SPELL_2DA::AltMessage)].m_Data = GetAltMessageStrRefString();
    (*spell)[GETIDX(SPELL_2DA::HostileSetting)].m_Data = std::string(hostile_setting->GetValue() ? "1" : "0");
    (*spell)[GETIDX(SPELL_2DA::FeatID)].m_Data = GetFeatId();
    (*spell)[GETIDX(SPELL_2DA::Counter1)].m_Data = counter_1_id > 0 ? std::to_string(counter_1_id - 1) : std::string("****");
    (*spell)[GETIDX(SPELL_2DA::Counter2)].m_Data = counter_2_id > 0 ? std::to_string(counter_2_id - 1) : std::string("****");
    (*spell)[GETIDX(SPELL_2DA::HasProjectile)].m_Data = std::string(has_projectile->GetValue() ? "1" : "0");

    configuration->Set2daModified("spells", true);

    this->EndModal(wxID_OK);
}

void SpellForm::OnCancel(wxCommandEvent& event)
{
    this->EndModal(wxID_CANCEL);
}

int SpellForm::GetSchoolSelection()
{
    if (!(*spell)[GETIDX(SPELL_2DA::School)].m_IsEmpty)
    {
        switch (Get2DAString(spell, SPELL_2DA::School)[0])
        {
            case 'C': return 1; break;
            case 'D': return 2; break;
            case 'E': return 3; break;
            case 'V': return 4; break;
            case 'I': return 5; break;
            case 'N': return 6; break;
            case 'T': return 7; break;
            default: return 0; break;
        }
    }

    return 0;
}

std::string SpellForm::GetSchoolSelectionString()
{
    switch (spell_school->GetSelection())
    {
        case 1: return std::string("C");
        case 2: return std::string("D");
        case 3: return std::string("E");
        case 4: return std::string("V");
        case 5: return std::string("I");
        case 6: return std::string("N");
        case 7: return std::string("T");
        default: return std::string("A");
    }
}

int SpellForm::GetRangeSelection()
{
    if (!(*spell)[GETIDX(SPELL_2DA::Range)].m_IsEmpty)
    {
        switch ((*spell)[GETIDX(SPELL_2DA::Range)].m_Data[0])
        {
            case 'T': return 1; break;
            case 'S': return 2; break;
            case 'M': return 3; break;
            case 'L': return 4; break;
            default: return 0; break;
        }
    }

    return 0;
}

std::string SpellForm::GetRangeSelectionString()
{
    switch (spell_range->GetSelection())
    {
        case 1: return std::string("T");
        case 2: return std::string("S");
        case 3: return std::string("M");
        case 4: return std::string("L");
        default: return std::string("P");
    }
}

int SpellForm::GetConjAnimSelection()
{
    // head = 0, hand = 1
    // if it's not hand, we can just fall back on default return
    if (Get2DAString(spell, SPELL_2DA::ConjAnim) == std::string("hand"))
        return 1;

    return 0;
}

int SpellForm::GetCastAnimSelection()
{
    if (!(*spell)[GETIDX(SPELL_2DA::CastAnim)].m_IsEmpty)
    {
        // area, attack, out, self, touch, and up.
        std::string anim = Get2DAString(spell, SPELL_2DA::CastAnim);
        if (anim == std::string("attack"))
            return 1;
        else if (anim == std::string("out"))
            return 2;
        else if (anim == std::string("self"))
            return 3;
        else if (anim == std::string("touch"))
            return 4;
        else if (anim == std::string("up"))
            return 5;
    }

    return 0;
}

int SpellForm::GetProjTypeSelection()
{
    if (!(*spell)[GETIDX(SPELL_2DA::ProjType)].m_IsEmpty)
    {
        std::string type = Get2DAString(spell, SPELL_2DA::ProjType);
        if (type == std::string("accelerating"))
            return 1;
        else if (type == std::string("ballistic"))
            return 2;
        else if (type == std::string("bounce"))
            return 3;
        else if (type == std::string("burst"))
            return 4;
        else if (type == std::string("highballistic"))
            return 5;
        else if (type == std::string("homing"))
            return 6;
        else if (type == std::string("linked"))
            return 7;
        else if (type == std::string("spiral"))
            return 8;
        else if (type == std::string("test"))
            return 9;
    }

    return 0;
}

int SpellForm::GetProjSpawnPointSelection()
{
    if (!(*spell)[GETIDX(SPELL_2DA::ProjSpwnPoint)].m_IsEmpty)
    {
        std::string spawn = Get2DAString(spell, SPELL_2DA::ProjSpwnPoint);
        if (spawn == std::string("hand"))
            return 1;
        else if (spawn == std::string("monster0"))
            return 2;
        else if (spawn == std::string("monster1"))
            return 3;
        else if (spawn == std::string("monster2"))
            return 4;
        else if (spawn == std::string("monster3"))
            return 5;
        else if (spawn == std::string("monster4"))
            return 6;
    }

    return 0;
}

int SpellForm::GetProjOrientationSelection()
{
    if (!(*spell)[GETIDX(SPELL_2DA::ProjOrientation)].m_IsEmpty)
    {
        std::string orientation = Get2DAString(spell, SPELL_2DA::ProjOrientation);
        if (orientation == std::string("path"))
            return 1;
        if (orientation == std::string("target"))
            return 2;
    }

    return 0;
}

int SpellForm::GetImmunityTypeSelection()
{
    if (!(*spell)[GETIDX(SPELL_2DA::ImmunityType)].m_IsEmpty)
    {
        std::string immunity = Get2DAString(spell, SPELL_2DA::ImmunityType);
        if (immunity == std::string("Acid"))
            return 1;
        else if (immunity == std::string("Cold"))
            return 2;
        else if (immunity == std::string("Death"))
            return 3;
        else if (immunity == std::string("Divine"))
            return 4;
        else if (immunity == std::string("Disease"))
            return 5;
        else if (immunity == std::string("Electricity"))
            return 6;
        else if (immunity == std::string("Fear"))
            return 7;
        else if (immunity == std::string("Fire"))
            return 8;
        else if (immunity == std::string("Mind Affecting"))
            return 9;
        else if (immunity == std::string("Negative"))
            return 10;
        else if (immunity == std::string("Poison"))
            return 11;
        else if (immunity == std::string("Positive"))
            return 12;
        else if (immunity == std::string("Sonic"))
            return 13;
    }

    return 0;
}

int SpellForm::GetUserTypeSelection()
{
    return GetIntFromString((*spell)[GETIDX(SPELL_2DA::UserType)].m_Data);
}

void SpellForm::SetSpellComponents()
{
    verbal->SetValue(false);
    somatic->SetValue(false);

    if (!(*spell)[GETIDX(SPELL_2DA::VS)].m_IsEmpty)
    {
        for (char const& cmp : (*spell)[GETIDX(SPELL_2DA::VS)].m_Data)
        {
            switch (cmp)
            {
                case 'v': verbal->SetValue(true); break;
                case 's': somatic->SetValue(true); break;
            }
        }
    }
}

std::string SpellForm::GetSpellComponentsString()
{
    std::string result = "";
    if (verbal->GetValue())
        result += 'v';
    if (somatic->GetValue())
        result += 's';

    if (result.size() == 0)
        result = "****";

    return result;
}

void SpellForm::SetSpellMetamagic()
{
    unsigned int value = GetUintFromHex(Get2DAString(spell, SPELL_2DA::MetaMagic));

    metamagic_empower->SetValue((value & METAMAGIC_EMPOWER) > 0);
    metamagic_extend->SetValue((value & METAMAGIC_EXTEND) > 0);
    metamagic_maximize->SetValue((value & METAMAGIC_MAXIMIZE) > 0);
    metamagic_quicken->SetValue((value & METAMAGIC_QUICKEN) > 0);
    metamagic_silent->SetValue((value & METAMAGIC_SILENT) > 0);
    metamagic_still->SetValue((value & METAMAGIC_STILL) > 0);
}

void SpellForm::SetSpellTargetType()
{
    unsigned int value = GetUintFromHex(Get2DAString(spell, SPELL_2DA::TargetType));

    target_self->SetValue((value & TARGET_SELF) > 0);
    target_creature->SetValue((value & TARGET_CREATURE) > 0);
    target_areaground->SetValue((value & TARGET_AREAGROUND) > 0);
    target_items->SetValue((value & TARGET_ITEMS) > 0);
    target_doors->SetValue((value & TARGET_DOORS) > 0);
    target_placeables->SetValue((value & TARGET_PLACEABLES) > 0);
    target_triggers->SetValue((value & TARGET_TRIGGERS) > 0);
}

std::string SpellForm::GetMetaMagicString()
{
    std::uint32_t value = 0;
    if (metamagic_empower->GetValue())
        value |= METAMAGIC_EMPOWER;
    if (metamagic_extend->GetValue())
        value |= METAMAGIC_EXTEND;
    if (metamagic_maximize->GetValue())
        value |= METAMAGIC_MAXIMIZE;
    if (metamagic_quicken->GetValue())
        value |= METAMAGIC_QUICKEN;
    if (metamagic_silent->GetValue())
        value |= METAMAGIC_SILENT;
    if (metamagic_still->GetValue())
        value |= METAMAGIC_STILL;

    std::string result = "0000";
    std::string aux = "0123456789ABCDEF";

    for (unsigned int i = 0; i < 4; i++)
        result[i] = aux[(value >> ((3 - i) * 4)) & 0xF];

    result = "0x" + result;
    return result;
}

std::string SpellForm::GetTargetTypeString()
{
    std::uint32_t value = 0;
    if (target_self->GetValue())
        value |= TARGET_SELF;
    if (target_creature->GetValue())
        value |= TARGET_CREATURE;
    if (target_areaground->GetValue())
        value |= TARGET_AREAGROUND;
    if (target_items->GetValue())
        value |= TARGET_ITEMS;
    if (target_doors->GetValue())
        value |= TARGET_DOORS;
    if (target_placeables->GetValue())
        value |= TARGET_PLACEABLES;
    if (target_triggers->GetValue())
        value |= TARGET_TRIGGERS;

    std::string result = "0000";
    std::string aux = "0123456789ABCDEF";

    for (unsigned int i = 0; i < 4; i++)
        result[i] = aux[(value >> ((3 - i) * 4)) & 0xF];

    result = "0x" + result;
    return result;
}

std::string SpellForm::GetConjAnimString()
{
    switch (conj_anim->GetSelection())
    {
        case 1: return std::string("Hand");
        default: return std::string("Head");
    }
}

std::string SpellForm::GetCastAnimString()
{
    switch (cast_anim->GetSelection())
    {
        case 1: return std::string("Attack");
        case 2: return std::string("Out");
        case 3: return std::string("Self");
        case 4: return std::string("Touch");
        case 5: return std::string("Up");
        default: return std::string("Area");
    }
}


std::string SpellForm::GetSpellLevelString(wxCheckBox* chkbox, wxSlider* slider)
{
    std::string result = "";
    if (chkbox->GetValue())
        result = std::to_string(slider->GetValue());
    else
        result = "****";

    return result;
}

std::string SpellForm::GetProjectileTypeString()
{
    switch (projectile_type->GetSelection())
    {
        case 1: return std::string("Accelerating");
        case 2: return std::string("Ballistic");
        case 3: return std::string("Bounce");
        case 4: return std::string("Burst");
        case 5: return std::string("Highballistic");
        case 6: return std::string("Homing");
        case 7: return std::string("Linked");
        case 8: return std::string("Spiral");
        case 9: return std::string("Test");
        default: return std::string("None");
    }
}

std::string SpellForm::GetProjectileSpawnString()
{
    switch (projectile_spawn_point->GetSelection())
    {
        case 1: return std::string("Hand");
        case 2: return std::string("Monster0");
        case 3: return std::string("Monster1");
        case 4: return std::string("Monster2");
        case 5: return std::string("Monster3");
        case 6: return std::string("Monster4");
        default: return std::string("None");
    }
}

std::string SpellForm::GetProjectileOrientationString()
{
    switch (projectile_orientation->GetSelection())
    {
        case 1: return std::string("Path");
        case 2: return std::string("Target");
        default: return std::string("None");
    }
}

std::string SpellForm::GetImmunityTypeString()
{
    switch (immunity_type->GetSelection())
    {
        case 1: return std::string("Acid");
        case 2: return std::string("Cold");
        case 3: return std::string("Death");
        case 4: return std::string("Divine");
        case 5: return std::string("Disease");
        case 6: return std::string("Electricity");
        case 7: return std::string("Fear");
        case 8: return std::string("Fire");
        case 9: return std::string("Mind Affecting");
        case 10: return std::string("Negative");
        case 11: return std::string("Poison");
        case 12: return std::string("Positive");
        case 13: return std::string("Sonic");
        default: return std::string("None");
    }
}

std::string SpellForm::GetUserTypeString()
{
    return std::to_string(user_type->GetSelection());
}

std::string SpellForm::GetNameStrRefString()
{
    std::uint32_t strref = GetUintFromString(Get2DAString(spell, SPELL_2DA::Name));

    std::string aux = name->GetValue().ToStdString();
    std::string base_name = configuration->GetTlkString(strref);
    if (base_name != aux)
        strref = configuration->SetTlkString(aux, strref);

    return std::to_string(strref);
}

std::string SpellForm::GetDescriptionStrRefString()
{
    std::uint32_t strref = GetUintFromString(Get2DAString(spell, SPELL_2DA::SpellDesc));

    std::string aux = description->GetValue().ToStdString();
    std::string base_desc = configuration->GetTlkString(strref);
    if (base_desc != aux)
        strref = configuration->SetTlkString(aux, strref);

    return std::to_string(strref);
}

std::string SpellForm::GetAltMessageStrRefString()
{
    std::uint32_t strref = GetUintFromString(Get2DAString(spell, SPELL_2DA::AltMessage));
    if (alt_message->GetValue().IsEmpty())
        return "****";

    std::string aux = alt_message->GetValue().ToStdString();
    std::string base_altmsg = configuration->GetTlkString(strref);
    if (base_altmsg != aux)
        strref = configuration->SetTlkString(aux, strref);

    return std::to_string(strref);
}

void SpellForm::SetSpellLevels()
{
    int bard = GetIntFromString(Get2DAString(spell, SPELL_2DA::Bard));
    int cleric = GetIntFromString(Get2DAString(spell, SPELL_2DA::Cleric));
    int druid = GetIntFromString(Get2DAString(spell, SPELL_2DA::Druid));
    int paladin = GetIntFromString(Get2DAString(spell, SPELL_2DA::Paladin));
    int ranger = GetIntFromString(Get2DAString(spell, SPELL_2DA::Ranger));
    int wiz_sorc = GetIntFromString(Get2DAString(spell, SPELL_2DA::Wiz_Sorc));
    int innate = GetIntFromString(Get2DAString(spell, SPELL_2DA::Innate));

    spell_level_bard->Enable(bard >= 0);
    spell_level_cleric->Enable(cleric >= 0);
    spell_level_druid->Enable(druid >= 0);
    spell_level_paladin->Enable(paladin >= 0);
    spell_level_ranger->Enable(ranger >= 0);
    spell_level_wiz_sorc->Enable(wiz_sorc >= 0);
    spell_level_innate->Enable(innate >= 0);

    spell_level_checkbox_bard->SetValue(bard >= 0);
    spell_level_checkbox_cleric->SetValue(cleric >= 0);
    spell_level_checkbox_druid->SetValue(druid >= 0);
    spell_level_checkbox_paladin->SetValue(paladin >= 0);
    spell_level_checkbox_ranger->SetValue(ranger >= 0);
    spell_level_checkbox_wiz_sorc->SetValue(wiz_sorc >= 0);
    spell_level_checkbox_innate->SetValue(innate >= 0);

    if (bard >= 0)
    {
        spell_level_bard->SetValue(bard);
        spell_level_label_val_bard->SetLabel(Get2DAString(spell, SPELL_2DA::Bard));
    }
    if (cleric >= 0)
    {
        spell_level_cleric->SetValue(cleric);
        spell_level_label_val_cleric->SetLabel(Get2DAString(spell, SPELL_2DA::Cleric));
    }
    if (druid >= 0)
    {
        spell_level_druid->SetValue(druid);
        spell_level_label_val_druid->SetLabel(Get2DAString(spell, SPELL_2DA::Druid));
    }
    if (paladin >= 0)
    {
        spell_level_paladin->SetValue(paladin);
        spell_level_label_val_paladin->SetLabel(Get2DAString(spell, SPELL_2DA::Paladin));
    }
    if (ranger >= 0)
    {
        spell_level_ranger->SetValue(ranger);
        spell_level_label_val_ranger->SetLabel(Get2DAString(spell, SPELL_2DA::Ranger));
    }
    if (wiz_sorc >= 0)
    {
        spell_level_wiz_sorc->SetValue(wiz_sorc);
        spell_level_label_val_wiz_sorc->SetLabel(Get2DAString(spell, SPELL_2DA::Wiz_Sorc));
    }
    if (innate >= 0)
    {
        spell_level_innate->SetValue(innate);
        spell_level_label_val_innate->SetLabel(Get2DAString(spell, SPELL_2DA::Innate));
    }
}

void SpellForm::InitFormValues()
{
    this->SetTitle(wxString(Get2DAString(spell, SPELL_2DA::Label)));

    spell_school->Append(std::string("Abjuration"));
    spell_school->Append(std::string("Conjuration"));
    spell_school->Append(std::string("Divination"));
    spell_school->Append(std::string("Enchantment"));
    spell_school->Append(std::string("Evocation"));
    spell_school->Append(std::string("Illusion"));
    spell_school->Append(std::string("Necromancy"));
    spell_school->Append(std::string("Transmutation"));
    spell_school->SetSelection(GetSchoolSelection());

    spell_range->Append(std::string("Personal"));
    spell_range->Append(std::string("Touch"));
    spell_range->Append(std::string("Short"));
    spell_range->Append(std::string("Medium"));
    spell_range->Append(std::string("Long"));
    spell_range->SetSelection(GetRangeSelection());

    conj_anim->Append(std::string("Head"));
    conj_anim->Append(std::string("Hand"));
    conj_anim->SetSelection(GetConjAnimSelection());

    cast_anim->Append(std::string("Area"));
    cast_anim->Append(std::string("Attack"));
    cast_anim->Append(std::string("Out"));
    cast_anim->Append(std::string("Self"));
    cast_anim->Append(std::string("Touch"));
    cast_anim->Append(std::string("Up"));
    cast_anim->SetSelection(GetCastAnimSelection());

    projectile_type->Append(std::string("None"));
    projectile_type->Append(std::string("Accelerating"));
    projectile_type->Append(std::string("Ballistic"));
    projectile_type->Append(std::string("Bounce"));
    projectile_type->Append(std::string("Burst"));
    projectile_type->Append(std::string("Highballistic"));
    projectile_type->Append(std::string("Homing"));
    projectile_type->Append(std::string("Linked"));
    projectile_type->Append(std::string("Spiral"));
    projectile_type->Append(std::string("Test")); // is there srsly this value?
    projectile_type->SetSelection(GetProjTypeSelection());

    projectile_spawn_point->Append(std::string("None"));
    projectile_spawn_point->Append(std::string("Hand"));
    projectile_spawn_point->Append(std::string("Monster0"));
    projectile_spawn_point->Append(std::string("Monster1"));
    projectile_spawn_point->Append(std::string("Monster2"));
    projectile_spawn_point->Append(std::string("Monster3"));
    projectile_spawn_point->Append(std::string("Monster4"));
    projectile_spawn_point->SetSelection(GetProjSpawnPointSelection());

    projectile_orientation->Append(std::string("None"));
    projectile_orientation->Append(std::string("Path"));
    projectile_orientation->Append(std::string("Target"));
    projectile_orientation->SetSelection(GetProjOrientationSelection());

    immunity_type->Append(std::string("None"));
    immunity_type->Append(std::string("Acid"));
    immunity_type->Append(std::string("Cold"));
    immunity_type->Append(std::string("Death"));
    immunity_type->Append(std::string("Divine"));
    immunity_type->Append(std::string("Disease"));
    immunity_type->Append(std::string("Electricity"));
    immunity_type->Append(std::string("Fear"));
    immunity_type->Append(std::string("Fire"));
    immunity_type->Append(std::string("Mind Affecting"));
    immunity_type->Append(std::string("Negative"));
    immunity_type->Append(std::string("Poison"));
    immunity_type->Append(std::string("Positive"));
    immunity_type->Append(std::string("Sonic"));
    immunity_type->SetSelection(GetImmunityTypeSelection());

    user_type->Append(std::string("None"));
    user_type->Append(std::string("Spell"));
    user_type->Append(std::string("Creature Ability"));
    user_type->Append(std::string("Feat"));
    user_type->Append(std::string("Item Power"));
    user_type->SetSelection(GetUserTypeSelection());

    SetSpellComponents();
    SetSpellMetamagic();
    SetSpellTargetType();
    SetSpellLevels();
    SetConjValues();
    SetCastValues();
    SetProjectionValues();
    SetMiscellaneousValues();
    LoadCategoryValues();
    LoadMasterSubSpells();

    label->SetValue(wxString(Get2DAString(spell, SPELL_2DA::Label)));

    // if strref > 0 we will output just empty name
    std::uint32_t strref = GetUintFromString(Get2DAString(spell, SPELL_2DA::Name));
    name->SetValue(wxString(strref > 0 ? configuration->GetTlkString(strref) : ""));

    icon_resref->SetValue(wxString(Get2DAString(spell, SPELL_2DA::IconResRef)));
    impact_script->SetValue(wxString(Get2DAString(spell, SPELL_2DA::ImpactScript)));
}

void SpellForm::OnInnateChange(wxCommandEvent& event)
{
    int level = spell_level_innate->GetValue();
    spell_level_label_val_innate->SetLabel(std::to_string(level));
}

void SpellForm::OnBardChange(wxCommandEvent& event)
{
    int level = spell_level_bard->GetValue();
    spell_level_label_val_bard->SetLabel(std::to_string(level));
}

void SpellForm::OnClericChange(wxCommandEvent& event)
{
    int level = spell_level_cleric->GetValue();
    spell_level_label_val_cleric->SetLabel(std::to_string(level));
}

void SpellForm::OnDruidChange(wxCommandEvent& event)
{
    int level = spell_level_druid->GetValue();
    spell_level_label_val_druid->SetLabel(std::to_string(level));
}

void SpellForm::OnPaladinChange(wxCommandEvent& event)
{
    int level = spell_level_paladin->GetValue();
    spell_level_label_val_paladin->SetLabel(std::to_string(level));
}

void SpellForm::OnRangerChange(wxCommandEvent& event)
{
    int level = spell_level_ranger->GetValue();
    spell_level_label_val_ranger->SetLabel(std::to_string(level));
}

void SpellForm::OnWizSorcChange(wxCommandEvent& event)
{
    int level = spell_level_wiz_sorc->GetValue();
    spell_level_label_val_wiz_sorc->SetLabel(std::to_string(level));
}

void SpellForm::OnInnateCheck(wxCommandEvent& event)
{
    if (!spell_level_checkbox_innate->IsChecked())
    {
        spell_level_innate->SetValue(0);
        spell_level_label_val_innate->SetLabel("0");
    }

    spell_level_innate->Enable(spell_level_checkbox_innate->IsChecked());
}

void SpellForm::OnBardCheck(wxCommandEvent& event)
{
    if (!spell_level_checkbox_bard->IsChecked())
    {
        spell_level_bard->SetValue(0);
        spell_level_label_val_bard->SetLabel("0");
    }

    spell_level_bard->Enable(spell_level_checkbox_bard->IsChecked());
}

void SpellForm::OnClericCheck(wxCommandEvent& event)
{
    if (!spell_level_checkbox_cleric->IsChecked())
    {
        spell_level_cleric->SetValue(0);
        spell_level_label_val_cleric->SetLabel("0");
    }

    spell_level_cleric->Enable(spell_level_checkbox_cleric->IsChecked());
}

void SpellForm::OnDruidCheck(wxCommandEvent& event)
{
    if (!spell_level_checkbox_druid->IsChecked())
    {
        spell_level_druid->SetValue(0);
        spell_level_label_val_druid->SetLabel("0");
    }

    spell_level_druid->Enable(spell_level_checkbox_druid->IsChecked());
}

void SpellForm::OnPaladinCheck(wxCommandEvent& event)
{
    if (!spell_level_checkbox_paladin->IsChecked())
    {
        spell_level_paladin->SetValue(0);
        spell_level_label_val_paladin->SetLabel("0");
    }

    spell_level_paladin->Enable(spell_level_checkbox_paladin->IsChecked());
}

void SpellForm::OnRangerCheck(wxCommandEvent& event)
{
    if (!spell_level_checkbox_ranger->IsChecked())
    {
        spell_level_ranger->SetValue(0);
        spell_level_label_val_ranger->SetLabel("0");
    }

    spell_level_ranger->Enable(spell_level_checkbox_ranger->IsChecked());
}

void SpellForm::OnWizSorcCheck(wxCommandEvent& event)
{
    if (!spell_level_checkbox_wiz_sorc->IsChecked())
    {
        spell_level_wiz_sorc->SetValue(0);
        spell_level_label_val_wiz_sorc->SetLabel("0");
    }

    spell_level_wiz_sorc->Enable(spell_level_checkbox_wiz_sorc->IsChecked());
}

void SpellForm::SetConjValues()
{
    // Since we're just adding only text values (for now)
    // We can simply ignore empty checks
    // TODO: remove asterisks
    conj_time->SetValue(Get2DAString(spell, SPELL_2DA::ConjTime));
    conj_head_visual->SetValue(Get2DAString(spell, SPELL_2DA::ConjHeadVisual));
    conj_hand_visual->SetValue(Get2DAString(spell, SPELL_2DA::ConjHandVisual));
    conj_ground_visual->SetValue(Get2DAString(spell, SPELL_2DA::ConjGrndVisual));
    conj_sound_vfx->SetValue(Get2DAString(spell, SPELL_2DA::ConjSoundVFX));
    conj_sound_male->SetValue(Get2DAString(spell, SPELL_2DA::ConjSoundMale));
    conj_sound_female->SetValue(Get2DAString(spell, SPELL_2DA::ConjSoundFemale));
}

void SpellForm::SetCastValues()
{
    // Since we're just adding only text values (for now)
    // We can simply ignore empty checks
    cast_time->SetValue(Get2DAString(spell, SPELL_2DA::CastTime));
    cast_head_visual->SetValue(Get2DAString(spell, SPELL_2DA::CastHeadVisual));
    cast_hand_visual->SetValue(Get2DAString(spell, SPELL_2DA::CastHandVisual));
    cast_ground_visual->SetValue(Get2DAString(spell, SPELL_2DA::CastGrndVisual));
    cast_sound->SetValue(Get2DAString(spell, SPELL_2DA::CastSound));
}

void SpellForm::SetProjectionValues()
{
    int is_projectile = GetIntFromString(Get2DAString(spell, SPELL_2DA::Proj));
    projectile->SetValue(is_projectile > 0);

    int _has_projectile = GetIntFromString(Get2DAString(spell, SPELL_2DA::HasProjectile));
    has_projectile->SetValue(_has_projectile > 0);

    projectile_model->SetValue(Get2DAString(spell, SPELL_2DA::ProjModel));
    projectile_sound->SetValue(Get2DAString(spell, SPELL_2DA::ProjSound));
}

void SpellForm::SetMiscellaneousValues()
{
    int itm_immunity = GetIntFromString(Get2DAString(spell, SPELL_2DA::ItemImmunity));
    item_immunity->SetValue(itm_immunity > 0);

    int _use_concentration = GetIntFromString(Get2DAString(spell, SPELL_2DA::UseConcentration));
    int _spontaneous_cast = GetIntFromString(Get2DAString(spell, SPELL_2DA::SpontaneouslyCast));
    int _hostile_setting = GetIntFromString(Get2DAString(spell, SPELL_2DA::HostileSetting));

    use_concentration->SetValue(_use_concentration > 0);
    spontaneous_cast->SetValue(_spontaneous_cast > 0);
    hostile_setting->SetValue(_hostile_setting > 0);

    std::uint32_t strref = GetUintFromString(Get2DAString(spell, SPELL_2DA::SpellDesc));
    description->SetValue(wxString(strref > 0 ? configuration->GetTlkString(strref) : ""));
    strref = GetUintFromString(Get2DAString(spell, SPELL_2DA::AltMessage));
    alt_message->SetValue(wxString(strref > 0 ? configuration->GetTlkString(strref) : ""));

    feat_id = 0;
    counter_1_id = 0;
    counter_2_id = 0;

    std::string aux = Get2DAString(spell, SPELL_2DA::FeatID);
    if (aux.size() > 0)
    {
        feat_id = GetUintFromString(aux) + 1;
        // if feat_id is greater than 65536 then its used in radial menu
        // 65536 * (Subradial Option (min: 5000)) + (FeatId) = SPELL_2DA::FeatID
        if (feat_id > 65536)
            feat_id = ((feat_id - 1) & 0xFFFF) + 1;

        TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("feat", feat_id - 1);
        std::uint32_t strref = GetUintFromString(Get2DAString(row, FEAT_2DA::Feat));
        feat->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
    }

    aux = Get2DAString(spell, SPELL_2DA::Counter1);
    if (aux.size() > 0)
    {
        counter_1_id = GetUintFromString(aux) + 1;
        TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("spells", counter_1_id - 1);
        std::uint32_t strref = GetUintFromString(Get2DAString(row, SPELL_2DA::Name));
        counter_1->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
    }

    aux = Get2DAString(spell, SPELL_2DA::Counter2);
    if (aux.size() > 0)
    {
        counter_2_id = GetUintFromString(aux) + 1;
        TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("spells", counter_2_id - 1);
        std::uint32_t strref = GetUintFromString(Get2DAString(row, SPELL_2DA::Name));
        counter_2->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
    }
}

void SpellForm::LoadCategoryValues()
{
    TwoDA::Friendly::TwoDA* categories = configuration->Get2da("categories");

    if (categories != NULL)
    {
        for (auto const& row : (*categories))
            category->Append(row["Category"].m_Data);

        unsigned int row_id = GetUintFromString(Get2DAString(spell, SPELL_2DA::Category));
        if (row_id > 0)
            category->SetSelection(row_id - 1);
    }
}

void SpellForm::LoadMasterSubSpells()
{
    TwoDA::Friendly::TwoDA* spells = configuration->Get2da("spells");

    master_id = 0;
    sub_rad_spell_1_id = 0;
    sub_rad_spell_2_id = 0;
    sub_rad_spell_3_id = 0;
    sub_rad_spell_4_id = 0;
    sub_rad_spell_5_id = 0;

    std::string aux = Get2DAString(spell, SPELL_2DA::Master);
    if (aux.size() > 0)
    {
        master_id = GetUintFromString(aux) + 1;
        TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("spells", master_id - 1);
        std::uint32_t strref = GetUintFromString(Get2DAString(row, SPELL_2DA::Name));
        master->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
    }

    aux = Get2DAString(spell, SPELL_2DA::SubRadSpell1);
    if (aux.size() > 0)
    {
        sub_rad_spell_1_id = GetUintFromString(aux) + 1;
        TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("spells", sub_rad_spell_1_id - 1);
        std::uint32_t strref = GetUintFromString(Get2DAString(row, SPELL_2DA::Name));
        sub_rad_spell_1->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
    }

    aux = Get2DAString(spell, SPELL_2DA::SubRadSpell2);
    if (aux.size() > 0)
    {
        sub_rad_spell_2_id = GetUintFromString(aux) + 1;
        TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("spells", sub_rad_spell_2_id - 1);
        std::uint32_t strref = GetUintFromString(Get2DAString(row, SPELL_2DA::Name));
        sub_rad_spell_2->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
    }

    aux = Get2DAString(spell, SPELL_2DA::SubRadSpell3);
    if (aux.size() > 0)
    {
        sub_rad_spell_3_id = GetUintFromString(aux) + 1;
        TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("spells", sub_rad_spell_3_id - 1);
        std::uint32_t strref = GetUintFromString(Get2DAString(row, SPELL_2DA::Name));
        sub_rad_spell_3->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
    }

    aux = Get2DAString(spell, SPELL_2DA::SubRadSpell4);
    if (aux.size() > 0)
    {
        sub_rad_spell_4_id = GetUintFromString(aux) + 1;
        TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("spells", sub_rad_spell_4_id - 1);
        std::uint32_t strref = GetUintFromString(Get2DAString(row, SPELL_2DA::Name));
        sub_rad_spell_4->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
    }

    aux = Get2DAString(spell, SPELL_2DA::SubRadSpell5);
    if (aux.size() > 0)
    {
        sub_rad_spell_5_id = GetUintFromString(aux) + 1;
        TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("spells", sub_rad_spell_5_id - 1);
        std::uint32_t strref = GetUintFromString(Get2DAString(row, SPELL_2DA::Name));
        sub_rad_spell_5->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
    }
}

void SpellForm::OnMasterClick(wxCommandEvent& event)
{
    SpellSelectionForm form(panel, configuration, master_id);
    if (form.ShowModal() == wxID_OK)
    {
        master_id = form.GetSpellSelection();
        if (master_id > 0)
        {
            TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("spells", master_id - 1);
            std::uint32_t strref = GetUintFromString(Get2DAString(row, SPELL_2DA::Name));
            master->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
        }
        else
            master->SetLabel("None");
    }
}

void SpellForm::OnSubSpell1(wxCommandEvent& event)
{
    SpellSelectionForm form(panel, configuration, sub_rad_spell_1_id);
    if (form.ShowModal() == wxID_OK)
    {
        sub_rad_spell_1_id = form.GetSpellSelection();
        if (sub_rad_spell_1_id > 0)
        {
            TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("spells", sub_rad_spell_1_id - 1);
            std::uint32_t strref = GetUintFromString(Get2DAString(row, SPELL_2DA::Name));
            sub_rad_spell_1->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
        }
        else
            sub_rad_spell_1->SetLabel("None");
    }
}

void SpellForm::OnSubSpell2(wxCommandEvent& event)
{
    SpellSelectionForm form(panel, configuration, sub_rad_spell_2_id);
    if (form.ShowModal() == wxID_OK)
    {
        sub_rad_spell_2_id = form.GetSpellSelection();
        if (sub_rad_spell_2_id > 0)
        {
            TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("spells", sub_rad_spell_2_id - 1);
            std::uint32_t strref = GetUintFromString(Get2DAString(row, SPELL_2DA::Name));
            sub_rad_spell_2->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
        }
        else
            sub_rad_spell_2->SetLabel("None");
    }
}

void SpellForm::OnSubSpell3(wxCommandEvent& event)
{
    SpellSelectionForm form(panel, configuration, sub_rad_spell_3_id);
    if (form.ShowModal() == wxID_OK)
    {
        sub_rad_spell_3_id = form.GetSpellSelection();
        if (sub_rad_spell_3_id > 0)
        {
            TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("spells", sub_rad_spell_3_id - 1);
            std::uint32_t strref = GetUintFromString(Get2DAString(row, SPELL_2DA::Name));
            sub_rad_spell_3->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
        }
        else
            sub_rad_spell_3->SetLabel("None");
    }
}

void SpellForm::OnSubSpell4(wxCommandEvent& event)
{
    SpellSelectionForm form(panel, configuration, sub_rad_spell_4_id);
    if (form.ShowModal() == wxID_OK)
    {
        sub_rad_spell_4_id = form.GetSpellSelection();
        if (sub_rad_spell_4_id > 0)
        {
            TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("spells", sub_rad_spell_4_id - 1);
            std::uint32_t strref = GetUintFromString(Get2DAString(row, SPELL_2DA::Name));
            sub_rad_spell_4->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
        }
        else
            sub_rad_spell_4->SetLabel("None");
    }
}

void SpellForm::OnSubSpell5(wxCommandEvent& event)
{
    SpellSelectionForm form(panel, configuration, sub_rad_spell_5_id);
    if (form.ShowModal() == wxID_OK)
    {
        sub_rad_spell_5_id = form.GetSpellSelection();
        if (sub_rad_spell_5_id > 0)
        {
            TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("spells", sub_rad_spell_5_id - 1);
            std::uint32_t strref = GetUintFromString(Get2DAString(row, SPELL_2DA::Name));
            sub_rad_spell_5->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
        }
        else
            sub_rad_spell_5->SetLabel("None");
    }
}

void SpellForm::OnFeatClick(wxCommandEvent& event)
{
    FeatSelectionForm form(panel, configuration, feat_id);
    if (form.ShowModal() == wxID_OK)
    {
        feat_id = form.GetFeatSelection();
        if (feat_id > 0)
        {
            TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("feat", feat_id - 1);
            std::uint32_t strref = GetUintFromString(Get2DAString(row, FEAT_2DA::Feat));
            feat->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
        }
        else
            feat->SetLabel("None");
    }
}

void SpellForm::OnCounterClick1(wxCommandEvent& event)
{
    SpellSelectionForm form(panel, configuration, counter_1_id);
    if (form.ShowModal() == wxID_OK)
    {
        counter_1_id = form.GetSpellSelection();
        if (counter_1_id > 0)
        {
            TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("spells", counter_1_id - 1);
            std::uint32_t strref = GetUintFromString(Get2DAString(row, SPELL_2DA::Name));
            counter_1->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
        }
        else
            counter_1->SetLabel("None");
    }
}

void SpellForm::OnCounterClick2(wxCommandEvent& event)
{
    SpellSelectionForm form(panel, configuration, counter_2_id);
    if (form.ShowModal() == wxID_OK)
    {
        counter_2_id = form.GetSpellSelection();
        if (counter_2_id > 0)
        {
            TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("spells", counter_2_id - 1);
            std::uint32_t strref = GetUintFromString(Get2DAString(row, SPELL_2DA::Name));
            counter_2->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
        }
        else
            counter_2->SetLabel("None");
    }
}

std::string SpellForm::GetFeatId()
{
    // feat_id > 0 ? std::to_string(feat_id - 1) : std::string("****");
    if (feat_id == 0)
        return std::string("****");

    // if feat and master are both set then its subrad spell
    // and we need to modify feat id

    if (master_id > 0)
    {
        TwoDA::Friendly::TwoDARow* master_spell = configuration->Get2daRow("spells", master_id - 1);

        int index = -1;
        std::size_t sub_start = GETIDX(SPELL_2DA::SubRadSpell1);
        for (std::size_t i = 0; i < 5; i++)
        {
            if (GetUintFromString((*master_spell)[sub_start + i].m_Data) == spell->RowId())
            {
                index = static_cast<int>(i);
                break;
            }
        }

        // TODO: Implement some sort of fix (adding id to master spell?)
        if (index < 0)
        {
            wxMessageBox("Subrad spells in master spell are not set up correcly! FeatId was not set. Set up master's spell subrad spells first!",
                "Warning", wxICON_WARNING | wxOK);
            return std::string("****");
        }

        for (std::size_t i = 0; i < 5; i++)
        {
            std::uint32_t aux = GetUintFromString((*master_spell)[sub_start + i].m_Data);
            if ((aux == 0) || (aux == spell->RowId()))
                continue;
            else
            {
                TwoDA::Friendly::TwoDARow* rad_spell = configuration->Get2daRow("spells", aux);

                std::uint32_t _mt = GetUintFromString((*rad_spell)[GETIDX(SPELL_2DA::FeatID)].m_Data) & 0xFFFF;
                std::uint32_t _ft = (65536 * (5000 + i)) + _mt;

                (*rad_spell)[GETIDX(SPELL_2DA::FeatID)].m_Data = std::to_string(_ft);
            }
        }

        return std::to_string((65536 * (5000 + index)) + (feat_id - 1));
    }


    return std::to_string(feat_id - 1);
}
