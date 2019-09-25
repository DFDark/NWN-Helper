#include "skill-column-form.hpp"

enum
{
    ENABLED_COLUMNS = wxID_HIGHEST + 1,
    DISABLED_COLUMNS,
};

wxBEGIN_EVENT_TABLE(SkillColumnForm, wxDialog)
    EVT_MENU(wxID_OK, SkillColumnForm::OnOk)
    EVT_MENU(wxID_CANCEL, SkillColumnForm::OnCancel)
    EVT_LISTBOX_DCLICK(ENABLED_COLUMNS, SkillColumnForm::OnEnabledDoubleClick)
    EVT_LISTBOX_DCLICK(DISABLED_COLUMNS, SkillColumnForm::OnDisabledDoubleClick)
wxEND_EVENT_TABLE()

SkillColumnForm::SkillColumnForm(wxWindow* parent, ConfigurationManager* _configuration) :
    wxDialog(parent, wxID_ANY, wxString("Skill Columns"), wxDefaultPosition, wxSize(400, 300))
{
    configuration = _configuration;

    enabled = new wxListBox(this, ENABLED_COLUMNS);
    disabled = new wxListBox(this, DISABLED_COLUMNS);
    SetInitialColumns();

    enabled_label = new wxStaticText(this, wxID_ANY, wxString("Enabled:"));
    disabled_label = new wxStaticText(this,  wxID_ANY, wxString("Disabled:"));

    ok_button = new wxButton(this, wxID_OK, wxString("Ok"));
    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SkillColumnForm::OnOk));

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

void SkillColumnForm::OnOk(wxCommandEvent& event)
{
    std::vector<std::string> cols;
    for (auto const& col : enabled->GetStrings())
        cols.emplace_back(col.ToStdString());

    configuration->SetColumns(SKILL_COLUMNS, cols);
    this->EndModal(wxID_OK);
}

void SkillColumnForm::OnCancel(wxCommandEvent& event)
{
    this->EndModal(wxID_CANCEL);
}

void SkillColumnForm::SetInitialColumns()
{
    std::map<std::string, bool> columns;
    columns["label"] = false;
    columns["skill"] = false;

    for (auto const& column : configuration->GetColumns(SKILL_COLUMNS))
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

void SkillColumnForm::OnEnabledDoubleClick(wxCommandEvent& event)
{
    int selection = enabled->GetSelection();
    wxString item = enabled->GetString(selection);

    enabled->Delete(selection);
    disabled->Append(item);
}

void SkillColumnForm::OnDisabledDoubleClick(wxCommandEvent& event)
{
    int selection = disabled->GetSelection();
    wxString item = disabled->GetString(selection);

    disabled->Delete(selection);
    enabled->Append(item);
}
