#include "master-feat-selection-form.hpp"

enum
{
    MASTER_FEATS = wxID_HIGHEST + 1,
};

wxBEGIN_EVENT_TABLE(MasterFeatSelectionForm, wxDialog)
    EVT_MENU(wxID_OK, MasterFeatSelectionForm::OnOk)
    EVT_SHOW(MasterFeatSelectionForm::OnShow)
    EVT_DATAVIEW_ITEM_ACTIVATED(MASTER_FEATS, MasterFeatSelectionForm::OnDoubleClick)
wxEND_EVENT_TABLE()

MasterFeatSelectionForm::MasterFeatSelectionForm(wxWindow* parent, ConfigurationManager* _configuration, const std::uint32_t& _selection) :
    wxDialog(parent, wxID_ANY, wxString("Master Feat selection form"), wxDefaultPosition, wxSize(480, 480))
{
    configuration = _configuration;
    selection = _selection;

    master_feats = new wxDataViewCtrl(this, MASTER_FEATS);
    TwoDA::Friendly::TwoDA* twoda = configuration->Get2da("masterfeats");

    master_ft_model = new MasterFeatListModel(twoda, configuration, true);
    master_feats->AssociateModel(master_ft_model);
    SetMasterFeatColumns();

    ok_button = new wxButton(this, wxID_OK, wxString("Ok"));
    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MasterFeatSelectionForm::OnOk));
    cancel_button = new wxButton(this, wxID_CANCEL, wxString("Cancel"));

    Centre();

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* destination_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* buttons_sizer = new wxBoxSizer(wxHORIZONTAL);

    destination_sizer->Add(master_feats, 1, wxEXPAND);
    buttons_sizer->Add(cancel_button);
    buttons_sizer->Add(ok_button);

    main_sizer->Add(destination_sizer, 1, wxEXPAND);
    main_sizer->Add(buttons_sizer, 0, wxALIGN_RIGHT|wxRIGHT|wxBOTTOM, 2);

    this->SetSizer(main_sizer);
}

void MasterFeatSelectionForm::OnOk(wxCommandEvent& event)
{
    if (!master_feats->HasSelection()) {
        wxMessageBox("No value selected!", "Error", wxOK|wxICON_ERROR);
        return;
    }

    TwoDA::Friendly::TwoDARow* row = master_ft_model->Get2daRow(master_ft_model->GetRow(master_feats->GetSelection()));
    selection = row->RowId();

    this->EndModal(wxID_OK);
}

void MasterFeatSelectionForm::OnDoubleClick(wxDataViewEvent& event)
{
    TwoDA::Friendly::TwoDARow* row = master_ft_model->Get2daRow(master_ft_model->GetRow(event.GetItem()));
    selection = row->RowId();

    this->EndModal(wxID_OK);
}

void MasterFeatSelectionForm::SetMasterFeatColumns()
{
    master_feats->ClearColumns();
    master_feats->AppendTextColumn("ID", MasterFeatListModel::ID);
    for (auto const& col : configuration->GetColumns(MASTER_FEAT_COLUMNS))
    {
        if (col == "label")
            master_feats->AppendTextColumn("Label", MasterFeatListModel::LABEL);
        else if (col == "master_feat")
            master_feats->AppendTextColumn("Master Feat", MasterFeatListModel::STRREF);
        else if (col == "icon")
            master_feats->AppendTextColumn("Icon", MasterFeatListModel::ICON);
    }
}

std::uint32_t MasterFeatSelectionForm::GetMasterFeatSelection()
{
    return selection;
}

void MasterFeatSelectionForm::OnShow(wxShowEvent& event)
{
    if (event.IsShown())
    {
        master_feats->Select(master_ft_model->GetItem(selection));
        master_feats->EnsureVisible(master_ft_model->GetItem(selection));
    }
}
