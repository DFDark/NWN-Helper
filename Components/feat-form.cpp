#include "feat-form.hpp"
#include "constants.hpp"
#include "functions.hpp"
#include "SelectionForms/spell-selection-form.hpp"
#include "SelectionForms/feat-selection-form.hpp"
#include "SelectionForms/master-feat-selection-form.hpp"

enum
{
    FT_PREREQ_FEAT_1 = wxID_HIGHEST + 1,
    FT_PREREQ_FEAT_2,
    FT_SPELLID,
    FT_SUCCESSOR,
    FT_MASTER_FEAT,
    FT_REQ_ONEOF_0,
    FT_REQ_ONEOF_1,
    FT_REQ_ONEOF_2,
    FT_REQ_ONEOF_3,
    FT_REQ_ONEOF_4,
    FT_REQ_SKILL_1,
    FT_REQ_SKILL_2
};

wxBEGIN_EVENT_TABLE(FeatForm, wxDialog)
    EVT_MENU(wxID_OK, FeatForm::OnOk)
    EVT_MENU(wxID_CANCEL, FeatForm::OnCancel)
    EVT_BUTTON(FT_PREREQ_FEAT_1, FeatForm::OnPrereqFeat1)
    EVT_BUTTON(FT_PREREQ_FEAT_2, FeatForm::OnPrereqFeat2)
    EVT_BUTTON(FT_SPELLID, FeatForm::OnSpell)
    EVT_BUTTON(FT_SUCCESSOR, FeatForm::OnSuccessor)
    EVT_BUTTON(FT_MASTER_FEAT, FeatForm::OnMasterFeat)
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
    spellid = 0;
    successor_id = 0;
    or_req_feat_0_id = 0;
    or_req_feat_1_id = 0;
    or_req_feat_2_id = 0;
    or_req_feat_3_id = 0;
    or_req_feat_4_id = 0;
    req_skill_1_id = 0;
    req_skill_2_id = 0;

    req_feat_staticbox = new wxStaticBox(panel, wxID_ANY, wxString("Prereq. Feats"));
    min_req_staticbox = new wxStaticBox(panel, wxID_ANY, wxString("Minimal Requirements"));
    req_oneof_feat_staticbox = new wxStaticBox(panel, wxID_ANY, wxString("Required one of"));
    req_skill_staticbox = new wxStaticBox(panel, wxID_ANY, wxString("Required skills"));
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
    category_label = new wxStaticText(panel, wxID_ANY, wxString("Category:"));
    max_cr_label = new wxStaticText(panel, wxID_ANY, wxString("Max. CR:"));
    spell_label = new wxStaticText(panel, wxID_ANY, wxString("Spell:"));
    successor_label = new wxStaticText(panel, wxID_ANY, wxString("Successor:"));
    cr_value_label = new wxStaticText(panel, wxID_ANY, wxString("CR Value:"));
    uses_per_day_label = new wxStaticText(panel, wxID_ANY, wxString("Uses per Day:"));
    master_feat_label = new wxStaticText(panel, wxID_ANY, wxString("Master feat:"));

    /*
    * FORM TEXT CONTROLS
    */
    label = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    name = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    description = new wxTextCtrl(panel, wxID_ANY, wxString(""), wxDefaultPosition, wxSize(450, -1), wxTE_MULTILINE);
    icon = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    max_cr = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    cr_value = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    uses_per_day = new wxTextCtrl(panel, wxID_ANY, wxString(""));

    min_attack = new wxTextCtrl(min_req_staticbox, wxID_ANY, wxString(""));
    min_str = new wxTextCtrl(min_req_staticbox, wxID_ANY, wxString(""));
    min_dex = new wxTextCtrl(min_req_staticbox, wxID_ANY, wxString(""));
    min_int = new wxTextCtrl(min_req_staticbox, wxID_ANY, wxString(""));
    min_wis = new wxTextCtrl(min_req_staticbox, wxID_ANY, wxString(""));
    min_con = new wxTextCtrl(min_req_staticbox, wxID_ANY, wxString(""));
    min_cha = new wxTextCtrl(min_req_staticbox, wxID_ANY, wxString(""));
    min_spell_lvl = new wxTextCtrl(min_req_staticbox, wxID_ANY, wxString(""));
    req_skill_min_rank_1 = new wxTextCtrl(req_skill_staticbox, wxID_ANY, wxString(""));
    req_skill_min_rank_2 = new wxTextCtrl(req_skill_staticbox, wxID_ANY, wxString(""));

    pre_req_feat_1 = new wxButton(req_feat_staticbox, FT_PREREQ_FEAT_1, wxString("None"));
    pre_req_feat_2 = new wxButton(req_feat_staticbox, FT_PREREQ_FEAT_2, wxString("None"));
    spell = new wxButton(panel, FT_SPELLID, wxString("None"));
    successor = new wxButton(panel, FT_SUCCESSOR, wxString("None"));
    master_feat = new wxButton(panel, FT_MASTER_FEAT, wxString("None"));
    or_req_feat_0 = new wxButton(req_oneof_feat_staticbox, FT_REQ_ONEOF_0, wxString("None"));
    or_req_feat_1 = new wxButton(req_oneof_feat_staticbox, FT_REQ_ONEOF_1, wxString("None"));
    or_req_feat_2 = new wxButton(req_oneof_feat_staticbox, FT_REQ_ONEOF_2, wxString("None"));
    or_req_feat_3 = new wxButton(req_oneof_feat_staticbox, FT_REQ_ONEOF_3, wxString("None"));
    or_req_feat_4 = new wxButton(req_oneof_feat_staticbox, FT_REQ_ONEOF_4, wxString("None"));
    req_skill_1 = new wxButton(req_skill_staticbox, FT_REQ_SKILL_1, wxString("None"));
    req_skill_2 = new wxButton(req_skill_staticbox, FT_REQ_SKILL_2, wxString("None"));


    gain_multiple = new wxCheckBox(panel, wxID_ANY, wxString("Gain Multiple"));
    effects_stack = new wxCheckBox(panel, wxID_ANY, wxString("Effects Stack"));
    all_classes_can_use = new wxCheckBox(panel, wxID_ANY, wxString("All Classes can use"));
    target_self = new wxCheckBox(panel, wxID_ANY, wxString("Target Self"));

    category = new wxComboBox(panel, wxID_ANY, wxString(""));


    ok_button = new wxButton(panel, wxID_OK, wxString("Ok"));
    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FeatForm::OnOk));

    cancel_button = new wxButton(panel, wxID_CANCEL, wxString("Cancel"));

    Centre();

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* first_row = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* second_row = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* third_row = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* forth_row = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer* label_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* name_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* icon_sizer = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* prereq_sizer = new wxStaticBoxSizer(req_feat_staticbox, wxHORIZONTAL);

    label_sizer->Add(label_label);
    label_sizer->Add(label, 0, wxEXPAND);
    name_sizer->Add(name_label);
    name_sizer->Add(name, 0, wxEXPAND);
    icon_sizer->Add(icon_label);
    icon_sizer->Add(icon, 0);
    prereq_sizer->Add(pre_req_feat_1, 1);
    prereq_sizer->Add(pre_req_feat_2, 1);

    first_row->Add(label_sizer, 1);
    first_row->Add(name_sizer, 1);
    first_row->Add(icon_sizer, 0);
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

    wxBoxSizer* checkbox_sizer_1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* checkbox_sizer_2 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* category_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* max_cr_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* spell_id_sizer = new wxBoxSizer(wxVERTICAL);

    checkbox_sizer_1->Add(gain_multiple);
    checkbox_sizer_1->Add(effects_stack);
    checkbox_sizer_2->Add(all_classes_can_use);
    checkbox_sizer_2->Add(target_self);
    category_sizer->Add(category_label);
    category_sizer->Add(category);
    max_cr_sizer->Add(max_cr_label);
    max_cr_sizer->Add(max_cr);
    spell_id_sizer->Add(spell_label);
    spell_id_sizer->Add(spell, 0, wxEXPAND);

    second_row->Add(checkbox_sizer_1);
    second_row->Add(checkbox_sizer_2);
    second_row->Add(category_sizer);
    second_row->Add(max_cr_sizer);
    second_row->Add(spell_id_sizer, 1);

    wxBoxSizer* successor_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* cr_value_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* uses_per_day_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* master_feat_sizer = new wxBoxSizer(wxVERTICAL);

    successor_sizer->Add(successor_label);
    successor_sizer->Add(successor);
    cr_value_sizer->Add(cr_value_label);
    cr_value_sizer->Add(cr_value);
    uses_per_day_sizer->Add(uses_per_day_label);
    uses_per_day_sizer->Add(uses_per_day);
    master_feat_sizer->Add(master_feat_label);
    master_feat_sizer->Add(master_feat);

    third_row->Add(successor_sizer);
    third_row->Add(cr_value_sizer);
    third_row->Add(uses_per_day_sizer);
    third_row->Add(master_feat_sizer);

    wxBoxSizer* description_row = new wxBoxSizer(wxHORIZONTAL);
    wxStaticBoxSizer* req_oneof_sizer = new wxStaticBoxSizer(req_oneof_feat_staticbox, wxVERTICAL);
    wxStaticBoxSizer* req_skill_sizer = new wxStaticBoxSizer(req_skill_staticbox, wxVERTICAL);
    wxBoxSizer* description_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* skill_sizer_1 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* skill_sizer_2 = new wxBoxSizer(wxHORIZONTAL);

    req_oneof_sizer->Add(or_req_feat_0, 0, wxEXPAND);
    req_oneof_sizer->Add(or_req_feat_1, 0, wxEXPAND);
    req_oneof_sizer->Add(or_req_feat_2, 0, wxEXPAND);
    req_oneof_sizer->Add(or_req_feat_3, 0, wxEXPAND);
    req_oneof_sizer->Add(or_req_feat_4, 0, wxEXPAND);

    skill_sizer_1->Add(req_skill_1, 1, wxEXPAND);
    skill_sizer_1->Add(req_skill_min_rank_1);
    skill_sizer_2->Add(req_skill_2, 1, wxEXPAND);
    skill_sizer_2->Add(req_skill_min_rank_2);

    req_skill_sizer->Add(skill_sizer_1, 0, wxEXPAND);
    req_skill_sizer->Add(skill_sizer_2, 0, wxEXPAND);

    description_sizer->Add(description_label);
    description_sizer->Add(description, 1, wxEXPAND);

    description_row->Add(req_oneof_sizer, 1, wxEXPAND);
    description_row->Add(req_skill_sizer, 1);
    description_row->Add(description_sizer, 1, wxEXPAND);

    forth_row->Add(description_row);

    wxBoxSizer* control_button_sizer = new wxBoxSizer(wxHORIZONTAL);

    control_button_sizer->Add(cancel_button);
    control_button_sizer->Add(ok_button);

    main_sizer->Add(first_row, 0, wxEXPAND);
    main_sizer->Add(min_req_sizer, 0, wxEXPAND);
    main_sizer->Add(second_row, 0, wxEXPAND);
    main_sizer->Add(third_row, 0, wxEXPAND);
    main_sizer->Add(forth_row, 1, wxEXPAND);
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

    // "One of" Section
    or_req_feat_0_id = GetUintFromString(Get2DAString(feat, FEAT_2DA::OrReqFeat0));
    if (or_req_feat_0_id > 0)
    {
        or_req_feat_0_id++;
        TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("feat", or_req_feat_0_id - 1);
        std::uint32_t strref = GetUintFromString(Get2DAString(row, FEAT_2DA::Feat));
        or_req_feat_0->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
    }

    or_req_feat_1_id = GetUintFromString(Get2DAString(feat, FEAT_2DA::OrReqFeat1));
    if (or_req_feat_1_id > 0)
    {
        or_req_feat_1_id++;
        TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("feat", or_req_feat_1_id - 1);
        std::uint32_t strref = GetUintFromString(Get2DAString(row, FEAT_2DA::Feat));
        or_req_feat_1->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
    }

    or_req_feat_2_id = GetUintFromString(Get2DAString(feat, FEAT_2DA::OrReqFeat2));
    if (or_req_feat_2_id > 0)
    {
        or_req_feat_2_id++;
        TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("feat", or_req_feat_2_id - 1);
        std::uint32_t strref = GetUintFromString(Get2DAString(row, FEAT_2DA::Feat));
        or_req_feat_2->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
    }

    or_req_feat_3_id = GetUintFromString(Get2DAString(feat, FEAT_2DA::OrReqFeat3));
    if (or_req_feat_3_id > 0)
    {
        or_req_feat_0_id++;
        TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("feat", or_req_feat_3_id - 1);
        std::uint32_t strref = GetUintFromString(Get2DAString(row, FEAT_2DA::Feat));
        or_req_feat_3->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
    }

    or_req_feat_4_id = GetUintFromString(Get2DAString(feat, FEAT_2DA::OrReqFeat4));
    if (or_req_feat_4_id > 0)
    {
        or_req_feat_4_id++;
        TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("feat", or_req_feat_4_id - 1);
        std::uint32_t strref = GetUintFromString(Get2DAString(row, FEAT_2DA::Feat));
        or_req_feat_4->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
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
    LoadCategoryValues();
    LoadSpellIdValue();
    LoadSuccessorIdValue();
    LoadMiscellaneousValues();
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

void FeatForm::LoadCategoryValues()
{
    TwoDA::Friendly::TwoDA* categories = configuration->Get2da("categories");

    if (categories != NULL)
    {
        for (auto const& row : (*categories))
            category->Append(row["Category"].m_Data);

        unsigned int row_id = GetUintFromString(Get2DAString(feat, FEAT_2DA::Category));
        if (row_id > 0)
            category->SetSelection(row_id - 1);
    }
}

void FeatForm::OnSpell(wxCommandEvent& event)
{
    SpellSelectionForm form(panel, configuration, spellid);
    if (form.ShowModal() == wxID_OK)
    {
        spellid = form.GetSpellSelection();
        if (spellid > 0)
        {
            TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("spells", spellid - 1);
            std::uint32_t strref = GetUintFromString(Get2DAString(row, SPELL_2DA::Name));
            spell->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
        }
        else
            spell->SetLabel("None");
    }
}

void FeatForm::LoadSpellIdValue()
{
    std::string sp_id = Get2DAString(feat, FEAT_2DA::SpellID);
    if (sp_id.size() > 0)
    {
        spellid = GetUintFromString(sp_id) + 1;
        TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("spells", spellid - 1);
        std::uint32_t strref = GetUintFromString(Get2DAString(row, SPELL_2DA::Name));
        spell->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
    }
    else
        spell->SetLabel("None");
}

void FeatForm::OnSuccessor(wxCommandEvent& event)
{
    FeatSelectionForm form(panel, configuration, successor_id);
    if (form.ShowModal() == wxID_OK)
    {
        successor_id = form.GetFeatSelection();
        if (successor_id > 0)
        {
            TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("feat", successor_id - 1);
            std::uint32_t strref = GetUintFromString(Get2DAString(row, FEAT_2DA::Feat));
            successor->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
        }
        else
            successor->SetLabel("None");
    }
}

void FeatForm::OnMasterFeat(wxCommandEvent& event)
{
    MasterFeatSelectionForm form(panel, configuration, master_feat_id);
    if (form.ShowModal() == wxID_OK)
    {
        master_feat_id = form.GetMasterFeatSelection();
        if (master_feat_id > 0)
        {
            TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("masterfeats", master_feat_id - 1);
            std::uint32_t strref = GetUintFromString(Get2DAString(row, MASTERFEAT_2DA::Strref));
            master_feat->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
        }
        else
            master_feat->SetLabel("None");
    }
}

void FeatForm::LoadSuccessorIdValue()
{
    std::string aux = Get2DAString(feat, FEAT_2DA::Successor);
    if (aux.size() > 0)
    {
        successor_id = GetUintFromString(aux) + 1;
        TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("feat", successor_id - 1);
        std::uint32_t strref = GetUintFromString(Get2DAString(row, FEAT_2DA::Feat));
        successor->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
    }
    else
        successor->SetLabel("None");
}

void FeatForm::LoadMiscellaneousValues()
{
    cr_value->SetValue(Get2DAString(feat, FEAT_2DA::CRValue));
    uses_per_day->SetValue(Get2DAString(feat, FEAT_2DA::UsesPerDay));

    std::uint32_t aux = GetUintFromString(Get2DAString(feat, FEAT_2DA::GainMultiple));
    gain_multiple->SetValue(aux > 0);
    aux = GetUintFromString(Get2DAString(feat, FEAT_2DA::EffectsStack));
    effects_stack->SetValue(aux > 0);
    aux = GetUintFromString(Get2DAString(feat, FEAT_2DA::AllClassesCanUse));
    all_classes_can_use->SetValue(aux > 0);
    aux = GetUintFromString(Get2DAString(feat, FEAT_2DA::TargetSelf));
    target_self->SetValue(aux > 0);
}

void FeatForm::LoadSkillValues()
{
    req_skill_1_id = GetUintFromString(Get2DAString(feat, FEAT_2DA::ReqSkill));
    if (req_skill_1_id > 0)
    {
        req_skill_1_id++;
        /* TODO: implement skills
        TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("feat", successor_id - 1);
        std::uint32_t strref = GetUintFromString(Get2DAString(row, FEAT_2DA::Feat));
        successor->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
        */
    }

    req_skill_2_id = GetUintFromString(Get2DAString(feat, FEAT_2DA::ReqSkill2));
    {
        req_skill_2_id++;
    }
}
