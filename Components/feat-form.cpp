#include "feat-form.hpp"
#include "constants.hpp"
#include "functions.hpp"
#include "spell-selection-form.hpp"
#include "feat-selection-form.hpp"

enum
{
    FT_PREREQ_FEAT_1 = wxID_HIGHEST + 1,
    FT_PREREQ_FEAT_2
};

wxBEGIN_EVENT_TABLE(FeatForm, wxDialog)
    EVT_MENU(wxID_OK, FeatForm::OnOk)
    EVT_MENU(wxID_CANCEL, FeatForm::OnCancel)
    EVT_BUTTON(FT_PREREQ_FEAT_1, FeatForm::OnPrereqFeat1)
    EVT_BUTTON(FT_PREREQ_FEAT_2, FeatForm::OnPrereqFeat2)
wxEND_EVENT_TABLE()

FeatForm::FeatForm(wxWindow* parent, ConfigurationManager* _configuration, std::uint32_t row_id)
    : wxDialog(parent, wxID_ANY, wxString("Feat Form"), wxDefaultPosition, wxSize(1024, 768))
{
    panel = new wxPanel(this, wxID_ANY);
    configuration = _configuration;
    feat = configuration->Get2daRow("feat", row_id);

    this->SetTitle(Get2DAString(feat, FEAT_2DA::Label));

    pre_req_feat_1_id = 0;
    pre_req_feat_2_id = 0;

    req_feat_staticbox = new wxStaticBox(panel, wxID_ANY, wxString("Prereq. Feats"));
    min_req_staticbox = new wxStaticBox(panel, wxID_ANY, wxString("Mininal Requirements"));
    /*
    * FORM LABELS
    */
    label_label = new wxStaticText(panel, wxID_ANY, wxString("Label:"));
    name_label = new wxStaticText(panel, wxID_ANY, wxString("Name:"));
    description_label = new wxStaticText(panel, wxID_ANY, wxString("Description:"));
    icon_label = new wxStaticText(panel, wxID_ANY, wxString("Icon:"));

    min_attack_label = new wxStaticText(min_req_staticbox, wxID_ANY, wxString("Attack Bonus:"));
    min_str_label = new wxStaticText(min_req_staticbox, wxID_ANY, wxString("Strength:"));
    min_dex_label = new wxStaticText(min_req_staticbox, wxID_ANY, wxString("Dexterity:"));
    min_int_label = new wxStaticText(min_req_staticbox, wxID_ANY, wxString("Intelligence:"));
    min_wis_label = new wxStaticText(min_req_staticbox, wxID_ANY, wxString("Wisdom:"));
    min_con_label = new wxStaticText(min_req_staticbox, wxID_ANY, wxString("Constitution:"));
    min_cha_label = new wxStaticText(min_req_staticbox, wxID_ANY, wxString("Charisma:"));
    min_spell_lvl_label = new wxStaticText(min_req_staticbox, wxID_ANY, wxString("Spell Level:"));

    /*
    * FORM TEXT CONTROLS
    */
    label = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    name = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    description = new wxTextCtrl(panel, wxID_ANY, wxString(""), wxDefaultPosition, wxSize(450, -1), wxTE_MULTILINE);
    icon = new wxTextCtrl(panel, wxID_ANY, wxString(""));

    min_attack = new wxTextCtrl(min_req_staticbox, wxID_ANY, wxString(""));
    min_str = new wxTextCtrl(min_req_staticbox, wxID_ANY, wxString(""));
    min_dex = new wxTextCtrl(min_req_staticbox, wxID_ANY, wxString(""));
    min_int = new wxTextCtrl(min_req_staticbox, wxID_ANY, wxString(""));
    min_wis = new wxTextCtrl(min_req_staticbox, wxID_ANY, wxString(""));
    min_con = new wxTextCtrl(min_req_staticbox, wxID_ANY, wxString(""));
    min_cha = new wxTextCtrl(min_req_staticbox, wxID_ANY, wxString(""));
    min_spell_lvl = new wxTextCtrl(min_req_staticbox, wxID_ANY, wxString(""));

    pre_req_feat_1 = new wxButton(req_feat_staticbox, FT_PREREQ_FEAT_1, wxString("None"));
    pre_req_feat_2 = new wxButton(req_feat_staticbox, FT_PREREQ_FEAT_2, wxString("None"));

    ok_button = new wxButton(panel, wxID_OK, wxString("Ok"));
    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FeatForm::OnOk));

    cancel_button = new wxButton(panel, wxID_CANCEL, wxString("Cancel"));

    Centre();

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* first_row = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* third_row = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer* label_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* name_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* icon_sizer = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* prereq_sizer = new wxStaticBoxSizer(req_feat_staticbox, wxHORIZONTAL);

    label_sizer->Add(label_label);
    label_sizer->Add(label, 0, wxEXPAND);
    name_sizer->Add(name_label);
    name_sizer->Add(name, 0, wxEXPAND);
    icon_sizer->Add(icon_label);
    icon_sizer->Add(icon, 0, wxEXPAND);
    prereq_sizer->Add(pre_req_feat_1, 1);
    prereq_sizer->Add(pre_req_feat_2, 1);

    first_row->Add(label_sizer, 1);
    first_row->Add(name_sizer, 1);
    first_row->Add(icon_sizer, 1);
    first_row->Add(prereq_sizer, 1);

    /*
        Minimu requirements
    */
    wxStaticBoxSizer* min_req_sizer = new wxStaticBoxSizer(min_req_staticbox, wxHORIZONTAL);
    wxBoxSizer* min_attack_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* min_str_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* min_dex_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* min_int_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* min_wis_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* min_con_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* min_cha_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* min_spell_lvl_sizer = new wxBoxSizer(wxVERTICAL);

    min_attack_sizer->Add(min_attack_label);
    min_attack_sizer->Add(min_attack, 0, wxEXPAND);
    min_str_sizer->Add(min_str_label);
    min_str_sizer->Add(min_str, 0, wxEXPAND);
    min_dex_sizer->Add(min_dex_label);
    min_dex_sizer->Add(min_dex, 0, wxEXPAND);
    min_int_sizer->Add(min_int_label);
    min_int_sizer->Add(min_int, 0, wxEXPAND);
    min_wis_sizer->Add(min_wis_label);
    min_wis_sizer->Add(min_wis, 0, wxEXPAND);
    min_con_sizer->Add(min_con_label);
    min_con_sizer->Add(min_con, 0, wxEXPAND);
    min_cha_sizer->Add(min_cha_label);
    min_cha_sizer->Add(min_cha, 0, wxEXPAND);
    min_spell_lvl_sizer->Add(min_spell_lvl_label);
    min_spell_lvl_sizer->Add(min_spell_lvl, 0, wxEXPAND);

    min_req_sizer->Add(min_attack_sizer, 1);
    min_req_sizer->Add(min_str_sizer, 1);
    min_req_sizer->Add(min_dex_sizer, 1);
    min_req_sizer->Add(min_int_sizer, 1);
    min_req_sizer->Add(min_wis_sizer, 1);
    min_req_sizer->Add(min_con_sizer, 1);
    min_req_sizer->Add(min_cha_sizer, 1);
    min_req_sizer->Add(min_spell_lvl_sizer, 1);

    wxBoxSizer* description_sizer = new wxBoxSizer(wxVERTICAL);

    description_sizer->Add(description_label);
    description_sizer->Add(description, 1, wxEXPAND);

    third_row->Add(description_sizer, 1, wxEXPAND);

    wxBoxSizer* control_button_sizer = new wxBoxSizer(wxHORIZONTAL);

    control_button_sizer->Add(cancel_button);
    control_button_sizer->Add(ok_button);

    main_sizer->Add(first_row, 0, wxEXPAND);
    main_sizer->Add(min_req_sizer, 0, wxEXPAND);
    main_sizer->Add(third_row, 1, wxEXPAND);
    main_sizer->Add(control_button_sizer, 0, wxALIGN_RIGHT|wxRIGHT|wxBOTTOM, 2);

    panel->SetSizer(main_sizer);

    InitFormValues();
}

