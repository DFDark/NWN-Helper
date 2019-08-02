#include "spell-form.hpp"
#include "../constants.hpp"

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
    SP_CHECKBOX_WIZ_SORC
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
wxEND_EVENT_TABLE()

SpellForm::SpellForm(wxWindow* parent, TwoDA::Friendly::TwoDARow* row, Tlk::Friendly::Tlk* _tlk)
    : wxDialog(parent, wxID_ANY, wxString("Spell Form"), wxDefaultPosition, wxSize(800, 600))
{
    panel = new wxPanel(this, wxID_ANY);
    spell = row;
    tlk = _tlk;

    /*
    * FORM LABELS
    */
    label_label = new wxStaticText(panel, wxID_ANY, wxString("Label:"));
    name_label = new wxStaticText(panel, wxID_ANY, wxString("Name:"));
    spellschool_label = new wxStaticText(panel, wxID_ANY, wxString("Spell School:"));
    spellrange_label = new wxStaticText(panel, wxID_ANY, wxString("Spell Range:"));
    impact_script_label = new wxStaticText(panel, wxID_ANY, wxString("Impact Script:"));

    /*
    * FORM TEXT CONTROLS
    */
    label = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    name = new wxTextCtrl(panel, wxID_ANY, wxString(""));

    /*
    * FORM SELECT
    */
    spell_school = new wxComboBox(panel, wxID_ANY, wxString(""));
    spell_range = new wxComboBox(panel, wxID_ANY, wxString(""));

    impact_script = new wxTextCtrl(panel, wxID_ANY, wxString(""));

    spell_components = new wxStaticBox(panel, wxID_ANY, wxString("Spell Components"));
    metamagic_staticbox = new wxStaticBox(panel, wxID_ANY, wxString("Metamagic"));
    target_staticbox = new wxStaticBox(panel, wxID_ANY, wxString("Target"));
    spell_levels = new wxStaticBox(panel, wxID_ANY, wxString("Spell levels"));
    conj_settings = new wxStaticBox(panel, wxID_ANY, wxString("Conjuration settings"));

    verbal = new wxToggleButton(spell_components, wxID_ANY, wxString("Verbal"));
    somatic = new wxToggleButton(spell_components, wxID_ANY, wxString("Somatic"));

    metamagic_empower = new wxToggleButton(metamagic_staticbox, wxID_ANY, wxString("Empower"), wxDefaultPosition, wxDefaultSize);
    metamagic_extend = new wxToggleButton(metamagic_staticbox, wxID_ANY, wxString("Extend"), wxDefaultPosition, wxDefaultSize);
    metamagic_maximize = new wxToggleButton(metamagic_staticbox, wxID_ANY, wxString("Maximize"), wxDefaultPosition, wxDefaultSize);
    metamagic_quicken = new wxToggleButton(metamagic_staticbox, wxID_ANY, wxString("Quicken"), wxDefaultPosition, wxDefaultSize);
    metamagic_silent = new wxToggleButton(metamagic_staticbox, wxID_ANY, wxString("Silent"), wxDefaultPosition, wxDefaultSize);
    metamagic_still = new wxToggleButton(metamagic_staticbox, wxID_ANY, wxString("Still"), wxDefaultPosition, wxDefaultSize, wxMINIMIZE_BOX);

    target_self = new wxToggleButton(target_staticbox, wxID_ANY, wxString("Self"));
    target_creature = new wxToggleButton(target_staticbox, wxID_ANY, wxString("Creature"));
    target_areaground = new wxToggleButton(target_staticbox, wxID_ANY, wxString("Area/Ground"));
    target_items = new wxToggleButton(target_staticbox, wxID_ANY, wxString("Items"));
    target_doors = new wxToggleButton(target_staticbox, wxID_ANY, wxString("Doors"));
    target_placeables = new wxToggleButton(target_staticbox, wxID_ANY, wxString("Placeables"));
    target_triggers = new wxToggleButton(target_staticbox, wxID_ANY, wxString("Triggers"));

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

    conj_time_label = new wxStaticText(conj_settings, wxID_ANY, wxString("Time"));
    conj_anim_label = new wxStaticText(conj_settings, wxID_ANY, wxString("Animation"));
    conj_head_visual_label = new wxStaticText(conj_settings, wxID_ANY, wxString("Head visual"));
    conj_hand_visual_label = new wxStaticText(conj_settings, wxID_ANY, wxString("Hand visual"));
    conj_ground_visual_label = new wxStaticText(conj_settings, wxID_ANY, wxString("Ground visual"));

    conj_time = new wxTextCtrl(conj_settings, wxID_ANY, wxString(""));
    conj_anim = new wxComboBox(conj_settings, wxID_ANY, wxString(""));
    conj_head_visual = new wxTextCtrl(conj_settings, wxID_ANY, wxString(""));
    conj_hand_visual = new wxTextCtrl(conj_settings, wxID_ANY, wxString(""));
    conj_ground_visual = new wxTextCtrl(conj_settings, wxID_ANY, wxString(""));

    ok_button = new wxButton(panel, wxID_OK, wxString("Ok"), wxPoint(695, 535), wxSize(100, 30));
    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SpellForm::OnOk));

    cancel_button = new wxButton(panel, wxID_CANCEL, wxString("Cancel"), wxPoint(590, 535), wxSize(100, 30));

    Centre();

    // Sizers
    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* first_row_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* second_row_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* third_row_sizer = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer* second_row_sizer_p1 = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* second_row_sizer_p2 = new wxStaticBoxSizer(spell_levels, wxVERTICAL);

    wxBoxSizer* label_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* name_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* school_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* range_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* impact_script_sizer = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* spell_components_sizer = new wxStaticBoxSizer(spell_components, wxHORIZONTAL);

    label_sizer->Add(label_label);
    label_sizer->Add(label, 1, wxEXPAND|wxALL);
    name_sizer->Add(name_label);
    name_sizer->Add(name, 1, wxEXPAND|wxALL);
    school_sizer->Add(spellschool_label);
    school_sizer->Add(spell_school, 1, wxEXPAND|wxALL);
    range_sizer->Add(spellrange_label);
    range_sizer->Add(spell_range, 1, wxEXPAND|wxALL);
    impact_script_sizer->Add(impact_script_label);
    impact_script_sizer->Add(impact_script, 1, wxEXPAND|wxALL);
    spell_components_sizer->Add(verbal);
    spell_components_sizer->Add(somatic);

    first_row_sizer->Add(label_sizer, 1, wxEXPAND|wxALL);
    first_row_sizer->Add(name_sizer, 1, wxEXPAND|wxALL);
    first_row_sizer->Add(school_sizer, 1, wxEXPAND|wxALL);
    first_row_sizer->Add(range_sizer, 1, wxEXPAND|wxALL);
    first_row_sizer->Add(impact_script_sizer, 1, wxEXPAND|wxALL);
    first_row_sizer->Add(spell_components_sizer);

    wxStaticBoxSizer* meta_sizer = new wxStaticBoxSizer(metamagic_staticbox, wxHORIZONTAL);

    meta_sizer->Add(metamagic_empower);
    meta_sizer->Add(metamagic_extend);
    meta_sizer->Add(metamagic_maximize);
    meta_sizer->Add(metamagic_quicken);
    meta_sizer->Add(metamagic_silent);
    meta_sizer->Add(metamagic_still);

    wxStaticBoxSizer* target_sizer = new wxStaticBoxSizer(target_staticbox, wxHORIZONTAL);

    target_sizer->Add(target_self);
    target_sizer->Add(target_creature);
    target_sizer->Add(target_areaground);
    target_sizer->Add(target_items);
    target_sizer->Add(target_doors);
    target_sizer->Add(target_placeables);
    target_sizer->Add(target_triggers);

    second_row_sizer_p1->Add(meta_sizer);
    second_row_sizer_p1->Add(target_sizer);
    second_row_sizer->Add(second_row_sizer_p1);

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

    wxBoxSizer* conj_settings_sizer_r1 = new wxBoxSizer(wxHORIZONTAL);

    wxStaticBoxSizer* conj_settings_sizer = new wxStaticBoxSizer(conj_settings, wxVERTICAL);

    conj_time_sizer->Add(conj_time_label);
    conj_time_sizer->Add(conj_time, 0, wxALL);
    conj_anim_sizer->Add(conj_anim_label);
    conj_anim_sizer->Add(conj_anim, 1, wxEXPAND|wxALL);
    conj_head_visual_sizer->Add(conj_head_visual_label);
    conj_head_visual_sizer->Add(conj_head_visual, 1, wxEXPAND|wxALL);
    conj_hand_visual_sizer->Add(conj_hand_visual_label);
    conj_hand_visual_sizer->Add(conj_hand_visual, 1, wxEXPAND|wxALL);
    conj_ground_visual_sizer->Add(conj_ground_visual_label);
    conj_ground_visual_sizer->Add(conj_ground_visual, 1, wxEXPAND|wxALL);

    conj_settings_sizer_r1->Add(conj_time_sizer, 1, wxEXPAND|wxALL);
    conj_settings_sizer_r1->Add(conj_anim_sizer, 1, wxEXPAND|wxALL);
    conj_settings_sizer_r1->Add(conj_head_visual_sizer, 1, wxEXPAND|wxALL);
    conj_settings_sizer_r1->Add(conj_hand_visual_sizer, 1, wxEXPAND|wxALL);
    conj_settings_sizer_r1->Add(conj_ground_visual_sizer, 1, wxEXPAND|wxALL);

    conj_settings_sizer->Add(conj_settings_sizer_r1, 0);

    //third_row_sizer->Add(conj_settings_sizer, 1, wxEXPAND);
    second_row_sizer_p1->Add(conj_settings_sizer, 1, wxEXPAND);
    main_sizer->Add(first_row_sizer);
    main_sizer->Add(second_row_sizer, 0, wxEXPAND);
    //main_sizer->Add(third_row_sizer);

    wxBoxSizer* control_button_sizer = new wxBoxSizer(wxHORIZONTAL);

    control_button_sizer->Add(ok_button);
    control_button_sizer->Add(cancel_button);

    main_sizer->Add(control_button_sizer, 0, wxALIGN_RIGHT|wxRIGHT|wxBOTTOM, 2);

    panel->SetSizer(main_sizer);

    InitFormValues();
}

