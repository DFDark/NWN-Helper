#include "skill-selection-form.hpp"

enum
{
    SKILLS = wxID_HIGHEST + 1,
};

wxBEGIN_EVENT_TABLE(SkillSelectionForm, wxDialog)
    EVT_MENU(wxID_OK, SkillSelectionForm::OnOk)
    EVT_SHOW(SkillSelectionForm::OnShow)
    EVT_DATAVIEW_ITEM_ACTIVATED(SKILLS, SkillSelectionForm::OnDoubleClick)
wxEND_EVENT_TABLE()

SkillSelectionForm::SkillSelectionForm(wxWindow* parent, ConfigurationManager* _configuration, const std::uint32_t& _selection) :
    wxDialog(parent, wxID_ANY, wxString("Skill selection form"), wxDefaultPosition, wxSize(480, 480))
{
    configuration = _configuration;
    selection = _selection;

    skills = new wxDataViewCtrl(this, SKILLS);
    TwoDA::Friendly::TwoDA* twoda = configuration->Get2da("skills");

    sk_model = new SkillListModel(twoda, configuration, true);
    skills->AssociateModel(sk_model);
    SetSkillColumns();

    ok_button = new wxButton(this, wxID_OK, wxString("Ok"));
    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SkillSelectionForm::OnOk));
    cancel_button = new wxButton(this, wxID_CANCEL, wxString("Cancel"));

    Centre();

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* destination_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* buttons_sizer = new wxBoxSizer(wxHORIZONTAL);

    destination_sizer->Add(skills, 1, wxEXPAND);
    buttons_sizer->Add(cancel_button);
    buttons_sizer->Add(ok_button);

    main_sizer->Add(destination_sizer, 1, wxEXPAND);
    main_sizer->Add(buttons_sizer, 0, wxALIGN_RIGHT|wxRIGHT|wxBOTTOM, 2);

    this->SetSizer(main_sizer);
}

void SkillSelectionForm::OnOk(wxCommandEvent& event)
{
    if (!skills->HasSelection()) {
        wxMessageBox("No value selected!", "Error", wxOK|wxICON_ERROR);
        return;
    }

    TwoDA::Friendly::TwoDARow* row = sk_model->Get2daRow(sk_model->GetRow(skills->GetSelection()));
    selection = row->RowId();

    this->EndModal(wxID_OK);
}

void SkillSelectionForm::OnDoubleClick(wxDataViewEvent& event)
{
    TwoDA::Friendly::TwoDARow* row = sk_model->Get2daRow(sk_model->GetRow(event.GetItem()));
    selection = row->RowId();

    this->EndModal(wxID_OK);
}

void SkillSelectionForm::SetSkillColumns()
{
    skills->ClearColumns();
    skills->AppendTextColumn("ID", SkillListModel::ID);
    for (auto const& col : configuration->GetColumns(SKILL_COLUMNS))
    {
        if (col == "label")
            skills->AppendTextColumn("Label", SkillListModel::LABEL);
        else if (col == "skill")
            skills->AppendTextColumn("Skill", SkillListModel::SKILL);
        else if (col == "untrained")
            skills->AppendTextColumn("Untrained", SkillListModel::UNTRAINED);
        else if (col == "key_ability")
            skills->AppendTextColumn("Key ability", SkillListModel::KEY_ABILITY);
        else if (col == "armor_check_penalty")
            skills->AppendTextColumn("Armor check penalty", SkillListModel::ARMOR_CHECK_PENALTY);
        else if (col == "all_classes_can_use")
            skills->AppendTextColumn("All classes can use", SkillListModel::ALL_CLASSES_CAN_USE);
        else if (col == "category")
            skills->AppendTextColumn("Category", SkillListModel::CATEGORY);
        else if (col == "max_cr")
            skills->AppendTextColumn("Max CR", SkillListModel::MAX_CR);
        else if (col == "constant")
            skills->AppendTextColumn("Constant", SkillListModel::CONSTANT);
        else if (col == "hostile_skill")
            skills->AppendTextColumn("Hostile", SkillListModel::HOSTILE_SKILL);
    }
}

std::uint32_t SkillSelectionForm::GetSkillSelection()
{
    return selection;
}

void SkillSelectionForm::OnShow(wxShowEvent& event)
{
    if (event.IsShown())
    {
        skills->Select(sk_model->GetItem(selection));
        skills->EnsureVisible(sk_model->GetItem(selection));
    }
}
