#include "export-form.hpp"

wxBEGIN_EVENT_TABLE(ExportForm, wxDialog)
    EVT_MENU(wxID_OK, ExportForm::OnOk)
wxEND_EVENT_TABLE()

ExportForm::ExportForm(wxWindow* parent, ConfigurationManager* _configuration) :
    wxDialog(parent, wxID_ANY, wxString("Export Form"), wxDefaultPosition, wxSize(640, 480))
{
    configuration = _configuration;

    ok_button = new wxButton(this, wxID_OK, wxString("Ok"));
    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ExportForm::OnOk));
    cancel_button = new wxButton(this, wxID_CANCEL, wxString("Cancel"));

    Centre();

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* buttons_sizer = new wxBoxSizer(wxHORIZONTAL);

    buttons_sizer->Add(cancel_button);
    buttons_sizer->Add(ok_button);

    main_sizer->Add(buttons_sizer, 1, wxALIGN_RIGHT|wxRIGHT|wxBOTTOM, 2);

    this->SetSizer(main_sizer);
}

void ExportForm::OnOk(wxCommandEvent& event)
{
    this->EndModal(wxID_OK);
}
