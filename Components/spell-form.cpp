#include "spell-form.hpp"
#include "../constants.hpp"

wxBEGIN_EVENT_TABLE(SpellForm, wxDialog)
    EVT_MENU(wxID_OK, SpellForm::OnOk)
    EVT_MENU(wxID_CANCEL, SpellForm::OnCancel)
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

    verbal = new wxToggleButton(spell_components, wxID_ANY, wxString("Verbal"));
    somatic = new wxToggleButton(spell_components, wxID_ANY, wxString("Somatic"));

    metamagic_empower = new wxToggleButton(metamagic_staticbox, wxID_ANY, wxString("Empower"));
    metamagic_extend = new wxToggleButton(metamagic_staticbox, wxID_ANY, wxString("Extend"));
    metamagic_maximize = new wxToggleButton(metamagic_staticbox, wxID_ANY, wxString("Maximize"));
    metamagic_quicken = new wxToggleButton(metamagic_staticbox, wxID_ANY, wxString("Quicken"));
    metamagic_silent = new wxToggleButton(metamagic_staticbox, wxID_ANY, wxString("Silent"));
    metamagic_still = new wxToggleButton(metamagic_staticbox, wxID_ANY, wxString("Still"));

    target_self = new wxToggleButton(target_staticbox, wxID_ANY, wxString("Self"));
    target_creature = new wxToggleButton(target_staticbox, wxID_ANY, wxString("Creature"));
    target_areaground = new wxToggleButton(target_staticbox, wxID_ANY, wxString("Area/Ground"));
    target_items = new wxToggleButton(target_staticbox, wxID_ANY, wxString("Items"));
    target_doors = new wxToggleButton(target_staticbox, wxID_ANY, wxString("Doors"));
    target_placeables = new wxToggleButton(target_staticbox, wxID_ANY, wxString("Placeables"));
    target_triggers = new wxToggleButton(target_staticbox, wxID_ANY, wxString("Triggers"));


    ok_button = new wxButton(panel, wxID_OK, wxString("Ok"), wxPoint(695, 535), wxSize(100, 30));
    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SpellForm::OnOk));

    cancel_button = new wxButton(panel, wxID_CANCEL, wxString("Cancel"), wxPoint(590, 535), wxSize(100, 30));

    Centre();

    // Sizers
    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* first_row_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* second_row_sizer = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer* second_row_sizer_p1 = new wxBoxSizer(wxVERTICAL);

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

    main_sizer->Add(first_row_sizer);
    main_sizer->Add(second_row_sizer);

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

    label->SetValue(wxString((*spell)[GETIDX(SPELL_2DA::Label)].m_Data));

    if (!(*spell)[GETIDX(SPELL_2DA::Name)].m_IsEmpty)
    {
        std::uint32_t strref = std::stoul((*spell)[GETIDX(SPELL_2DA::Name)].m_Data);
        name->SetValue(wxString((*tlk)[strref]));
    }

    impact_script->SetValue(wxString((*spell)[GETIDX(SPELL_2DA::ImpactScript)].m_Data));
}
