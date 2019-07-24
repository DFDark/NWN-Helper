#include "spell-column-form.hpp"

enum
{
    ENABLED_COLUMNS = wxID_HIGHEST + 1,
    DISABLED_COLUMNS,
};

wxBEGIN_EVENT_TABLE(SpellColumnForm, wxDialog)
    EVT_MENU(wxID_OK, SpellColumnForm::OnOk)
    EVT_MENU(wxID_CANCEL, SpellColumnForm::OnCancel)
    EVT_LISTBOX_DCLICK(ENABLED_COLUMNS, SpellColumnForm::OnEnabledDoubleClick)
    EVT_LISTBOX_DCLICK(DISABLED_COLUMNS, SpellColumnForm::OnDisabledDoubleClick)
wxEND_EVENT_TABLE()

SpellColumnForm::SpellColumnForm(wxWindow* parent, ConfigurationManager* _configuration) :
    wxDialog(parent, wxID_ANY, wxString("Spell Columns"), wxDefaultPosition, wxSize(400, 300))
{
    configuration = _configuration;

    enabled = new wxListBox(this, ENABLED_COLUMNS, wxPoint(5, 5), wxSize(150, 150));
    disabled = new wxListBox(this, DISABLED_COLUMNS, wxPoint(160, 5), wxSize(150, 150));
    SetInitialColumns();

    ok_button = new wxButton(this, wxID_OK, wxString("Ok"), wxPoint(295, 235), wxSize(100, 30));
    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SpellColumnForm::OnOk));

    cancel_button = new wxButton(this, wxID_CANCEL, wxString("Cancel"), wxPoint(190, 235), wxSize(100, 30));

    Centre();
}

void SpellColumnForm::OnOk(wxCommandEvent& event)
{
    std::vector<std::string> cols;
    for (auto const& col : enabled->GetStrings())
        cols.emplace_back(col.ToStdString());

    configuration->SetSpellColumns(cols);
    this->EndModal(wxID_OK);
}

void SpellColumnForm::OnCancel(wxCommandEvent& event)
{
    this->EndModal(wxID_CANCEL);
}

void SpellColumnForm::SetInitialColumns()
{
    std::map<std::string, bool> columns;
    columns["label"] = false;
    columns["spell"] = false;
    columns["school"] = false;
    columns["range"] = false;

    for (auto const& column : configuration->GetSpellColumns())
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

void SpellColumnForm::OnEnabledDoubleClick(wxCommandEvent& event)
{
    int selection = enabled->GetSelection();
    wxString item = enabled->GetString(selection);

    enabled->Delete(selection);
    disabled->Append(item);
}

void SpellColumnForm::OnDisabledDoubleClick(wxCommandEvent& event)
{
    int selection = disabled->GetSelection();
    wxString item = disabled->GetString(selection);

    disabled->Delete(selection);
    enabled->Append(item);
}
