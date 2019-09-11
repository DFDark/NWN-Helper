#include "feat-selection-form.hpp"

enum
{
    FEATS = wxID_HIGHEST + 1,
};

wxBEGIN_EVENT_TABLE(FeatSelectionForm, wxDialog)
    EVT_MENU(wxID_OK, FeatSelectionForm::OnOk)
    EVT_DATAVIEW_ITEM_ACTIVATED(FEATS, FeatSelectionForm::OnDoubleClick)
wxEND_EVENT_TABLE()

FeatSelectionForm::FeatSelectionForm(wxWindow* parent, ConfigurationManager* _configuration) :
    wxDialog(parent, wxID_ANY, wxString("Feat selection form"), wxDefaultPosition, wxSize(480, 480))
{
    configuration = _configuration;

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
    for (auto const& col : configuration->GetFeatColumns())
    {
        if (col == "label")
            feats->AppendTextColumn("Label", FeatListModel::LABEL);
        else if (col == "feat")
            feats->AppendTextColumn("Feat", FeatListModel::FEAT);
    }
}

std::uint32_t FeatSelectionForm::GetFeatSelection()
{
    return selection;
}