void FeatForm::OnOk(wxCommandEvent& event)
{
    this->EndModal(wxID_OK);
}

void FeatForm::OnCancel(wxCommandEvent& event)
{
    this->EndModal(wxID_CANCEL);
}

void FeatForm::SetFeatRequirements()
{
    min_attack->SetValue(Get2DAString(feat, FEAT_2DA::MinAttackBonus));
    min_str->SetValue(Get2DAString(feat, FEAT_2DA::MinStr));
    min_dex->SetValue(Get2DAString(feat, FEAT_2DA::MinDex));
    min_int->SetValue(Get2DAString(feat, FEAT_2DA::MinInt));
    min_wis->SetValue(Get2DAString(feat, FEAT_2DA::MinWis));
    min_con->SetValue(Get2DAString(feat, FEAT_2DA::MinCon));
    min_cha->SetValue(Get2DAString(feat, FEAT_2DA::MinCha));
    min_spell_lvl->SetValue(Get2DAString(feat, FEAT_2DA::MinSpellLvl));

    pre_req_feat_1_id = GetUintFromString(Get2DAString(feat, FEAT_2DA::PreReqFeat1));
    if (pre_req_feat_1_id > 0)
    {
        pre_req_feat_1_id++; // 0 is to denote "None"
        TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("feat", pre_req_feat_1_id - 1);
        std::uint32_t strref = GetUintFromString(Get2DAString(row, FEAT_2DA::Feat));
        pre_req_feat_1->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
    }

    pre_req_feat_2_id = GetUintFromString(Get2DAString(feat, FEAT_2DA::PreReqFeat2));
    if (pre_req_feat_2_id > 0)
    {
        pre_req_feat_2_id++; // 0 is to denote "None"
        TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("feat", pre_req_feat_2_id - 1);
        std::uint32_t strref = GetUintFromString(Get2DAString(row, FEAT_2DA::Feat));
        pre_req_feat_2->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
    }
}

