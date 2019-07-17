#include "spell-form.hpp"

wxBEGIN_EVENT_TABLE(SpellForm, wxDialog)
    EVT_MENU(wxID_OK, SpellForm::OnOk)
    EVT_MENU(wxID_CANCEL, SpellForm::OnCancel)
wxEND_EVENT_TABLE()

SpellForm::SpellForm(wxWindow* parent, const wxString& title, const wxSize& size)
    : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, size)
{
    panel = new wxPanel(this, wxID_ANY);
    
    /*
    * FORM LABELS
    */
    wxStaticText* label_label = new wxStaticText(panel, wxID_ANY, wxString("Label:"),
       wxPoint(5, 5), wxDefaultSize, wxALIGN_LEFT);
    wxStaticText* name_label = new wxStaticText(panel, wxID_ANY, wxString("Name:"),
       wxPoint(155, 5), wxDefaultSize, wxALIGN_LEFT);
    wxStaticText* spellschool_label = new wxStaticText(panel, wxID_ANY, wxString("Spell School:"),
       wxPoint(305, 5), wxDefaultSize, wxALIGN_LEFT);
    wxStaticText* spellrange_label = new wxStaticText(panel, wxID_ANY, wxString("Spell Range:"),
       wxPoint(305, 5), wxDefaultSize, wxALIGN_LEFT);
    
    /*
    * FORM TEXT CONTROLS
    */
    wxTextCtrl* label = new wxTextCtrl(panel, wxID_ANY, wxString(""), wxPoint(5, 20), wxSize(150, 20));
    wxTextCtrl* name = new wxTextCtrl(panel, wxID_ANY, wxString(""), wxPoint(155, 20), wxSize(150, 20));
    
    /*
    * FORM SELECT
    */
    wxComboBox* spell_school = new wxComboBox(panel, wxID_ANY, wxString(""), wxPoint(305, 20), wxSize(100, 20),
        0/*, const wxString choices[]=NULL*/);
    wxComboBox* spell_range = new wxComboBox(panel, wxID_ANY, wxString(""));
    
    
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

void SpellForm::OnCancel(wxCommandEvent& event)
{
    this->EndModal(wxID_CANCEL);
}