#include "master-feat-form.hpp"
#include "constants.hpp"
#include "functions.hpp"

wxBEGIN_EVENT_TABLE(MasterFeatForm, wxDialog)
    EVT_MENU(wxID_OK, MasterFeatForm::OnOk)
    EVT_MENU(wxID_CANCEL, MasterFeatForm::OnCancel)
wxEND_EVENT_TABLE()

MasterFeatForm::MasterFeatForm(wxWindow* parent, ConfigurationManager* _configuration, std::uint32_t row_id)
    : wxDialog(parent, wxID_ANY, wxString("Master Feat Form"), wxDefaultPosition, wxSize(640, 480))
{
    panel = new wxPanel(this, wxID_ANY);
    configuration = _configuration;
    feat = configuration->Get2daRow("masterfeats", row_id);

    this->SetTitle(Get2DAString(master_feat, MASTERFEAT_2DA::Label));

    label_label = new wxStaticText(panel, wxID_ANY, wxString("Label:"));
    name_label = new wxStaticText(panel, wxID_ANY, wxString("Name:"));
    description_label = new wxStaticText(panel, wxID_ANY, wxString("Description:"));
    icon_label = new wxStaticText(panel, wxID_ANY, wxString("Icon:"));

    label = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    name = new wxTextCtrl(panel, wxID_ANY, wxString(""));
    description = new wxTextCtrl(panel, wxID_ANY, wxString(""), wxDefaultPosition, wxSize(450, -1), wxTE_MULTILINE);
    icon = new wxTextCtrl(panel, wxID_ANY, wxString(""));

    ok_button = new wxButton(panel, wxID_OK, wxString("Ok"));
    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(FeatForm::OnOk));

    cancel_button = new wxButton(panel, wxID_CANCEL, wxString("Cancel"));

    Centre();

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* first_row = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* second_row = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer* label_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* name_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* icon_sizer = new wxBoxSizer(wxVERTICAL);

    label_sizer->Add(label_label);
    label_sizer->Add(label, 0, wxEXPAND);
    name_sizer->Add(name_label);
    name_sizer->Add(name, 0, wxEXPAND);
    icon_sizer->Add(icon_label);
    icon_sizer->Add(icon, 0);

    first_row->Add(label_sizer, 1);
    first_row->Add(name_sizer, 1);
    first_row->Add(icon_sizer, 0);

    wxBoxSizer* description_sizer = new wxBoxSizer(wxVERTICAL);

    description_sizer->Add(description_label);
    description_sizer->Add(description, 1, wxEXPAND);

    second_row->Add(description_sizer, 1, wxEXPAND);

    wxBoxSizer* control_button_sizer = new wxBoxSizer(wxHORIZONTAL);

    control_button_sizer->Add(cancel_button);
    control_button_sizer->Add(ok_button);

    main_sizer->Add(first_row, 0, wxEXPAND);
    main_sizer->Add(second_row, 1, wxEXPAND);
    main_sizer->Add(control_button_sizer, 0, wxALIGN_RIGHT|wxRIGHT|wxBOTTOM, 2);

    panel->SetSizer(main_sizer);

    InitFormValues();
}

void MasterFeatForm::InitFormValues()
{
    label->SetValue(Get2DAString(master_feat, MASTERFEAT_2DA::Label));
    icon->SetValue(Get2DAString(master_feat, MASTERFEAT_2DA::Icon));

    std::uint32_t strref = GetUintFromString(Get2DAString(master_feat, MASTERFEAT_2DA::Strref));
    name->SetValue(strref > 0 ? configuration->GetTlkString(strref) : "");

    strref = GetUintFromString(Get2DAString(master_feat, MASTERFEAT_2DA::Description));
    description->SetValue(strref > 0 ? configuration->GetTlkString(strref) : "");
}

void MasterFeatForm::OnOk(wxCommandEvent& event)
{
    (*master_feat)[GETIDX(MASTERFEAT_2DA::Label)].m_Data = GetStringFromTextCtrl(label);
    (*master_feat)[GETIDX(MASTERFEAT_2DA::Icon)].m_Data = GetStringFromTextCtrl(Icon);
    (*master_feat)[GETIDX(MASTERFEAT_2DA::Strref)].m_Data = GetNameStrref();
    (*master_feat)[GETIDX(MASTERFEAT_2DA::Description)].m_Data = GetDescriptionStrref();
    
    this->EndModal(wxID_OK);
}

std::string MasterFeatForm::GetNameStrref()
{
    std::uint32_t strref = GetUintFromString(Get2DAString(master_feat, MASTERFEAT_2DA::Strref));

    std::string aux = name->GetValue().ToStdString();
    std::string base_name = configuration->GetTlkString(strref);
    if (base_name != aux)
        strref = configuration->SetTlkString(aux, strref);

    return std::to_string(strref);
}

std::string MasterFeatForm::GetDescriptionStrref()
{
    std::uint32_t strref = GetUintFromString(Get2DAString(master_feat, MASTERFEAT_2DA::Description));

    std::string aux = name->GetValue().ToStdString();
    std::string base_name = configuration->GetTlkString(strref);
    if (base_name != aux)
        strref = configuration->SetTlkString(aux, strref);

    return std::to_string(strref);
}