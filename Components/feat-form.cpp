#include "feat-form.hpp"
#include "constants.hpp"
#include "functions.hpp"
#include "SelectionForms/spell-selection-form.hpp"
#include "SelectionForms/feat-selection-form.hpp"
#include "SelectionForms/master-feat-selection-form.hpp"
#include "SelectionForms/skill-selection-form.hpp"
#include "SelectionForms/class-selection-form.hpp"

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
    FT_REQ_SKILL_2,
    FT_MIN_LEVEL_CLASS
};

wxBEGIN_EVENT_TABLE(FeatForm, wxDialog)
    EVT_MENU(wxID_OK, FeatForm::OnOk)
    EVT_MENU(wxID_CANCEL, FeatForm::OnCancel)
    EVT_BUTTON(FT_PREREQ_FEAT_1, FeatForm::OnPrereqFeat1)
    EVT_BUTTON(FT_PREREQ_FEAT_2, FeatForm::OnPrereqFeat2)
    EVT_BUTTON(FT_SPELLID, FeatForm::OnSpell)
    EVT_BUTTON(FT_SUCCESSOR, FeatForm::OnSuccessor)
    EVT_BUTTON(FT_MASTER_FEAT, FeatForm::OnMasterFeat)
    EVT_BUTTON(FT_REQ_SKILL_1, FeatForm::OnRequiredSkill1)
    EVT_BUTTON(FT_REQ_SKILL_2, FeatForm::OnRequiredSkill2)
    EVT_BUTTON(FT_REQ_ONEOF_0, FeatForm::OnRequiredOneOf0)
    EVT_BUTTON(FT_REQ_ONEOF_1, FeatForm::OnRequiredOneOf1)
    EVT_BUTTON(FT_REQ_ONEOF_2, FeatForm::OnRequiredOneOf2)
    EVT_BUTTON(FT_REQ_ONEOF_3, FeatForm::OnRequiredOneOf3)
    EVT_BUTTON(FT_REQ_ONEOF_4, FeatForm::OnRequiredOneOf4)
    EVT_BUTTON(FT_MIN_LEVEL_CLASS, FeatForm::OnMinLevelClass)
wxEND_EVENT_TABLE()

