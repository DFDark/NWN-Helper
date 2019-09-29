#include "skill-form.hpp"
#include "constants.hpp"
#include "functions.hpp"

wxBEGIN_EVENT_TABLE(SkillForm, wxDialog)
    EVT_MENU(wxID_OK, SkillForm::OnOk)
    EVT_MENU(wxID_CANCEL, SkillForm::OnCancel)
wxEND_EVENT_TABLE()

SkillForm::SkillForm(wxWindow* parent, ConfigurationManager* _configuration, std::uint32_t row_id)
    : wxDialog(parent, wxID_ANY, wxString("Skill Form"), wxDefaultPosition, wxSize(800, 600))
{
    panel = new wxPanel(this, wxID_ANY);
    configuration = _configuration;
    skill = configuration->Get2daRow("skills", row_id);

    this->SetTitle(Get2DAString(skill, SKILL_2DA::Label));

    /*
    * FORM LABELS
    */
    label_label = new wxStaticText(panel, wxID_ANY, wxString("Label:"));
    name_label = new wxStaticText(panel, wxID_ANY, wxString("Name:"));
    description_label = new wxStaticText(panel, wxID_ANY, wxString("Description:"));
    icon_label = new wxStaticText(panel, wxID_ANY, wxString("Icon:"));
    category_label = new wxStaticText(panel, wxID_ANY, wxString("Category:"));
    constant_label = new wxStaticText(panel, wxID_ANY, wxString("Constant:"));
    key_ability_label = new wxStaticText(panel, wxID_ANY, wxString("Key Ability:"));
    max_cr_label = new wxStaticText(panel, wxID_ANY, wxString("Max CR:"));
    /*
    * FORM TEXT CONTROLS
    */
    label = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    name = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    description = new wxTextCtrl(panel, wxID_ANY, wxString(""), wxDefaultPosition, wxSize(450, -1), wxTE_MULTILINE);
    icon = new wxTextCtrl(panel, wxID_ANY, wxString(""));

    category = new wxComboBox(panel, wxID_ANY, wxString(""));
    constant = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    key_ability = new wxComboBox(panel, wxID_ANY, wxString(""));
    max_cr = new wxTextCtrl(panel, wxID_ANY, wxString(""));

    untrained = new wxCheckBox(panel, wxID_ANY, wxString("Untrained"));
    armor_check_penalty = new wxCheckBox(panel, wxID_ANY, wxString("Armor Check Penalty"));
    all_classes_can_use = new wxCheckBox(panel, wxID_ANY, wxString("All Classes Can Use"));
    hostile_skill = new wxCheckBox(panel, wxID_ANY, wxString("Hostile Skill"));

    ok_button = new wxButton(panel, wxID_OK, wxString("Ok"));
    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SkillForm::OnOk));

    cancel_button = new wxButton(panel, wxID_CANCEL, wxString("Cancel"));

    Centre();

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* first_row = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* second_row = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer* label_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* name_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* icon_sizer = new wxBoxSizer(wxVERTICAL);

    label_sizer->Add(label_label);
    label_sizer->Add(label, 0, wxEXPAND);
    name_sizer->Add(name_label);
    name_sizer->Add(name, 0, wxEXPAND);
    icon_sizer->Add(icon_label);
    icon_sizer->Add(icon, 0);

    first_row->Add(label_sizer, 1);
    first_row->Add(name_sizer, 1);
    first_row->Add(icon_sizer, 0);

    wxBoxSizer* category_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* constant_sizer = new wxBoxSizer(wxVERTICAL);

    category_sizer->Add(category_label);
    category_sizer->Add(category, 1, wxEXPAND);
    constant_sizer->Add(constant_label);
    constant_sizer->Add(constant, 1, wxEXPAND);

    second_row->Add(category_sizer, 1, wxEXPAND);
    second_row->Add(constant_sizer, 1, wxEXPAND);

    wxBoxSizer* description_row = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* other_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* description_sizer = new wxBoxSizer(wxVERTICAL);

    // wxBoxSizer* key_ability_sizer = new wxBoxSizer(wx)

    other_sizer->Add(key_ability_label);
    other_sizer->Add(key_ability, 1, wxEXPAND);
    other_sizer->Add(max_cr_label);
    other_sizer->Add(max_cr, 1, wxEXPAND);
    other_sizer->Add(untrained, 1, wxEXPAND);
    other_sizer->Add(armor_check_penalty, 1, wxEXPAND);
    other_sizer->Add(all_classes_can_use, 1, wxEXPAND);
    other_sizer->Add(hostile_skill, 1, wxEXPAND);

    description_sizer->Add(description_label);
    description_sizer->Add(description, 1, wxEXPAND);

    description_row->Add(other_sizer);
    description_row->Add(description_sizer, 1, wxEXPAND);

    wxBoxSizer* control_button_sizer = new wxBoxSizer(wxHORIZONTAL);

    control_button_sizer->Add(cancel_button);
    control_button_sizer->Add(ok_button);

    main_sizer->Add(first_row, 0, wxEXPAND);
    main_sizer->Add(second_row, 0, wxEXPAND);
    main_sizer->Add(description_row, 1, wxEXPAND);
    main_sizer->Add(control_button_sizer, 0, wxALIGN_RIGHT|wxRIGHT|wxBOTTOM, 2);

    panel->SetSizer(main_sizer);

    InitFormValues();
}