void SpellForm::OnOk(wxCommandEvent& event)
{
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
        switch ((*spell)[GETIDX(SPELL_2DA::School)].m_Data[0])
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

unsigned int SpellForm::GetUIntFromHex(const std::string& hex) const
{
    unsigned int result;
    try
    {
        result = std::stoul(hex, nullptr, 16);
    }
    catch (std::exception& e)
    {
        result = 0;
    }

    return result;
}

int SpellForm::GetIntFromString(const std::string& num) const
{
    unsigned int result;
    try
    {
        result = std::stoi(num);
    }
    catch(std::exception& e)
    {
        result = -1;
    }

    return result;
}

void SpellForm::SetSpellMetamagic()
{
    unsigned int value = 0;
    if (!(*spell)[GETIDX(SPELL_2DA::MetaMagic)].m_IsEmpty)
        value = GetUIntFromHex((*spell)[GETIDX(SPELL_2DA::MetaMagic)].m_Data);

    metamagic_empower->SetValue((value & METAMAGIC_EMPOWER) > 0);
    metamagic_extend->SetValue((value & METAMAGIC_EXTEND) > 0);
    metamagic_maximize->SetValue((value & METAMAGIC_MAXIMIZE) > 0);
    metamagic_quicken->SetValue((value & METAMAGIC_QUICKEN) > 0);
    metamagic_silent->SetValue((value & METAMAGIC_SILENT) > 0);
    metamagic_still->SetValue((value & METAMAGIC_STILL) > 0);
}

void SpellForm::SetSpellTargetType()
{
    unsigned int value = 0;
    if (!(*spell)[GETIDX(SPELL_2DA::TargetType)].m_IsEmpty)
        value = GetUIntFromHex((*spell)[GETIDX(SPELL_2DA::TargetType)].m_Data);

    target_self->SetValue((value & TARGET_SELF) > 0);
    target_creature->SetValue((value & TARGET_CREATURE) > 0);
    target_areaground->SetValue((value & TARGET_AREAGROUND) > 0);
    target_items->SetValue((value & TARGET_ITEMS) > 0);
    target_doors->SetValue((value & TARGET_DOORS) > 0);
    target_placeables->SetValue((value & TARGET_PLACEABLES) > 0);
    target_triggers->SetValue((value & TARGET_TRIGGERS) > 0);
}

void SpellForm::SetSpellLevels()
{
    int bard = GetIntFromString((*spell)[GETIDX(SPELL_2DA::Bard)].m_Data);
    int cleric = GetIntFromString((*spell)[GETIDX(SPELL_2DA::Cleric)].m_Data);
    int druid = GetIntFromString((*spell)[GETIDX(SPELL_2DA::Druid)].m_Data);
    int paladin = GetIntFromString((*spell)[GETIDX(SPELL_2DA::Paladin)].m_Data);
    int ranger = GetIntFromString((*spell)[GETIDX(SPELL_2DA::Ranger)].m_Data);
    int wiz_sorc = GetIntFromString((*spell)[GETIDX(SPELL_2DA::Wiz_Sorc)].m_Data);
    int innate = GetIntFromString((*spell)[GETIDX(SPELL_2DA::Innate)].m_Data);

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
        spell_level_label_val_bard->SetLabel((*spell)[GETIDX(SPELL_2DA::Bard)].m_Data);
    }
    if (cleric >= 0)
    {
        spell_level_cleric->SetValue(cleric);
        spell_level_label_val_cleric->SetLabel((*spell)[GETIDX(SPELL_2DA::Cleric)].m_Data);
    }
    if (druid >= 0)
    {
        spell_level_druid->SetValue(druid);
        spell_level_label_val_druid->SetLabel((*spell)[GETIDX(SPELL_2DA::Druid)].m_Data);
    }
    if (paladin >= 0)
    {
        spell_level_paladin->SetValue(paladin);
        spell_level_label_val_paladin->SetLabel((*spell)[GETIDX(SPELL_2DA::Paladin)].m_Data);
    }
    if (ranger >= 0)
    {
        spell_level_ranger->SetValue(ranger);
        spell_level_label_val_ranger->SetLabel((*spell)[GETIDX(SPELL_2DA::Ranger)].m_Data);
    }
    if (wiz_sorc >= 0)
    {
        spell_level_wiz_sorc->SetValue(wiz_sorc);
        spell_level_label_val_wiz_sorc->SetLabel((*spell)[GETIDX(SPELL_2DA::Wiz_Sorc)].m_Data);
    }
    if (innate >= 0)
    {
        spell_level_innate->SetValue(innate);
        spell_level_label_val_innate->SetLabel((*spell)[GETIDX(SPELL_2DA::Innate)].m_Data);
    }
}

