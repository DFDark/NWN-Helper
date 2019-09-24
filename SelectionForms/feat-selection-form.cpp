#include "feat-selection-form.hpp"

enum
{
    FEATS = wxID_HIGHEST + 1,
};

wxBEGIN_EVENT_TABLE(FeatSelectionForm, wxDialog)
    EVT_MENU(wxID_OK, FeatSelectionForm::OnOk)
    EVT_SHOW(FeatSelectionForm::OnShow)
    EVT_DATAVIEW_ITEM_ACTIVATED(FEATS, FeatSelectionForm::OnDoubleClick)
wxEND_EVENT_TABLE()

FeatSelectionForm::FeatSelectionForm(wxWindow* parent, ConfigurationManager* _configuration, const std::uint32_t& _selection) :
    wxDialog(parent, wxID_ANY, wxString("Feat selection form"), wxDefaultPosition, wxSize(480, 480))
{
    configuration = _configuration;
    selection = _selection;

    feats = new wxDataViewCtrl(this, FEATS);
    TwoDA::Friendly::TwoDA* twoda = configuration->Get2da("feat");

    ft_model = new FeatListModel(twoda, configuration, true);
    feats->AssociateModel(ft_model);
    SetFeatColumns();

    ok_button = new wxButton(this, wxID_OK, wxString("Ok"));
    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FeatSelectionForm::OnOk));
    cancel_button = new wxButton(this, wxID_CANCEL, wxString("Cancel"));

    Centre();

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* destination_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* buttons_sizer = new wxBoxSizer(wxHORIZONTAL);

    destination_sizer->Add(feats, 1, wxEXPAND);
    buttons_sizer->Add(cancel_button);
    buttons_sizer->Add(ok_button);

    main_sizer->Add(destination_sizer, 1, wxEXPAND);
    main_sizer->Add(buttons_sizer, 0, wxALIGN_RIGHT|wxRIGHT|wxBOTTOM, 2);

    this->SetSizer(main_sizer);
}

void FeatSelectionForm::OnOk(wxCommandEvent& event)
{
    if (!feats->HasSelection()) {
        wxMessageBox("No value selected!", "Error", wxOK|wxICON_ERROR);
        return;
    }

    TwoDA::Friendly::TwoDARow* row = ft_model->Get2daRow(ft_model->GetRow(feats->GetSelection()));
    selection = row->RowId();

    this->EndModal(wxID_OK);
}

void FeatSelectionForm::OnDoubleClick(wxDataViewEvent& event)
{
    TwoDA::Friendly::TwoDARow* row = ft_model->Get2daRow(ft_model->GetRow(event.GetItem()));
    selection = row->RowId();

    this->EndModal(wxID_OK);
}

void FeatSelectionForm::SetFeatColumns()
{
    feats->ClearColumns();
    feats->AppendTextColumn("ID", FeatListModel::ID);
    for (auto const& col : configuration->GetColumns(FEAT_COLUMNS))
    {
        if (col == "label")
            feats->AppendTextColumn("Label", FeatListModel::LABEL);
        else if (col == "feat")
            feats->AppendTextColumn("Feat", FeatListModel::FEAT);
        else if (col == "min_attack_bonus")
            feats->AppendTextColumn("Min. AB", FeatListModel::MIN_ATTACK_BONUS);
        else if (col == "min_str")
            feats->AppendTextColumn("Min. Str.", FeatListModel::MIN_STR);
        else if (col == "min_dex")
            feats->AppendTextColumn("Min. Dex", FeatListModel::MIN_DEX);
        else if (col == "min_int")
            feats->AppendTextColumn("Min. Int", FeatListModel::MIN_INT);
        else if (col == "min_wiz")
            feats->AppendTextColumn("Min. Wiz", FeatListModel::MIN_WIS);
        else if (col == "min_con")
            feats->AppendTextColumn("Min. Con", FeatListModel::MIN_CON);
        else if (col == "min_cha")
            feats->AppendTextColumn("Min. Cha", FeatListModel::MIN_CHA);
        else if (col == "min_spell_level")
            feats->AppendTextColumn("Min. Spell Lvl.", FeatListModel::MIN_SPELL_LVL);
    }
}

std::uint32_t FeatSelectionForm::GetFeatSelection()
{
    return selection;
}

void FeatSelectionForm::OnShow(wxShowEvent& event)
{
    if (event.IsShown())
    {
        feats->Select(ft_model->GetItem(selection));
        feats->EnsureVisible(ft_model->GetItem(selection));
    }
}
