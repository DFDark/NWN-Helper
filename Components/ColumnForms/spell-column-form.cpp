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

    enabled = new wxListBox(this, ENABLED_COLUMNS);
    disabled = new wxListBox(this, DISABLED_COLUMNS);
    SetInitialColumns();

    enabled_label = new wxStaticText(this, wxID_ANY, wxString("Enabled:"));
    disabled_label = new wxStaticText(this,  wxID_ANY, wxString("Disabled:"));

    ok_button = new wxButton(this, wxID_OK, wxString("Ok"));
    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SpellColumnForm::OnOk));

    cancel_button = new wxButton(this, wxID_CANCEL, wxString("Cancel"));

    Centre();

    // Sizers
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

void SpellColumnForm::OnOk(wxCommandEvent& event)
{
    std::vector<std::string> cols;
    for (auto const& col : enabled->GetStrings())
        cols.emplace_back(col.ToStdString());

    configuration->SetColumns(SPELL_COLUMNS, cols);
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
    columns["vs"] = false;
    columns["metamagic"] = false;
    columns["targettype"] = false;
    columns["impactscript"] = false;
    columns["bard"] = false;
    columns["cleric"] = false;
    columns["druid"] = false;
    columns["palading"] = false;
    columns["ranger"] = false;
    columns["wiz_sorc"] = false;
    columns["innate"] = false;

    for (auto const& column : configuration->GetColumns(SPELL_COLUMNS))
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
