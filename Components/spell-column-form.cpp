#include "spell-column-form.hpp"

wxBEGIN_EVENT_TABLE(SpellColumnForm, wxDialog)
    EVT_MENU(wxID_OK, SpellColumnForm::OnOk)
    EVT_MENU(wxID_CANCEL, SpellColumnForm::OnCancel)
wxEND_EVENT_TABLE()

SpellColumnForm::SpellColumnForm(wxWindow* parent, ConfigurationManager* _configuration) :
    wxDialog(parent, wxID_ANY, wxString("Spell Columns"), wxDefaultPosition, wxSize(400, 300))
{
    configuration = _configuration;

    ok_button = new wxButton(this, wxID_OK, wxString("Ok"), wxPoint(295, 235), wxSize(100, 30));
    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SpellColumnForm::OnOk));

    cancel_button = new wxButton(this, wxID_CANCEL, wxString("Cancel"), wxPoint(190, 235), wxSize(100, 30));

    Centre();
}

void SpellColumnForm::OnOk(wxCommandEvent& event)
{
    this->EndModal(wxID_OK);
}

void SpellColumnForm::OnCancel(wxCommandEvent& event)
{
    this->EndModal(wxID_CANCEL);
}
