#include "export-form.hpp"

enum
{
    DIRECTORY_BUTTON = wxID_HIGHEST + 1,
};

wxBEGIN_EVENT_TABLE(ExportForm, wxDialog)
    EVT_MENU(wxID_OK, ExportForm::OnOk)
    EVT_MENU(DIRECTORY_BUTTON, ExportForm::OnFindDirectoryClick)
wxEND_EVENT_TABLE()

ExportForm::ExportForm(wxWindow* parent) :
    wxDialog(parent, wxID_ANY, wxString("Export Form"), wxDefaultPosition, wxSize(480, 320))
{
    destination_label = new wxStaticText(this, wxID_ANY, wxString("Project directory"));
    destination = new wxTextCtrl(this, wxID_ANY, wxString(""));

    tlk_filename_label = new wxStaticText(this, wxID_ANY, wxString("TLK filename"));
    tlk_filename = new wxTextCtrl(this, wxID_ANY, wxString(""));

    project_name_label = new wxStaticText(this, wxID_ANY, wxString("Project name"));
    project_name = new wxTextCtrl(this, wxID_ANY, wxString(""));

    destination_button = new wxButton(this, DIRECTORY_BUTTON, wxString("Find"));
    Connect(DIRECTORY_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ExportForm::OnFindDirectoryClick));

    ok_button = new wxButton(this, wxID_OK, wxString("Ok"));
    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ExportForm::OnOk));
    cancel_button = new wxButton(this, wxID_CANCEL, wxString("Cancel"));

    Centre();

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* first_row_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* destination_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* destination_sizer_r2 = new wxBoxSizer(wxHORIZONTAL);

    destination_sizer->Add(destination_label);

    destination_sizer_r2->Add(destination, 1);
    destination_sizer_r2->Add(destination_button);

    destination_sizer->Add(destination_sizer_r2, 1, wxEXPAND);

    wxBoxSizer* project_name_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* tlk_filename_sizer = new wxBoxSizer(wxVERTICAL);

    project_name_sizer->Add(project_name_label);
    project_name_sizer->Add(project_name);

    tlk_filename_sizer->Add(tlk_filename_label);
    tlk_filename_sizer->Add(tlk_filename);

    first_row_sizer->Add(project_name_sizer);
    first_row_sizer->Add(tlk_filename_sizer);

    wxBoxSizer* buttons_sizer = new wxBoxSizer(wxHORIZONTAL);

    buttons_sizer->Add(cancel_button);
    buttons_sizer->Add(ok_button);

    main_sizer->Add(first_row_sizer, 0, wxEXPAND);
    main_sizer->Add(destination_sizer, 0, wxEXPAND);
    main_sizer->Add(buttons_sizer, 1, wxALIGN_RIGHT|wxRIGHT|wxBOTTOM, 2);

    this->SetSizer(main_sizer);
}

void ExportForm::OnOk(wxCommandEvent& event)
{
    if (destination->GetValue().IsEmpty())
    {
        wxMessageBox("Output directory must be entered to export files!",
            "Error", wxOK|wxICON_ERROR);
        return;
    }

    if (tlk_filename->GetValue().IsEmpty())
    {
        wxMessageBox("TLK filename must be entered to export files!",
            "Error", wxOK|wxICON_ERROR);
        return;
    }

    if (project_name->GetValue().IsEmpty())
    {
        wxMessageBox("Project name must be entered to export files!",
            "Error", wxOK|wxICON_ERROR);
        return;
    }

    this->EndModal(wxID_OK);
}

void ExportForm::OnFindDirectoryClick(wxCommandEvent& event)
{
    wxDirDialog destination_dialog(this, wxString("Locate folder to save current project to"), "");

    if (destination_dialog.ShowModal() == wxID_CANCEL)
        return;

    destination->SetValue(destination_dialog.GetPath());
}

std::string ExportForm::GetProjectName()
{
    return std::string(project_name->GetValue());
}

std::string ExportForm::GetBasePath()
{
    return std::string(destination->GetValue());
}

std::string ExportForm::GetTLKName()
{
    return std::string(tlk_filename->GetValue());
}