void FeatForm::InitFormValues()
{
    label->SetValue(Get2DAString(feat, FEAT_2DA::Label));

    std::uint32_t feat_strref = GetUintFromString(Get2DAString(feat, FEAT_2DA::Feat));
    name->SetValue(wxString(feat_strref > 0 ? configuration->GetTlkString(feat_strref) : ""));

    icon->SetValue(Get2DAString(feat, FEAT_2DA::Icon));

    std::uint32_t desc_strref = GetUintFromString(Get2DAString(feat, FEAT_2DA::Description));
    description->SetValue(wxString(desc_strref > 0 ? configuration->GetTlkString(desc_strref) : ""));

    SetFeatRequirements();
}

void FeatForm::OnPrereqFeat1(wxCommandEvent& event)
{
    FeatSelectionForm form(panel, configuration, pre_req_feat_1_id);
    if (form.ShowModal() == wxID_OK)
    {
        pre_req_feat_1_id = form.GetFeatSelection();
        if (pre_req_feat_1_id > 0)
        {
            TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("feat", pre_req_feat_1_id - 1);
            std::uint32_t strref = GetUintFromString(Get2DAString(row, FEAT_2DA::Feat));
            pre_req_feat_1->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
        }
        else
            pre_req_feat_1->SetLabel("None");
    }
}

void FeatForm::OnPrereqFeat2(wxCommandEvent& event)
{
    FeatSelectionForm form(panel, configuration, pre_req_feat_2_id);
    if (form.ShowModal() == wxID_OK)
    {
        pre_req_feat_2_id = form.GetFeatSelection();
        if (pre_req_feat_2_id > 0)
        {
            TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("feat", pre_req_feat_2_id - 1);
            std::uint32_t strref = GetUintFromString(Get2DAString(row, FEAT_2DA::Feat));
            pre_req_feat_2->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
        }
        else
            pre_req_feat_2->SetLabel("None");
    }
}
