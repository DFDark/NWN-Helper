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

    enabled = new wxListBox(this, ENABLED_COLUMNS);
    disabled = new wxListBox(this, DISABLED_COLUMNS);
    SetInitialColumns();

    enabled_label = new wxStaticText(this, wxID_ANY, wxString("Enabled:"));
    disabled_label = new wxStaticText(this,  wxID_ANY, wxString("Disabled:"));

    ok_button = new wxButton(this, wxID_OK, wxString("Ok"));
    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FeatColumnForm::OnOk));

    cancel_button = new wxButton(this, wxID_CANCEL, wxString("Cancel"));

    Centre();

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* enabled_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* disabled_sizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* horizon = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* buttons = new wxBoxSizer(wxHORIZONTAL);

    enabled_sizer->Add(enabled_label, 0, wxEXPAND|wxALL);
    enabled_sizer->Add(enabled, 1, wxEXPAND|wxALL);
    disabled_sizer->Add(disabled_label, 0, wxEXPAND|wxALL);
    disabled_sizer->Add(disabled, 1, wxEXPAND|wxALL);

    buttons->Add(cancel_button, 0, wxALL);
    buttons->Add(ok_button, 0, wxALL);

    horizon->Add(enabled_sizer, 1, wxEXPAND|wxALL);
    horizon->Add(disabled_sizer, 1, wxEXPAND|wxALL);

    main_sizer->Add(horizon, 1, wxEXPAND|wxALL);
    main_sizer->Add(buttons, 0, wxALIGN_RIGHT|wxRIGHT|wxBOTTOM, 2);

    this->SetSizer(main_sizer);
}

void FeatColumnForm::OnOk(wxCommandEvent& event)
{
    std::vector<std::string> cols;
    for (auto const& col : enabled->GetStrings())
        cols.emplace_back(col.ToStdString());

    configuration->SetColumns(FEAT_COLUMNS, cols);
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
    columns["min_attack_bonus"] = false;
    columns["min_str"] = false;
    columns["min_dex"] = false;
    columns["min_int"] = false;
    columns["min_wiz"] = false;
    columns["min_con"] = false;
    columns["min_cha"] = false;
    columns["min_spell_level"] = false;

    for (auto const& column : configuration->GetColumns(FEAT_COLUMNS))
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
