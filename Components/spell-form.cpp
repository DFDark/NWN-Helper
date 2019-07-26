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

    if (!(*spell)[GETIDX(SPELL_2DA::Label)].m_IsEmpty)
        this->SetTitle(wxString((*spell)[GETIDX(SPELL_2DA::Label)].m_Data));
    /*
    * FORM LABELS
    */
    label_label = new wxStaticText(panel, wxID_ANY, wxString("Label:"),
       wxPoint(5, 5), wxDefaultSize, wxALIGN_LEFT);
    name_label = new wxStaticText(panel, wxID_ANY, wxString("Name:"),
       wxPoint(155, 5), wxDefaultSize, wxALIGN_LEFT);
    spellschool_label = new wxStaticText(panel, wxID_ANY, wxString("Spell School:"),
       wxPoint(305, 5), wxDefaultSize, wxALIGN_LEFT);
    spellrange_label = new wxStaticText(panel, wxID_ANY, wxString("Spell Range:"),
       wxPoint(425, 5), wxDefaultSize, wxALIGN_LEFT);

    /*
    * FORM TEXT CONTROLS
    */
    label = new wxTextCtrl(panel, wxID_ANY, wxString(""), wxPoint(5, 30), wxSize(150, 20));
    name = new wxTextCtrl(panel, wxID_ANY, wxString(""), wxPoint(155, 30), wxSize(150, 20));

    /*
    * FORM SELECT
    */
    spell_school = new wxComboBox(panel, wxID_ANY, wxString(""), wxPoint(305, 27));
    spell_range = new wxComboBox(panel, wxID_ANY, wxString(""), wxPoint(427, 27));

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

    verbal = new wxToggleButton(panel, wxID_ANY, wxString("Verbal"), wxPoint(50, 45), wxSize(100, 20));
    somatic = new wxToggleButton(panel, wxID_ANY, wxString("Somatic"), wxPoint(50, 70), wxSize(100, 20));
    SetSpellComponents();

    metamagic_empower = new wxToggleButton(panel, wxID_ANY, wxString("Empower"), wxPoint(20, 100), wxSize(100, 20));
    metamagic_extend = new wxToggleButton(panel, wxID_ANY, wxString("Extend"), wxPoint(20, 120), wxSize(100, 20));
    metamagic_maximize = new wxToggleButton(panel, wxID_ANY, wxString("Maximize"), wxPoint(20, 140), wxSize(100, 20));
    metamagic_quicken = new wxToggleButton(panel, wxID_ANY, wxString("Quicken"), wxPoint(20, 160), wxSize(100, 20));
    metamagic_silent = new wxToggleButton(panel, wxID_ANY, wxString("Silent"), wxPoint(20, 180), wxSize(100, 20));
    metamagic_still = new wxToggleButton(panel, wxID_ANY, wxString("Still"), wxPoint(20, 200), wxSize(100, 20));
    SetSpellMetamagic();

    target_self = new wxToggleButton(panel, wxID_ANY, wxString("Self"), wxPoint(140, 100), wxSize(100, 20));
    target_creature = new wxToggleButton(panel, wxID_ANY, wxString("Creature"), wxPoint(140, 120), wxSize(100, 20));
    target_areaground = new wxToggleButton(panel, wxID_ANY, wxString("Area/Ground"), wxPoint(140, 140), wxSize(100, 20));
    target_items = new wxToggleButton(panel, wxID_ANY, wxString("Items"), wxPoint(140, 160), wxSize(100, 20));
    target_doors = new wxToggleButton(panel, wxID_ANY, wxString("Doors"), wxPoint(140, 180), wxSize(100, 20));
    target_placeables = new wxToggleButton(panel, wxID_ANY, wxString("Placeables"), wxPoint(140, 200), wxSize(100, 20));
    target_triggers = new wxToggleButton(panel, wxID_ANY, wxString("Triggers"), wxPoint(140, 220), wxSize(100, 20));
    SetSpellTargetType();

    impact_script = new wxTextCtrl(panel, wxID_ANY, wxString(""), wxPoint(155, 250), wxSize(150, 20));

    ok_button = new wxButton(panel, wxID_OK, wxString("Ok"), wxPoint(695, 535), wxSize(100, 30));
    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SpellForm::OnOk));

    cancel_button = new wxButton(panel, wxID_CANCEL, wxString("Cancel"), wxPoint(590, 535), wxSize(100, 30));

    Centre();

    label->SetValue(wxString((*spell)[GETIDX(SPELL_2DA::Label)].m_Data));

    if (!(*spell)[GETIDX(SPELL_2DA::Name)].m_IsEmpty)
    {
        std::uint32_t strref = std::stoul((*spell)[GETIDX(SPELL_2DA::Name)].m_Data);
        name->SetValue(wxString((*tlk)[strref]));
    }

    impact_script->SetValue(wxString((*spell)[GETIDX(SPELL_2DA::ImpactScript)].m_Data));
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
