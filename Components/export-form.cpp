#include "export-form.hpp"

wxBEGIN_EVENT_TABLE(ExportForm, wxDialog)
    EVT_COMMAND_BUTTON_CLICKED(wxID_OK, ExportForm::OnOk)
wxEND_EVENT_TABLE()

ExportForm::ExportForm(wxWindow* parent, ConfigurationManager* _configuration) :
    wxDialog(parent, wxID_ANY, wxString("Export Form"), wxDefaultPosition, wxSize(640, 480))
{
    configuration = _configuration;
    
    ok_button = new wxButton(panel, wxID_OK, wxString("Ok"));
    cancel_button = new wxButton(panel, wxID_CANCEL, wxString("Cancel"));

    Centre();
    
    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* buttons_sizer = new wxBoxSizer(wxHORIZONTAL);
    
    buttons_sizer->(cancel_button);
    buttons_sizer->(ok_button);
    
    main_sizer->Add(buttons_sizer, 0, wxALIGN_RIGHT|wxRIGHT|wxBOTTOM, 2);
    
    this->SetSizer(main_sizer);
}

void ExportForm::OnOk(wxCommandEvent& event)
{
    this->EndModal(wxID_OK);
}