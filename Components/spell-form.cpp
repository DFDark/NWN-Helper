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

    if (!(*spell)[SPELL_2DA::Label].m_IsEmpty)
        this->SetTitle(wxString((*spell)[SPELL_2DA::Label].m_Data));
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


    ok_button = new wxButton(panel, wxID_OK, wxString("Ok"), wxPoint(695, 535), wxSize(100, 30));
    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SpellForm::OnOk));

    cancel_button = new wxButton(panel, wxID_CANCEL, wxString("Cancel"), wxPoint(590, 535), wxSize(100, 30));

    Centre();

    label->SetValue(wxString((*spell)[SPELL_2DA::Label].m_Data));

    std::uint32_t strref = std::stoul((*spell)[SPELL_2DA::Name].m_Data);
    name->SetValue(wxString((*tlk)[strref]));
}

SpellForm::~SpellForm()
{
    /*delete ok_button;
    delete cancel_button;
    delete panel;
    delete label_label;
    delete name_label;
    delete spellschool_label;
    delete spellrange_label;
    delete label;
    delete name;
    delete spell_school;
    delete spell_range;

    ok_button = NULL;
    cancel_button = NULL;
    panel = NULL;
    label_label = NULL;
    name_label = NULL;
    spellschool_label = NULL;
    spellrange_label = NULL;
    label = NULL;
    name = NULL;
    spell_school = NULL;
    spell_range = NULL;*/
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
    if (!(*spell)[SPELL_2DA::School].m_IsEmpty)
    {
        switch ((*spell)[SPELL_2DA::School].m_Data[0])
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
    if (!(*spell)[SPELL_2DA::Range].m_IsEmpty)
    {
        switch ((*spell)[SPELL_2DA::Range].m_Data[0])
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
