#include "feat-form.hpp"

enum
{
    Label = 0,
    Feat,
    Description,
    Icon,
    MinAttackBonus,
    MinStr,
    MinDex,
    MinInt,
    MinWis,
    MinCon,
    MinCha,
    MinSpellLvl,
    PreReqFeat1,
    PreReqFeat2,
    GainMultiple,
    EffectsStack,
    AllClassesCanUse,
    Category,
    MaxCR,
    SpellID,
    Successor,
    CRValue,
    UsesPerDay,
    MasterFeat,
    TargetSelf,
    OrReqFeat0,
    OrReqFeat1,
    OrReqFeat2,
    OrReqFeat3,
    OrReqFeat4,
    ReqSkill,
    ReqSkillMinRanks,
    ReqSkill2,
    ReqSkillMinRanks2,
    Constant,
    ToolsCategories,
    HostileFeat,
    MinLevel,
    MinLevelClass,
    MaxLevel,
    MinFortSave,
    PreReqEpic,
    ReqAction
};

wxBEGIN_EVENT_TABLE(FeatForm, wxDialog)
    EVT_MENU(wxID_OK, FeatForm::OnOk)
    EVT_MENU(wxID_CANCEL, FeatForm::OnCancel)
wxEND_EVENT_TABLE()

FeatForm::FeatForm(wxWindow* parent, TwoDA::Friendly::TwoDARow* row)
    : wxDialog(parent, wxID_ANY, wxString("Feat Form"))
{
    panel = new wxPanel(this, wxID_ANY);
    feat = row;

    if (!(*feat)[Label].m_IsEmpty)
        this->SetTitle(wxString((*feat)[Label].m_Data));
    /*
    * FORM LABELS
    */
    label_label = new wxStaticText(panel, wxID_ANY, wxString("Label:"),
       wxPoint(5, 5), wxDefaultSize, wxALIGN_LEFT);
    name_label = new wxStaticText(panel, wxID_ANY, wxString("Name:"),
       wxPoint(155, 5), wxDefaultSize, wxALIGN_LEFT);

    /*
    * FORM TEXT CONTROLS
    */
    label = new wxTextCtrl(panel, wxID_ANY, wxString(""), wxPoint(5, 20), wxSize(150, 20));
    name = new wxTextCtrl(panel, wxID_ANY, wxString(""), wxPoint(155, 20), wxSize(150, 20));

    ok_button = new wxButton(panel, wxID_OK, wxString("Ok"), wxPoint(295, 135), wxSize(100, 30));
    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FeatForm::OnOk));

    cancel_button = new wxButton(panel, wxID_CANCEL, wxString("Cancel"), wxPoint(190, 135), wxSize(100, 30));

    Centre();

    label->SetValue(wxString((*feat)[Label].m_Data));
    // TODO: Link up with TLK and load actual value from strref
    name->SetValue(wxString((*feat)[Feat].m_Data));
}

FeatForm::~FeatForm()
{
    /*delete ok_button;
    delete cancel_button;
    delete panel;
    delete label_label;
    delete name_label;
    delete name;
    delete label;

    ok_button = NULL;
    cancel_button = NULL;
    panel = NULL;
    label_label = NULL;
    name_label = NULL;
    label = NULL;
    name = NULL;*/
}

void FeatForm::OnOk(wxCommandEvent& event)
{
    this->EndModal(wxID_OK);
}

void FeatForm::OnCancel(wxCommandEvent& event)
{
    this->EndModal(wxID_CANCEL);
}