void SkillForm::OnOk(wxCommandEvent& event)
{
    if (!IsFloat(max_cr))
    {
        wxMessageBox("MaxCR must be floating point or empty!", "Error", wxOK | wxICON_ERROR);
        return;
    }

    (*skill)[GETIDX(SKILL_2DA::Label)].m_Data = GetStringFromTextCtrl(label);
    (*skill)[GETIDX(SKILL_2DA::Name)].m_Data = GetStrref(name, SKILL_2DA::Name);
    (*skill)[GETIDX(SKILL_2DA::Description)].m_Data = GetStrref(description, SKILL_2DA::Description);
    (*skill)[GETIDX(SKILL_2DA::Icon)].m_Data = GetStringFromTextCtrl(icon);

    (*skill)[GETIDX(SKILL_2DA::Untrained)].m_Data = std::string(untrained->GetValue() ? "1" : "0");
    (*skill)[GETIDX(SKILL_2DA::KeyAbility)].m_Data = GetKeyAbility();
    (*skill)[GETIDX(SKILL_2DA::ArmorCheckPenalty)].m_Data = std::string(armor_check_penalty->GetValue() ? "1" : "0");
    (*skill)[GETIDX(SKILL_2DA::AllClassesCanUse)].m_Data = std::string(all_classes_can_use->GetValue() ? "1" : "0");
    (*skill)[GETIDX(SKILL_2DA::Category)].m_Data = category->GetSelection() > 0 ? std::to_string(category->GetSelection()) : std::string("****");
    (*skill)[GETIDX(SKILL_2DA::MaxCR)].m_Data =
    (*skill)[GETIDX(SKILL_2DA::Constant)].m_Data = GetStringFromTextCtrl(constant);
    (*skill)[GETIDX(SKILL_2DA::HostileSkill)].m_Data = std::string(hostile_skill->GetValue() ? "1" : "0");

    configuration->Set2daModified("skills", true);

    this->EndModal(wxID_OK);
}

void SkillForm::OnCancel(wxCommandEvent& event)
{
    this->EndModal(wxID_CANCEL);
}

void SkillForm::InitFormValues()
{
    label->SetValue(Get2DAString(skill, SKILL_2DA::Label));

    std::uint32_t skill_strref = GetUintFromString(Get2DAString(skill, SKILL_2DA::Name));
    name->SetValue(wxString(skill_strref > 0 ? configuration->GetTlkString(skill_strref) : ""));

    icon->SetValue(Get2DAString(skill, SKILL_2DA::Icon));

    std::uint32_t desc_strref = GetUintFromString(Get2DAString(skill, SKILL_2DA::Description));
    description->SetValue(wxString(desc_strref > 0 ? configuration->GetTlkString(desc_strref) : ""));

    LoadCategoryValues();
    LoadMiscellaneousValues();
}

void SkillForm::LoadMiscellaneousValues()
{
    constant->SetValue(Get2DAString(skill, SKILL_2DA::Constant));

    key_ability->Append(wxString("Strength"));
    key_ability->Append(wxString("Dexterity"));
    key_ability->Append(wxString("Constitution"));
    key_ability->Append(wxString("Wisdom"));
    key_ability->Append(wxString("Intelligence"));
    key_ability->Append(wxString("Charisma"));

    std::string aux = Get2DAString(skill, SKILL_2DA::KeyAbility);
    if (aux == "STR")
        key_ability->SetSelection(0);
    else if (aux == "DEX")
        key_ability->SetSelection(1);
    else if (aux == "CON")
        key_ability->SetSelection(2);
    else if (aux == "WIS")
        key_ability->SetSelection(3);
    else if (aux == "INT")
        key_ability->SetSelection(4);
    else if (aux == "CHA")
        key_ability->SetSelection(5);

    max_cr->SetValue(Get2DAString(skill, SKILL_2DA::MaxCR));

    int untrained_value = GetIntFromString(Get2DAString(skill, SKILL_2DA::Untrained));
    untrained->SetValue(untrained_value > 0);

    int armor_check_penalty_value = GetIntFromString(Get2DAString(skill, SKILL_2DA::ArmorCheckPenalty));
    armor_check_penalty->SetValue(armor_check_penalty_value > 0);

    int all_classes_can_use_value = GetIntFromString(Get2DAString(skill, SKILL_2DA::AllClassesCanUse));
    all_classes_can_use->SetValue(all_classes_can_use_value > 0);

    int hostile_skill_value = GetIntFromString(Get2DAString(skill, SKILL_2DA::HostileSkill));
    hostile_skill->SetValue(hostile_skill_value > 0);
}

void SkillForm::LoadCategoryValues()
{
    TwoDA::Friendly::TwoDA* categories = configuration->Get2da("categories");

    if (categories != NULL)
    {
        category->Append(wxString("None"));
        for (auto const& row : (*categories))
            category->Append(row["Category"].m_Data);

        unsigned int row_id = GetUintFromString(Get2DAString(skill, SKILL_2DA::Category));
        category->SetSelection(row_id);
    }
}

std::string SkillForm::GetKeyAbility()
{
    switch (key_ability->GetSelection())
    {
        case 1: return std::string("DEX");
        case 2: return std::string("CON");
        case 3: return std::string("WIS");
        case 4: return std::string("INT");
        case 5: return std::string("CHA");
        default: return std::string("STR");
    }
}

std::string SkillForm::GetStrref(wxTextCtrl* component, const auto& column)
{
    std::uint32_t strref = GetUintFromString(Get2DAString(skill, column));
    if (component->GetValue().IsEmpty())
        return std::string("****");

    std::string aux = component->GetValue().ToStdString();
    std::string base_desc = configuration->GetTlkString(strref);
    if (base_desc != aux)
        strref = configuration->SetTlkString(aux, strref);

    return std::to_string(strref);
}
