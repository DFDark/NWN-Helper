#include "import-form.hpp"

enum
{
    IMPORT_MODE_MERGE = wxID_HIGHEST + 1,
    IMPORT_MODE_OVERWRITE,
    IMPORT_MODE_MANUAL,
    TWODA_FILES;
    ADD_2DA,
    ADD_TLK
};

wxBEGIN_EVENT_TABLE(ImportForm, wxDialog)
    EVT_MENU(wxID_OK, ImportForm::OnOk)
wxEND_EVENT_TABLE()

ImportForm::ImportForm(wxWindow* parent, ConfigurationManager* _configuration):
    wxDialog(parent, wxID_ANY, wxString("Import Form"), wxDefaultPosition, wxSize(480, 320))
{
    configuration = _configuration;

    import_2da_sb = new wxStaticBox(this, wxID_ANY, wxString("2da files"));
    import_mode_sb = new wxStaticBox(this, wxID_ANY, wxString("Import Mode"));
    import_tlk_sb = new wxStaticBox(this, wxID_ANY, wxString("Tlk File"));

    import_mode_merge = new wxRadioButton(import_mode_sb, IMPORT_MODE_MERGE, wxString("Merge"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    import_mode_overwrite = new wxRadioButton(import_mode_sb, IMPORT_MODE_OVERWRITE, wxString("Overwrite"));
    import_mode_manual = new wxRadioButton(import_mode_sb, IMPORT_MODE_MANUAL, wxString("Manual"));

    tlk_filename = new wxTextCtrl(import_tlk_sb, wxID_ANY, wxString(""));

    import_2da_list = new wxListBox(import_2da_sb, TWODA_FILES);
    add_2da_button = new wxButton(import_2da_sb, ADD_2DA, wxString("Add"));

    add_tlk_button = new wxButton(import_tlk_sb, ADD_TLK, wxString("Find"));


    ok_button = new wxButton(this, wxID_OK, wxString("Ok"));
    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ExportForm::OnOk));
    cancel_button = new wxButton(this, wxID_CANCEL, wxString("Cancel"));

    Centre();

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* first_row_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* second_row_sizer = new wxBoxSizer(wxHORIZONTAL);

    wxStaticBoxSizer* twoda_files = new wxStaticBoxSizer(import_2da_list, wxVERTICAL);
    twoda_files->Add(import_2da_list, 1, wxEXPAND);
    twoda_files->Add(add_2da_button);
    first_row_sizer->Add(twoda_files, 1, wxEXPAND);

    wxStaticBoxSizer* import_mode_sizer = new wxStaticBoxSizer(import_mode_sb, wxVERTICAL);
    import_mode_sizer->Add(import_mode_merge, 0, wxEXPAND);
    import_mode_sizer->Add(import_mode_overwrite, 0, wxEXPAND);
    import_mode_sizer->Add(import_mode_manual, 0, wxEXPAND);
    first_row_sizer->Add(import_mode_sizer, 1);

    wxStaticBoxSizer* import_tlk_sizer = new wxStaticBoxSizer(import_tlk_sb, wxHORIZONTAL);
    
    import_tlk_sizer->Add(tlk_filename, 1);
    import_tlk_sizer->Add(add_tlk_button);

    wxBoxSizer* buttons_sizer = new wxBoxSizer(wxHORIZONTAL);

    buttons_sizer->Add(cancel_button);
    buttons_sizer->Add(ok_button);

    main_sizer->Add(first_row_sizer, 0, wxEXPAND);
    main_sizer->Add(import_tlk_sizer, 1, wxEXPAND);
    main_sizer->Add(buttons_sizer, 0, wxALIGN_RIGHT|wxRIGHT|wxBOTTOM, 2);

    this->SetSizer(main_sizer);
}

void ImportForm::OnOk(wxCommandEvent& event)
{
    this->EndModal(wxID_OK);
}