void SpellForm::InitFormValues()
{
    if (!(*spell)[GETIDX(SPELL_2DA::Label)].m_IsEmpty)
        this->SetTitle(wxString((*spell)[GETIDX(SPELL_2DA::Label)].m_Data));

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

    SetSpellComponents();
    SetSpellMetamagic();
    SetSpellTargetType();
    SetSpellLevels();

    label->SetValue(wxString((*spell)[GETIDX(SPELL_2DA::Label)].m_Data));

    if (!(*spell)[GETIDX(SPELL_2DA::Name)].m_IsEmpty)
    {
        std::uint32_t strref = std::stoul((*spell)[GETIDX(SPELL_2DA::Name)].m_Data);
        name->SetValue(wxString((*tlk)[strref]));
    }

    impact_script->SetValue(wxString((*spell)[GETIDX(SPELL_2DA::ImpactScript)].m_Data));
}

void SpellForm::OnInnateChange(wxCommandEvent& event)
{
    int level = spell_level_innate->GetValue();
    spell_level_label_val_innate->SetLabel(std::to_string(level));
}

void SpellForm::OnBardChange(wxCommandEvent& event)
{
    printf("call 1\n" );
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
    spell_level_innate->Enable(spell_level_checkbox_innate->IsChecked());
}

void SpellForm::OnBardCheck(wxCommandEvent& event)
{
    spell_level_bard->Enable(spell_level_checkbox_bard->IsChecked());
}

void SpellForm::OnClericCheck(wxCommandEvent& event)
{
    spell_level_cleric->Enable(spell_level_checkbox_cleric->IsChecked());
}

void SpellForm::OnDruidCheck(wxCommandEvent& event)
{
    spell_level_druid->Enable(spell_level_checkbox_druid->IsChecked());
}

void SpellForm::OnPaladinCheck(wxCommandEvent& event)
{
    spell_level_paladin->Enable(spell_level_checkbox_paladin->IsChecked());
}

void SpellForm::OnRangerCheck(wxCommandEvent& event)
{
    spell_level_ranger->Enable(spell_level_checkbox_ranger->IsChecked());
}

void SpellForm::OnWizSorcCheck(wxCommandEvent& event)
{
    spell_level_wiz_sorc->Enable(spell_level_checkbox_wiz_sorc->IsChecked());
}