FeatForm::FeatForm(wxWindow* parent, ConfigurationManager* _configuration, std::uint32_t row_id)
    : wxDialog(parent, wxID_ANY, wxString("Feat Form"), wxDefaultPosition, wxSize(800, 600))
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
    min_level_class_id = 0;

    req_feat_staticbox = new wxStaticBox(panel, wxID_ANY, wxString("Prereq. Feats"), wxDefaultPosition, wxSize(400, -1));
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
    constant_label = new wxStaticText(panel, wxID_ANY, wxString("Constant:"));
    tools_categories_label = new wxStaticText(panel, wxID_ANY, wxString("Tools Categories:"));
    min_level_label = new wxStaticText(panel, wxID_ANY, wxString("Min. Level:"));
    min_level_class_label = new wxStaticText(panel, wxID_ANY, wxString("Min. Level Class:"));
    max_level_label = new wxStaticText(panel, wxID_ANY, wxString("Max. Level:"));
    min_fort_save_label = new wxStaticText(panel, wxID_ANY, wxString("Min. Fort. Save:"));
    pre_req_epic_label = new wxStaticText(panel, wxID_ANY, wxString(""));

    /*
    * FORM TEXT CONTROLS
    */
    label = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    name = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    description = new wxTextCtrl(panel, wxID_ANY, wxString(""), wxDefaultPosition, wxSize(500, -1), wxTE_MULTILINE);
    icon = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    max_cr = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    cr_value = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    uses_per_day = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    constant = new wxTextCtrl(panel, wxID_ANY, wxString(""));

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

    min_level = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    max_level = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    min_fort_save = new wxTextCtrl(panel, wxID_ANY, wxString(""));

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
    min_level_class = new wxButton(panel, FT_MIN_LEVEL_CLASS, wxString("None"), wxDefaultPosition, wxSize(200, -1));


    gain_multiple = new wxCheckBox(panel, wxID_ANY, wxString("Gain Multiple"));
    effects_stack = new wxCheckBox(panel, wxID_ANY, wxString("Effects Stack"));
    all_classes_can_use = new wxCheckBox(panel, wxID_ANY, wxString("All Classes can use"));
    target_self = new wxCheckBox(panel, wxID_ANY, wxString("Target Self"));
    hostile_feat = new wxCheckBox(panel, wxID_ANY, wxString("Hostile Feat"));
    req_action = new wxCheckBox(panel, wxID_ANY, wxString("Requires action"));
    pre_req_epic = new wxCheckBox(panel, wxID_ANY, wxString("Requires Epic levels:"));

    category = new wxComboBox(panel, wxID_ANY, wxString(""));
    tools_categories = new wxComboBox(panel, wxID_ANY, wxString(""));

    ok_button = new wxButton(panel, wxID_OK, wxString("Ok"));
    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FeatForm::OnOk));

    cancel_button = new wxButton(panel, wxID_CANCEL, wxString("Cancel"));

    Centre();

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* first_row = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* second_row = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* third_row = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* forth_row = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* fifth_row = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sixth_row = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer* label_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* name_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* icon_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* cr_value_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* max_cr_sizer = new wxBoxSizer(wxVERTICAL);

    label_sizer->Add(label_label);
    label_sizer->Add(label, 0, wxEXPAND);
    name_sizer->Add(name_label);
    name_sizer->Add(name, 0, wxEXPAND);
    icon_sizer->Add(icon_label);
    icon_sizer->Add(icon, 0);
    cr_value_sizer->Add(cr_value_label);
    cr_value_sizer->Add(cr_value);
    max_cr_sizer->Add(max_cr_label);
    max_cr_sizer->Add(max_cr);

    first_row->Add(label_sizer, 1);
    first_row->Add(name_sizer, 1);
    first_row->Add(icon_sizer, 0);
    first_row->Add(cr_value_sizer);
    first_row->Add(max_cr_sizer);

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
    wxBoxSizer* spell_id_sizer = new wxBoxSizer(wxVERTICAL);

    checkbox_sizer_1->Add(gain_multiple);
    checkbox_sizer_1->Add(effects_stack);
    checkbox_sizer_2->Add(all_classes_can_use);
    checkbox_sizer_2->Add(target_self);
    category_sizer->Add(category_label);
    category_sizer->Add(category);
    spell_id_sizer->Add(spell_label);
    spell_id_sizer->Add(spell, 0, wxEXPAND);

    second_row->Add(checkbox_sizer_1);
    second_row->Add(checkbox_sizer_2);
    second_row->Add(category_sizer);
    second_row->Add(spell_id_sizer, 1);

    wxBoxSizer* third_row_checkbox_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* constant_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* uses_per_day_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* tools_categories_sizer = new wxBoxSizer(wxVERTICAL);

    third_row_checkbox_sizer->Add(hostile_feat, 0, wxEXPAND);
    third_row_checkbox_sizer->Add(req_action, 0, wxEXPAND);
    constant_sizer->Add(constant_label);
    constant_sizer->Add(constant, 0, wxEXPAND);
    uses_per_day_sizer->Add(uses_per_day_label);
    uses_per_day_sizer->Add(uses_per_day);
    tools_categories_sizer->Add(tools_categories_label);
    tools_categories_sizer->Add(tools_categories, 0, wxEXPAND);

    third_row->Add(third_row_checkbox_sizer);
    third_row->Add(constant_sizer, 1);
    third_row->Add(uses_per_day_sizer);
    third_row->Add(tools_categories_sizer, 1);

    wxBoxSizer* successor_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* master_feat_sizer = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* prereq_sizer = new wxStaticBoxSizer(req_feat_staticbox, wxHORIZONTAL);

    prereq_sizer->Add(pre_req_feat_1, 1);
    prereq_sizer->Add(pre_req_feat_2, 1);
    successor_sizer->Add(successor_label);
    successor_sizer->Add(successor, 0, wxEXPAND);
    master_feat_sizer->Add(master_feat_label);
    master_feat_sizer->Add(master_feat, 0, wxEXPAND);

    forth_row->Add(prereq_sizer, 1, wxEXPAND);
    forth_row->Add(successor_sizer, 1, wxEXPAND);
    forth_row->Add(master_feat_sizer, 1, wxEXPAND);

    wxBoxSizer* description_row = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* forth_row_column_1 = new wxBoxSizer(wxVERTICAL);
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

    forth_row_column_1->Add(req_oneof_sizer, 0, wxEXPAND);
    forth_row_column_1->Add(req_skill_sizer, 0, wxEXPAND);
    description_row->Add(forth_row_column_1, 1, wxEXPAND);
    description_row->Add(description_sizer, 1, wxEXPAND);

    fifth_row->Add(description_row);

    wxBoxSizer* pre_req_epic_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* min_level_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* min_level_class_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* max_level_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* min_fort_save_sizer = new wxBoxSizer(wxVERTICAL);

    pre_req_epic_sizer->Add(pre_req_epic_label);
    pre_req_epic_sizer->Add(pre_req_epic);
    min_level_sizer->Add(min_level_label);
    min_level_sizer->Add(min_level, 0, wxEXPAND);
    min_level_class_sizer->Add(min_level_class_label);
    min_level_class_sizer->Add(min_level_class, 0, wxEXPAND);
    max_level_sizer->Add(max_level_label);
    max_level_sizer->Add(max_level, 0, wxEXPAND);
    min_fort_save_sizer->Add(min_fort_save_label);
    min_fort_save_sizer->Add(min_fort_save, 0, wxEXPAND);

    sixth_row->Add(pre_req_epic_sizer);
    sixth_row->Add(min_level_sizer);
    sixth_row->Add(min_level_class_sizer);
    sixth_row->Add(max_level_sizer);
    sixth_row->Add(min_fort_save_sizer);

    wxBoxSizer* control_button_sizer = new wxBoxSizer(wxHORIZONTAL);

    control_button_sizer->Add(cancel_button);
    control_button_sizer->Add(ok_button);

    main_sizer->Add(first_row, 0, wxEXPAND);
    main_sizer->Add(min_req_sizer, 0, wxEXPAND);
    main_sizer->Add(second_row, 0, wxEXPAND);
    main_sizer->Add(third_row, 0, wxEXPAND);
    main_sizer->Add(forth_row, 0, wxEXPAND);
    main_sizer->Add(fifth_row, 0, wxEXPAND);
    main_sizer->Add(sixth_row, 1, wxEXPAND);
    main_sizer->Add(control_button_sizer, 0, wxALIGN_RIGHT|wxRIGHT|wxBOTTOM, 2);

    panel->SetSizer(main_sizer);

    InitFormValues();
}

