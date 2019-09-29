#include "class-selection-form.hpp"

enum
{
    CLASSES = wxID_HIGHEST + 1,
};

wxBEGIN_EVENT_TABLE(ClassSelectionForm, wxDialog)
    EVT_MENU(wxID_OK, ClassSelectionForm::OnOk)
    EVT_SHOW(ClassSelectionForm::OnShow)
    EVT_DATAVIEW_ITEM_ACTIVATED(CLASSES, ClassSelectionForm::OnDoubleClick)
wxEND_EVENT_TABLE()

ClassSelectionForm::ClassSelectionForm(wxWindow* parent, ConfigurationManager* _configuration, const std::uint32_t& _selection) :
    wxDialog(parent, wxID_ANY, wxString("Class selection form"), wxDefaultPosition, wxSize(480, 480))
{
    configuration = _configuration;
    selection = _selection;

    classes = new wxDataViewCtrl(this, CLASSES);
    TwoDA::Friendly::TwoDA* twoda = configuration->Get2da("classes");

    cls_model = new ClassListModel(twoda, configuration, true);
    classes->AssociateModel(cls_model);
    SetClassColumns();

    ok_button = new wxButton(this, wxID_OK, wxString("Ok"));
    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ClassSelectionForm::OnOk));
    cancel_button = new wxButton(this, wxID_CANCEL, wxString("Cancel"));

    Centre();

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* destination_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* buttons_sizer = new wxBoxSizer(wxHORIZONTAL);

    destination_sizer->Add(classes, 1, wxEXPAND);
    buttons_sizer->Add(cancel_button);
    buttons_sizer->Add(ok_button);

    main_sizer->Add(destination_sizer, 1, wxEXPAND);
    main_sizer->Add(buttons_sizer, 0, wxALIGN_RIGHT|wxRIGHT|wxBOTTOM, 2);

    this->SetSizer(main_sizer);
}

void ClassSelectionForm::OnOk(wxCommandEvent& event)
{
    if (!classes->HasSelection()) {
        wxMessageBox("No value selected!", "Error", wxOK|wxICON_ERROR);
        return;
    }

    TwoDA::Friendly::TwoDARow* row = cls_model->Get2daRow(cls_model->GetRow(classes->GetSelection()));
    selection = row->RowId();

    this->EndModal(wxID_OK);
}

void ClassSelectionForm::OnDoubleClick(wxDataViewEvent& event)
{
    TwoDA::Friendly::TwoDARow* row = cls_model->Get2daRow(cls_model->GetRow(event.GetItem()));
    selection = row->RowId();

    this->EndModal(wxID_OK);
}

void ClassSelectionForm::SetClassColumns()
{
    classes->ClearColumns();
    classes->AppendTextColumn("ID", ClassListModel::ID);
    for (auto const& col : configuration->GetColumns(CLASS_COLUMNS))
    {
        if (col == "label")
            classes->AppendTextColumn("Label", ClassListModel::LABEL);
        else if (col == "class")
            classes->AppendTextColumn("Class", ClassListModel::CLASS);
        else if (col == "hit_die")
            classes->AppendTextColumn("HD", ClassListModel::HIT_DIE);
        else if (col == "player_class")
            classes->AppendTextColumn("Player Class", ClassListModel::PLAYER_CLASS);
        else if (col == "spell_caster")
            classes->AppendTextColumn("Spell Caster", ClassListModel::SPELL_CASTER);
        else if (col == "strength")
            classes->AppendTextColumn("Str", ClassListModel::STRENGTH);
        else if (col == "dexterity")
            classes->AppendTextColumn("Dex", ClassListModel::DEXTERITY);
        else if (col == "constant")
            classes->AppendTextColumn("Con", ClassListModel::CONSTANT);
        else if (col == "wisdom")
            classes->AppendTextColumn("Wis", ClassListModel::WISDOM);
        else if (col == "intelligence")
            classes->AppendTextColumn("Int", ClassListModel::INTELLIGENCE);
        else if (col == "charisma")
            classes->AppendTextColumn("Cha", ClassListModel::CHARISMA);
        else if (col == "primary_ability")
            classes->AppendTextColumn("Primary Ability", ClassListModel::PRIMARY_ABILITY);
    }
}

std::uint32_t ClassSelectionForm::GetClassSelection()
{
    return selection;
}

void ClassSelectionForm::OnShow(wxShowEvent& event)
{
    if (event.IsShown())
    {
        classes->Select(cls_model->GetItem(selection));
        classes->EnsureVisible(cls_model->GetItem(selection));
    }
}
