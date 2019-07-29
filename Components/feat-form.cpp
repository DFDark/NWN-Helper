#include "feat-form.hpp"
#include "../constants.hpp"

wxBEGIN_EVENT_TABLE(FeatForm, wxDialog)
    EVT_MENU(wxID_OK, FeatForm::OnOk)
    EVT_MENU(wxID_CANCEL, FeatForm::OnCancel)
wxEND_EVENT_TABLE()

FeatForm::FeatForm(wxWindow* parent, TwoDA::Friendly::TwoDARow* row)
    : wxDialog(parent, wxID_ANY, wxString("Feat Form"))
{
    panel = new wxPanel(this, wxID_ANY);
    feat = row;

    if (!(*feat)[GETIDX(FEAT_2DA::Label)].m_IsEmpty)
        this->SetTitle(wxString((*feat)[GETIDX(FEAT_2DA::Label)].m_Data));
    /*
    * FORM LABELS
    */
    label_label = new wxStaticText(panel, wxID_ANY, wxString("Label:"));
    name_label = new wxStaticText(panel, wxID_ANY, wxString("Name:"));

    /*
    * FORM TEXT CONTROLS
    */
    label = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    name = new wxTextCtrl(panel, wxID_ANY, wxString(""));

    ok_button = new wxButton(panel, wxID_OK, wxString("Ok"));
    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FeatForm::OnOk));

    cancel_button = new wxButton(panel, wxID_CANCEL, wxString("Cancel"));

    Centre();
    
    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    
    wxBoxSizer* first_row = new wxBoxSizer(wxHORIZONTAL);
    
    wxBoxSizer* label_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* name_sizer = new wxBoxSizer(wxVERTICAL);
    
    label_sizer->Add(label_label);
    label_sizer->Add(label, 1, wxEXPAND|wxALL);
    name_sizer->Add(name_label);
    name_sizer->Add(name, 1, wxEXPAND|wxALL);
    
    first_row->Add(label_sizer);
    first_row->Add(name_sizer);
    
    main_sizer->SetSizer(first_row);
    
    wxBoxSizer* control_button_sizer = new wxBoxSizer(wxHORIZONTAL);
    
    control_button_sizer->Add(cancel_button
    control_button_sizer->Add(ok_button);
    main_sizer->SetSizer(control_button_sizer, 0, wxALIGN_RIGHT|wxRIGHT|wxBOTTOM, 2);
    
    panel->SetSizer(main_sizer);

    InitFormValues();
}

void FeatForm::OnOk(wxCommandEvent& event)
{
    this->EndModal(wxID_OK);
}

void FeatForm::OnCancel(wxCommandEvent& event)
{
    this->EndModal(wxID_CANCEL);
}

void FeatForm::InitFormValues()
{
    label->SetValue(wxString((*feat)[GETIDX(FEAT_2DA::Label)].m_Data));
    // TODO: Link up with TLK and load actual value from strref
    name->SetValue(wxString((*feat)[GETIDX(FEAT_2DA::Feat)].m_Data));
}