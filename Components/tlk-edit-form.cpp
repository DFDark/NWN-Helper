#include "tlk-edit-form.hpp"

enum
{
    TLK = wxID_HIGHEST + 1,
};

wxBEGIN_EVENT_TABLE(TlkEditForm, wxDialog)
wxEND_EVENT_TABLE()

TlkEditForm::TlkEditForm(wxWindow* parent, ConfigurationManager* _configuration, const bool& _custom) :
    wxDialog(parent, wxID_ANY, wxString("Tlk Edit form"), wxDefaultPosition, wxSize(800, 600))
{
    configuration = _configuration;
    custom = _custom;

    Tlk::Friendly::Tlk* tlk = configuration->GetTlk(custom);

    dataview = new wxDataViewCtrl(this, TLK);

    model = new TlkListModel(tlk, custom);
    dataview->AssociateModel(model);


    dataview->AppendTextColumn("ID", TlkListModel::ID);
    dataview->AppendTextColumn("Tlk entry", TlkListModel::TEXT);

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

    main_sizer->Add(dataview, 1, wxEXPAND);

    this->SetSizer(main_sizer);
}
