#include "spell-form.hpp"

enum
{
    ID = 0,
    Label,
    Name,
    IconResRef,
    School,
    Range,
    VS,
    MetaMagic,
    TargetType,
    ImpactScript,
    Bard,
    Cleric,
    Druid,
    Paladin,
    Ranger,
    Wiz_Sorc,
    Innate,
    ConjTime,
    ConjAnim,
    ConjHeadVisual,
    ConjHandVisual,
    ConjGrndVisual,
    ConjSoundVFX,
    ConjSoundMale,
    ConjSoundFemale,
    CastAnim,
    CastTime,
    CastHeadVisual,
    CastHandVisual,
    CastGrndVisual,
    CastSound,
    Proj,
    ProjModel,
    ProjType,
    ProjSpwnPoint,
    ProjSound,
    ProjOrientation,
    ImmunityType,
    ItemImmunity,
    SubRadSpell1,
    SubRadSpell2,
    SubRadSpell3,
    SubRadSpell4,
    SubRadSpell5,
    Category,
    Master,
    UserType,
    Values,
    SpellDesc,
    UseConcentration,
    SpontaneouslyCast,
    AltMessage,
    HostileSetting,
    FeatID,
    Counter1,
    Counter2,
    HasProjectile
};

wxBEGIN_EVENT_TABLE(SpellForm, wxDialog)
    EVT_MENU(wxID_OK, SpellForm::OnOk)
    EVT_MENU(wxID_CANCEL, SpellForm::OnCancel)
wxEND_EVENT_TABLE()

SpellForm::SpellForm(wxWindow* parent, TwoDA::Friendly::TwoDARow* row)
    : wxDialog(parent, wxID_ANY, wxString("Spell Form"))
{
    panel = new wxPanel(this, wxID_ANY);
    spell = row;
    
    if (!(*spell)[1].m_IsEmpty)
        this->SetTitle(wxString((*spell)[1].m_Data));
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
       wxPoint(305, 5), wxDefaultSize, wxALIGN_LEFT);
    
    /*
    * FORM TEXT CONTROLS
    */
    label = new wxTextCtrl(panel, wxID_ANY, wxString(""), wxPoint(5, 20), wxSize(150, 20));
    name = new wxTextCtrl(panel, wxID_ANY, wxString(""), wxPoint(155, 20), wxSize(150, 20));
    
    /*
    * FORM SELECT
    */
    spell_school = new wxComboBox(panel, wxID_ANY, wxString(""), wxPoint(305, 20), wxSize(100, 20),
        0/*, const wxString choices[]=NULL*/);
    spell_range = new wxComboBox(panel, wxID_ANY, wxString(""));
    
    
    ok_button = new wxButton(panel, wxID_OK, wxString("Ok"), wxPoint(295, 135), wxSize(100, 30));
    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SpellForm::OnOk));
    
    cancel_button = new wxButton(panel, wxID_CANCEL, wxString("Cancel"), wxPoint(190, 135), wxSize(100, 30));
    
    Centre();
    
    label->SetValue(wxString((*spell)[Label].m_Data));
    // TODO: Link up with TLK and load actual value from strref
    name->SetValue(wxString((*spell)[Name].m_Data));
}

SpellForm::~SpellForm()
{
    delete ok_button;
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
    spell_range = NULL;
}

void SpellForm::OnOk(wxCommandEvent& event)
{
    this->EndModal(wxID_OK);
}

void SpellForm::OnCancel(wxCommandEvent& event)
{
    this->EndModal(wxID_CANCEL);
}