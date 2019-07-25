#include "feat-column-form.hpp"

enum
{
    ENABLED_COLUMNS = wxID_HIGHEST + 1,
    DISABLED_COLUMNS,
};

wxBEGIN_EVENT_TABLE(FeatColumnForm, wxDialog)
    EVT_MENU(wxID_OK, FeatColumnForm::OnOk)
    EVT_MENU(wxID_CANCEL, FeatColumnForm::OnCancel)
    EVT_LISTBOX_DCLICK(ENABLED_COLUMNS, FeatColumnForm::OnEnabledDoubleClick)
    EVT_LISTBOX_DCLICK(DISABLED_COLUMNS, FeatColumnForm::OnDisabledDoubleClick)
wxEND_EVENT_TABLE()

FeatColumnForm::FeatColumnForm(wxWindow* parent, ConfigurationManager* _configuration) :
    wxDialog(parent, wxID_ANY, wxString("Feat Columns"), wxDefaultPosition, wxSize(400, 300))
{
    configuration = _configuration;

    enabled = new wxListBox(this, ENABLED_COLUMNS, wxPoint(10, 35), wxSize(150, 150));
    disabled = new wxListBox(this, DISABLED_COLUMNS, wxPoint(160, 35), wxSize(150, 150));
    SetInitialColumns();
    
    enabled_label = new wxStaticText(this, wxID_ANY, wxString("Enabled:"), wxPoint(10, 10));
    disabled_label = new wxStaticText(this,  wxID_ANY, wxString("Disabled:"), wxPoint(160, 10));

    ok_button = new wxButton(this, wxID_OK, wxString("Ok"), wxPoint(295, 235), wxSize(100, 30));
    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FeatColumnForm::OnOk));

    cancel_button = new wxButton(this, wxID_CANCEL, wxString("Cancel"), wxPoint(190, 235), wxSize(100, 30));

    Centre();
}

void FeatColumnForm::OnOk(wxCommandEvent& event)
{
    std::vector<std::string> cols;
    for (auto const& col : enabled->GetStrings())
        cols.emplace_back(col.ToStdString());

    configuration->SetFeatColumns(cols);
    this->EndModal(wxID_OK);
}

void FeatColumnForm::OnCancel(wxCommandEvent& event)
{
    this->EndModal(wxID_CANCEL);
}

void FeatColumnForm::SetInitialColumns()
{
    std::map<std::string, bool> columns;
    columns["label"] = false;
    columns["feat"] = false;

    for (auto const& column : configuration->GetFeatColumns())
    {
        columns[column] = true;
        enabled->Append(wxString(column));
    }

    for (auto const& kvp : columns)
    {
        if (!kvp.second)
            disabled->Append(wxString(kvp.first));
    }
}

void FeatColumnForm::OnEnabledDoubleClick(wxCommandEvent& event)
{
    int selection = enabled->GetSelection();
    wxString item = enabled->GetString(selection);

    enabled->Delete(selection);
    disabled->Append(item);
}

void FeatColumnForm::OnDisabledDoubleClick(wxCommandEvent& event)
{
    int selection = disabled->GetSelection();
    wxString item = disabled->GetString(selection);

    disabled->Delete(selection);
    enabled->Append(item);
}
