#include "feat-form.hpp"
#include "constants.hpp"
#include "functions.hpp"

wxBEGIN_EVENT_TABLE(FeatForm, wxDialog)
    EVT_MENU(wxID_OK, FeatForm::OnOk)
    EVT_MENU(wxID_CANCEL, FeatForm::OnCancel)
wxEND_EVENT_TABLE()

FeatForm::FeatForm(wxWindow* parent, ConfigurationManager* _configuration, std::uint32_t row_id)
    : wxDialog(parent, wxID_ANY, wxString("Feat Form"), wxDefaultPosition, wxSize(1024, 768))
{
    panel = new wxPanel(this, wxID_ANY);
    configuration = _configuration;
    feat = configuration->Get2daRow("feat", row_id);

    this->SetTitle(Get2DAString(feat, FEAT_2DA::Label));

    /*
    * FORM LABELS
    */
    label_label = new wxStaticText(panel, wxID_ANY, wxString("Label:"));
    name_label = new wxStaticText(panel, wxID_ANY, wxString("Name:"));
    description_label = new wxStaticText(panel, wxID_ANY, wxString("Description:"));
    icon_label = new wxStaticText(panel, wxID_ANY, wxString("Icon:"));

    min_attack_label = new wxStaticText(panel, wxID_ANY, wxString("Min. Attack Bonus:"));
    min_str_label = new wxStaticText(panel, wxID_ANY, wxString("Min. Str.:"));
    min_dex_label = new wxStaticText(panel, wxID_ANY, wxString("Min. Dex.:"));
    min_int_label = new wxStaticText(panel, wxID_ANY, wxString("Min. Int.:"));
    min_wis_label = new wxStaticText(panel, wxID_ANY, wxString("Min. Wis.:"));
    min_con_label = new wxStaticText(panel, wxID_ANY, wxString("Min. Con.:"));
    min_cha_label = new wxStaticText(panel, wxID_ANY, wxString("Min. Cha.:"));
    min_spell_lvl_label = new wxStaticText(panel, wxID_ANY, wxString("Min. Spell Level:"));

    /*
    * FORM TEXT CONTROLS
    */
    label = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    name = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    description = new wxTextCtrl(panel, wxID_ANY, wxString(""), wxDefaultPosition, wxSize(450, -1), wxTE_MULTILINE);
    icon = new wxTextCtrl(panel, wxID_ANY, wxString(""));

    min_attack = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    min_str = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    min_dex = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    min_int = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    min_wis = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    min_con = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    min_cha = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    min_spell_lvl = new wxTextCtrl(panel, wxID_ANY, wxString(""));

    ok_button = new wxButton(panel, wxID_OK, wxString("Ok"));
    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FeatForm::OnOk));

    cancel_button = new wxButton(panel, wxID_CANCEL, wxString("Cancel"));

    Centre();

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* first_row = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* second_row = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* third_row = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer* label_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* name_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* icon_sizer = new wxBoxSizer(wxVERTICAL);

    label_sizer->Add(label_label);
    label_sizer->Add(label, 1, wxEXPAND|wxALL);
    name_sizer->Add(name_label);
    name_sizer->Add(name, 1, wxEXPAND|wxALL);
    icon_sizer->Add(icon_label);
    icon_sizer->Add(icon);

    first_row->Add(label_sizer);
    first_row->Add(name_sizer, 1, wxEXPAND);
    first_row->Add(icon_sizer);

    /*
        Minimu requirements
    */
    wxBoxSizer* min_attack_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* min_str_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* min_dex_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* min_int_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* min_wis_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* min_con_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* min_cha_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* min_spell_lvl_sizer = new wxBoxSizer(wxVERTICAL);

    min_attack_sizer->Add(min_attack_label);
    min_attack_sizer->Add(min_attack);
    min_str_sizer->Add(min_str_label);
    min_str_sizer->Add(min_str);
    min_dex_sizer->Add(min_dex_label);
    min_dex_sizer->Add(min_dex);
    min_int_sizer->Add(min_int_label);
    min_int_sizer->Add(min_int);
    min_wis_sizer->Add(min_wis_label);
    min_wis_sizer->Add(min_wis);
    min_con_sizer->Add(min_con_label);
    min_con_sizer->Add(min_con);
    min_cha_sizer->Add(min_cha_label);
    min_cha_sizer->Add(min_cha);
    min_spell_lvl_sizer->Add(min_spell_lvl_label);
    min_spell_lvl_sizer->Add(min_spell_lvl);

    second_row->Add(min_attack_sizer);
    second_row->Add(min_str_sizer);
    second_row->Add(min_dex_sizer);
    second_row->Add(min_int_sizer);
    second_row->Add(min_wis_sizer);
    second_row->Add(min_con_sizer);
    second_row->Add(min_cha_sizer);
    second_row->Add(min_spell_lvl_sizer);
    
    wxBoxSizer* description_sizer = new wxBoxSizer(wxVERTICAL);

    description_sizer->Add(description_label);
    description_sizer->Add(description, 1, wxEXPAND);

    third_row->Add(description_sizer, 1, wxEXPAND);

    wxBoxSizer* control_button_sizer = new wxBoxSizer(wxHORIZONTAL);

    control_button_sizer->Add(cancel_button);
    control_button_sizer->Add(ok_button);
    
    main_sizer->Add(first_row);
    main_sizer->Add(second_row);
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
