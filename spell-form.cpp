#include "spell-form.hpp"

wxBEGIN_EVENT_TABLE(SpellForm, wxDialog)
    EVT_MENU(wxID_OK, SpellForm::OnOk)
wxEND_EVENT_TABLE()

SpellForm::SpellForm(wxWindow* parent, const wxString& title, const wxSize& size)
    : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, size)
{
    panel = new wxPanel(this, wxID_ANY);

    ok_button = new wxButton(panel, wxID_OK, wxString("Ok"), wxPoint(295, 135), wxSize(100, 30));
    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SpellForm::OnOk));
    cancel_button = new wxButton(panel, wxID_CANCEL, wxString("Cancel"), wxPoint(190, 135), wxSize(100, 30));
    Centre();
}

SpellForm::~SpellForm()
{
    delete ok_button;
    ok_button = NULL;

    delete cancel_button;
    cancel_button = NULL;

    delete panel;
    panel = NULL;
}

void SpellForm::OnOk(wxCommandEvent& event)
{
    this->EndModal(wxID_OK);
}