void FeatForm::OnOk(wxCommandEvent& event)
{
    (*feat)[GETIDX(FEAT_2DA::Label)].m_Data = GetStringFromTextCtrl(label);
    (*feat)[GETIDX(FEAT_2DA::Feat)].m_Data = GetStrref(name, FEAT_2DA::Feat);
    (*feat)[GETIDX(FEAT_2DA::Description)].m_Data = GetStrref(description, FEAT_2DA::Description);
    (*feat)[GETIDX(FEAT_2DA::Icon)].m_Data = GetStringFromTextCtrl(icon);
    (*feat)[GETIDX(FEAT_2DA::MinAttackBonus)].m_Data = GetStringFromTextCtrl(min_attack);
    (*feat)[GETIDX(FEAT_2DA::MinStr)].m_Data = GetStringFromTextCtrl(min_str);
    (*feat)[GETIDX(FEAT_2DA::MinDex)].m_Data = GetStringFromTextCtrl(min_dex);
    (*feat)[GETIDX(FEAT_2DA::MinInt)].m_Data = GetStringFromTextCtrl(min_int);
    (*feat)[GETIDX(FEAT_2DA::MinWis)].m_Data = GetStringFromTextCtrl(min_wis);
    (*feat)[GETIDX(FEAT_2DA::MinCon)].m_Data = GetStringFromTextCtrl(min_con);
    (*feat)[GETIDX(FEAT_2DA::MinCha)].m_Data = GetStringFromTextCtrl(min_cha);
    (*feat)[GETIDX(FEAT_2DA::MinSpellLvl)].m_Data = GetStringFromTextCtrl(min_spell_lvl);
    (*feat)[GETIDX(FEAT_2DA::PreReqFeat1)].m_Data = pre_req_feat_1_id > 0 ? std::to_string(pre_req_feat_1_id - 1) : std::string("****");
    (*feat)[GETIDX(FEAT_2DA::PreReqFeat2)].m_Data = pre_req_feat_2_id > 0 ? std::to_string(pre_req_feat_2_id - 1) : std::string("****");
    (*feat)[GETIDX(FEAT_2DA::GainMultiple)].m_Data = std::string(gain_multiple->GetValue() ? "1" : "0");
    (*feat)[GETIDX(FEAT_2DA::EffectsStack)].m_Data = std::string(effects_stack->GetValue() ? "1" : "0");
    (*feat)[GETIDX(FEAT_2DA::AllClassesCanUse)].m_Data = std::string(all_classes_can_use->GetValue() ? "1" : "0");
    (*feat)[GETIDX(FEAT_2DA::Category)].m_Data = GetCategoryString();
    (*feat)[GETIDX(FEAT_2DA::MaxCR)].m_Data = GetStringFromTextCtrl(max_cr);
    (*feat)[GETIDX(FEAT_2DA::SpellID)].m_Data = spellid > 0 ? std::to_string(spellid - 1) : std::string("****");
    (*feat)[GETIDX(FEAT_2DA::Successor)].m_Data = successor_id > 0 ? std::to_string(successor_id - 1) : std::string("****");;
    (*feat)[GETIDX(FEAT_2DA::CRValue)].m_Data = GetStringFromTextCtrl(cr_value);
    (*feat)[GETIDX(FEAT_2DA::UsesPerDay)].m_Data = GetStringFromTextCtrl(uses_per_day);
    (*feat)[GETIDX(FEAT_2DA::MasterFeat)].m_Data = master_feat_id > 0 ? std::to_string(master_feat_id - 1) : std::string("****");;
    (*feat)[GETIDX(FEAT_2DA::TargetSelf)].m_Data = std::string(target_self->GetValue() ? "1" : "0");
    (*feat)[GETIDX(FEAT_2DA::OrReqFeat0)].m_Data = or_req_feat_0_id > 0 ? std::to_string(or_req_feat_0_id - 1) : std::string("****");
    (*feat)[GETIDX(FEAT_2DA::OrReqFeat1)].m_Data = or_req_feat_1_id > 0 ? std::to_string(or_req_feat_1_id - 1) : std::string("****");
    (*feat)[GETIDX(FEAT_2DA::OrReqFeat2)].m_Data = or_req_feat_2_id > 0 ? std::to_string(or_req_feat_2_id - 1) : std::string("****");
    (*feat)[GETIDX(FEAT_2DA::OrReqFeat3)].m_Data = or_req_feat_3_id > 0 ? std::to_string(or_req_feat_3_id - 1) : std::string("****");
    (*feat)[GETIDX(FEAT_2DA::OrReqFeat4)].m_Data = or_req_feat_4_id > 0 ? std::to_string(or_req_feat_4_id - 1) : std::string("****");
    (*feat)[GETIDX(FEAT_2DA::ReqSkill)].m_Data = req_skill_1_id > 0 ? std::to_string(or_req_feat_0_id - 1) : std::string("****");
    (*feat)[GETIDX(FEAT_2DA::ReqSkillMinRanks)].m_Data = GetStringFromTextCtrl(req_skill_min_rank_1);
    (*feat)[GETIDX(FEAT_2DA::ReqSkill2)].m_Data = req_skill_2_id > 0 ? std::to_string(req_skill_2_id - 1) : std::string("****");
    (*feat)[GETIDX(FEAT_2DA::ReqSkillMinRanks2)].m_Data = GetStringFromTextCtrl(req_skill_min_rank_2);
    (*feat)[GETIDX(FEAT_2DA::Constant)].m_Data = GetStringFromTextCtrl(constant);
    (*feat)[GETIDX(FEAT_2DA::ToolsCategories)].m_Data = tools_categories->GetSelection() > 0 ? std::to_string(tools_categories->GetSelection()) : std::string("****");
    (*feat)[GETIDX(FEAT_2DA::HostileFeat)].m_Data = std::string(hostile_feat->GetValue() ? "1" : "0");
    (*feat)[GETIDX(FEAT_2DA::MinLevel)].m_Data = GetStringFromTextCtrl(min_level);
    (*feat)[GETIDX(FEAT_2DA::MinLevelClass)].m_Data = min_level_class_id > 0 ? std::to_string(min_level_class_id - 1) : std::string("****");
    (*feat)[GETIDX(FEAT_2DA::MaxLevel)].m_Data = GetStringFromTextCtrl(max_level);
    (*feat)[GETIDX(FEAT_2DA::MinFortSave)].m_Data = GetStringFromTextCtrl(min_fort_save);
    (*feat)[GETIDX(FEAT_2DA::PreReqEpic)].m_Data = std::string(pre_req_epic->GetValue() ? "1" : "0");
    (*feat)[GETIDX(FEAT_2DA::ReqAction)].m_Data = std::string(req_action->GetValue() ? "1" : "0");

    configuration->Set2daModified("feat", true);

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

    SetFeatValue(pre_req_feat_1, pre_req_feat_1_id, Get2DAString(feat, FEAT_2DA::PreReqFeat1));
    SetFeatValue(pre_req_feat_2, pre_req_feat_2_id, Get2DAString(feat, FEAT_2DA::PreReqFeat2));

    SetFeatValue(or_req_feat_0, or_req_feat_0_id, Get2DAString(feat, FEAT_2DA::OrReqFeat0));
    SetFeatValue(or_req_feat_1, or_req_feat_1_id, Get2DAString(feat, FEAT_2DA::OrReqFeat1));
    SetFeatValue(or_req_feat_2, or_req_feat_2_id, Get2DAString(feat, FEAT_2DA::OrReqFeat2));
    SetFeatValue(or_req_feat_3, or_req_feat_3_id, Get2DAString(feat, FEAT_2DA::OrReqFeat3));
    SetFeatValue(or_req_feat_4, or_req_feat_4_id, Get2DAString(feat, FEAT_2DA::OrReqFeat4));

    req_skill_min_rank_1->SetValue(Get2DAString(feat, FEAT_2DA::ReqSkillMinRanks));
    SetSkillValue(req_skill_1, req_skill_1_id, Get2DAString(feat, FEAT_2DA::ReqSkill));
    req_skill_min_rank_2->SetValue(Get2DAString(feat, FEAT_2DA::ReqSkillMinRanks2));
    SetSkillValue(req_skill_2, req_skill_2_id, Get2DAString(feat, FEAT_2DA::ReqSkill2));

    min_level->SetValue(Get2DAString(feat, FEAT_2DA::MinLevel));
    max_level->SetValue(Get2DAString(feat, FEAT_2DA::MaxLevel));
    min_fort_save->SetValue(Get2DAString(feat, FEAT_2DA::MinFortSave));

    std::uint32_t require_epic = GetUintFromString(Get2DAString(feat, FEAT_2DA::PreReqEpic));
    pre_req_epic->SetValue(require_epic > 0);

    std::string aux = Get2DAString(feat, FEAT_2DA::MinLevelClass);
    if (aux.size() > 0)
    {
        min_level_class_id = GetUintFromString(aux) + 1;
        TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("classes", min_level_class_id - 1);
        std::uint32_t strref = GetUintFromString(Get2DAString(row, CLASS_2DA::Name));
        min_level_class->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
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

    GetToolsSelection();
    SetFeatRequirements();
    LoadCategoryValues();
    LoadSpellIdValue();
    LoadMiscellaneousValues();

    SetFeatValue(successor, successor_id, Get2DAString(feat, FEAT_2DA::Successor));
}

void FeatForm::OnPrereqFeat1(wxCommandEvent& event)
{
    InvokeFeatSelection(pre_req_feat_1, pre_req_feat_1_id);
}

void FeatForm::OnPrereqFeat2(wxCommandEvent& event)
{
    InvokeFeatSelection(pre_req_feat_2, pre_req_feat_2_id);
}

void FeatForm::LoadCategoryValues()
{
    TwoDA::Friendly::TwoDA* categories = configuration->Get2da("categories");

    if (categories != NULL)
    {
        category->Append(wxString("None"));
        for (auto const& row : (*categories))
            category->Append(row["Category"].m_Data);

        category->SetSelection(GetUintFromString(Get2DAString(feat, FEAT_2DA::Category)));
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
    InvokeFeatSelection(successor, successor_id);
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

void FeatForm::LoadMiscellaneousValues()
{
    cr_value->SetValue(Get2DAString(feat, FEAT_2DA::CRValue));
    uses_per_day->SetValue(Get2DAString(feat, FEAT_2DA::UsesPerDay));
    constant->SetValue(Get2DAString(feat, FEAT_2DA::Constant));

    std::uint32_t aux = GetUintFromString(Get2DAString(feat, FEAT_2DA::GainMultiple));
    gain_multiple->SetValue(aux > 0);
    aux = GetUintFromString(Get2DAString(feat, FEAT_2DA::EffectsStack));
    effects_stack->SetValue(aux > 0);
    aux = GetUintFromString(Get2DAString(feat, FEAT_2DA::AllClassesCanUse));
    all_classes_can_use->SetValue(aux > 0);
    aux = GetUintFromString(Get2DAString(feat, FEAT_2DA::TargetSelf));
    target_self->SetValue(aux > 0);
    aux = GetUintFromString(Get2DAString(feat, FEAT_2DA::HostileFeat));
    hostile_feat->SetValue(aux > 0);
    aux = GetUintFromString(Get2DAString(feat, FEAT_2DA::ReqAction));
    req_action->SetValue(aux > 0);

    aux = GetUintFromString(Get2DAString(feat, FEAT_2DA::ToolsCategories));
    tools_categories->SetSelection(aux);
}

void FeatForm::OnRequiredSkill1(wxCommandEvent& event)
{
    InvokeSkillSelection(req_skill_1, req_skill_1_id);
}

void FeatForm::OnRequiredSkill2(wxCommandEvent& event)
{
    InvokeSkillSelection(req_skill_2, req_skill_2_id);
}

void FeatForm::OnRequiredOneOf0(wxCommandEvent& event)
{
    InvokeFeatSelection(or_req_feat_0, or_req_feat_0_id);
}

void FeatForm::OnRequiredOneOf1(wxCommandEvent& event)
{
    InvokeFeatSelection(or_req_feat_1, or_req_feat_1_id);
}

void FeatForm::OnRequiredOneOf2(wxCommandEvent& event)
{
    InvokeFeatSelection(or_req_feat_2, or_req_feat_2_id);
}

void FeatForm::OnRequiredOneOf3(wxCommandEvent& event)
{
    InvokeFeatSelection(or_req_feat_3, or_req_feat_3_id);
}

void FeatForm::OnRequiredOneOf4(wxCommandEvent& event)
{
    InvokeFeatSelection(or_req_feat_4, or_req_feat_4_id);
}


void FeatForm::InvokeFeatSelection(wxButton* button, std::uint32_t& variable)
{
    FeatSelectionForm form(panel, configuration, variable);
    if (form.ShowModal() == wxID_OK)
    {
        variable = form.GetFeatSelection();
        if (variable > 0)
        {
            TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("feat", variable - 1);
            std::uint32_t strref = GetUintFromString(Get2DAString(row, FEAT_2DA::Feat));
            button->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
        }
        else
            button->SetLabel("None");
    }
}

void FeatForm::SetFeatValue(wxButton* button, std::uint32_t& variable, const std::string& value)
{
    if (value.size() > 0)
    {
        variable = GetUintFromString(value) + 1;
        TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("feat", variable - 1);
        std::uint32_t strref = GetUintFromString(Get2DAString(row, FEAT_2DA::Feat));
        button->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
    }
    else
        button->SetLabel("None");
}


void FeatForm::InvokeSkillSelection(wxButton* button, std::uint32_t& variable)
{
    SkillSelectionForm form(panel, configuration, variable);
    if (form.ShowModal() == wxID_OK)
    {
        variable = form.GetSkillSelection();
        if (variable > 0)
        {
            TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("skills", variable - 1);
            std::uint32_t strref = GetUintFromString(Get2DAString(row, SKILL_2DA::Name));
            button->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
        }
        else
            button->SetLabel("None");
    }
}

void FeatForm::SetSkillValue(wxButton* button, std::uint32_t& variable, const std::string& value)
{
    if (value.size() > 0)
    {
        variable = GetUintFromString(value) + 1;
        TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("skills", variable - 1);
        std::uint32_t strref = GetUintFromString(Get2DAString(row, SKILL_2DA::Name));
        button->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
    }
    else
        button->SetLabel("None");
}

void FeatForm::GetToolsSelection()
{
    tools_categories->Append(wxString("None"));
    tools_categories->Append(wxString("Combat Feat"));
    tools_categories->Append(wxString("Active Combat Feat"));
    tools_categories->Append(wxString("Defensive Feat"));
    tools_categories->Append(wxString("Magical Feat"));
    tools_categories->Append(wxString("Class/Racial Feat"));
    tools_categories->Append(wxString("Other Feat"));
}

void FeatForm::OnMinLevelClass(wxCommandEvent& event)
{
    ClassSelectionForm form(panel, configuration, min_level_class_id);
    if (form.ShowModal() == wxID_OK)
    {
        min_level_class_id = form.GetClassSelection();
        if (min_level_class_id > 0)
        {
            TwoDA::Friendly::TwoDARow* row = configuration->Get2daRow("classes", min_level_class_id - 1);
            std::uint32_t strref = GetUintFromString(Get2DAString(row, CLASS_2DA::Name));
            min_level_class->SetLabel(strref > 0 ? configuration->GetTlkString(strref) : "");
        }
        else
            min_level_class->SetLabel("None");
    }
}

std::string FeatForm::GetStrref(wxTextCtrl* component, const auto& column)
{
    std::uint32_t strref = GetUintFromString(Get2DAString(feat, column));
    if (component->GetValue().IsEmpty())
        return std::string("****");

    std::string aux = component->GetValue().ToStdString();
    std::string base_desc = configuration->GetTlkString(strref);
    if (base_desc != aux)
        strref = configuration->SetTlkString(aux, strref);

    return std::to_string(strref);
}

std::string FeatForm::GetCategoryString()
{
    if (category->GetSelection() == 0)
        return std::string("****");

    return std::to_string(category->GetSelection());
}
