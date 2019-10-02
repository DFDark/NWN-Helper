#include "import-form.hpp"
#include <experimental/filesystem>
#include "constants.hpp"
#include "functions.hpp"

namespace fs = std::experimental::filesystem;

enum
{
    IMPORT_MODE_MERGE = wxID_HIGHEST + 1,
    IMPORT_MODE_OVERWRITE,
    IMPORT_MODE_MANUAL,
    TWODA_FILES,
    ADD_2DA,
    ADD_TLK,
    IMPORT_MODE_MERGE_HOVER
};

wxBEGIN_EVENT_TABLE(ImportForm, wxDialog)
    EVT_BUTTON(wxID_OK, ImportForm::OnOk)
    EVT_BUTTON(ADD_2DA, ImportForm::OnAdd2daFiles)
    EVT_BUTTON(ADD_TLK, ImportForm::OnAddTlkFile)
wxEND_EVENT_TABLE()

ImportForm::ImportForm(wxWindow* parent, ConfigurationManager* _configuration):
    wxDialog(parent, wxID_ANY, wxString("Import Form"), wxDefaultPosition, wxSize(480, 320))
{
    configuration = _configuration;

    import_2da_sb = new wxStaticBox(this, wxID_ANY, wxString("2da files"));
    import_mode_sb = new wxStaticBox(this, wxID_ANY, wxString("Import Mode")/*, wxDefaultPosition, wxSize(120, -1)*/);
    import_tlk_sb = new wxStaticBox(this, wxID_ANY, wxString("Tlk File"));

    import_mode_merge = new wxRadioButton(import_mode_sb, IMPORT_MODE_MERGE, wxString("Merge"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    import_mode_overwrite = new wxRadioButton(import_mode_sb, IMPORT_MODE_OVERWRITE, wxString("Overwrite"));
    import_mode_manual = new wxRadioButton(import_mode_sb, IMPORT_MODE_MANUAL, wxString("Manual"));
    add_2da_button = new wxButton(this, ADD_2DA, wxString("Add 2da file(s)"));

    import_mode_merge->SetToolTip("Individual 2da rows will be compared to the project file. Any different rows will be added to the end of the file.");
    import_mode_overwrite->SetToolTip("Individual 2da rows will be compared to the project file. Any different rows will overwrite row with the same ROW ID.");

    tlk_filename = new wxTextCtrl(import_tlk_sb, wxID_ANY, wxString(""));

    import_2da_list = new wxListBox(import_2da_sb, TWODA_FILES);

    add_tlk_button = new wxButton(import_tlk_sb, ADD_TLK, wxString("Find"));


    ok_button = new wxButton(this, wxID_OK, wxString("Ok"));
    cancel_button = new wxButton(this, wxID_CANCEL, wxString("Cancel"));

    Centre();

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* first_row_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* second_row_sizer = new wxBoxSizer(wxHORIZONTAL);

    wxStaticBoxSizer* twoda_files = new wxStaticBoxSizer(import_2da_sb, wxVERTICAL);
    twoda_files->Add(import_2da_list, 1, wxEXPAND);
    first_row_sizer->Add(twoda_files, 1, wxEXPAND);

    wxBoxSizer* right_column = new wxBoxSizer(wxVERTICAL);
    wxStaticBoxSizer* import_mode_sizer = new wxStaticBoxSizer(import_mode_sb, wxVERTICAL);

    import_mode_sizer->Add(import_mode_merge, 0, wxEXPAND);
    import_mode_sizer->Add(import_mode_overwrite, 0, wxEXPAND);
    import_mode_sizer->Add(import_mode_manual, 0, wxEXPAND);
    import_mode_manual->Enable(false);

    right_column->Add(import_mode_sizer, 0, wxEXPAND);
    right_column->Add(add_2da_button, 0, wxEXPAND);
    first_row_sizer->Add(right_column);

    wxStaticBoxSizer* import_tlk_sizer = new wxStaticBoxSizer(import_tlk_sb, wxHORIZONTAL);

    import_tlk_sizer->Add(tlk_filename, 1);
    import_tlk_sizer->Add(add_tlk_button);

    wxBoxSizer* buttons_sizer = new wxBoxSizer(wxHORIZONTAL);

    buttons_sizer->Add(cancel_button);
    buttons_sizer->Add(ok_button);

    main_sizer->Add(first_row_sizer, 1, wxEXPAND);
    main_sizer->Add(import_tlk_sizer, 0, wxEXPAND);
    main_sizer->Add(buttons_sizer, 0, wxALIGN_RIGHT|wxRIGHT|wxBOTTOM, 2);

    this->SetSizer(main_sizer);
}

void ImportForm::OnOk(wxCommandEvent& event)
{
    bool has_2da = import_2da_list->GetCount() > 0;
    bool has_tlk = !tlk_filename->GetValue().IsEmpty();

    if (!has_2da && !has_tlk)
    {
        wxMessageBox("No 2da or tlk file was entered to import!", "Error", wxOK | wxICON_ERROR);
        return;
    }

    if (!has_2da && has_tlk)
    {
        if (wxMessageBox("No 2da file was entered to import! If you proceed tlk entries will be added or will overwrite current tlk file. Do you want to proceed?",
            "Confirm", wxYES_NO|wxICON_QUESTION) != wxYES)
            return;
    }

    if (has_2da && !has_tlk)
    {
        if (wxMessageBox("No tlk file was entered to import! All imported rows will have their strrefs nullified. Do you want to proceed?",
            "Confirm", wxYES_NO|wxICON_QUESTION) != wxYES)
            return;
    }

    if (import_mode_merge->GetValue())
        Merge();

    if (import_mode_overwrite->GetValue())
        Overwrite();

    this->EndModal(wxID_OK);
}

void ImportForm::OnAdd2daFiles(wxCommandEvent& event)
{
    wxFileDialog files_dialog(this, wxString("Locate *.2da file(s)"), "", "",
        "*.2da", wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);

    if (files_dialog.ShowModal() == wxID_CANCEL)
        return;

    wxArrayString paths;
    files_dialog.GetPaths(paths);
    import_2da_list->Append(paths);
}

void ImportForm::OnAddTlkFile(wxCommandEvent& event)
{
    wxFileDialog tlk_dialog(this, wxString("Locate *.tlk file"), "", "",
        "*.tlk", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (tlk_dialog.ShowModal() == wxID_CANCEL)
        return;

    tlk_filename->SetValue(tlk_dialog.GetPath());
}

void ImportForm::Merge()
{
    Tlk::Raw::Tlk rawtlk;
    if (!Tlk::Raw::Tlk::ReadFromFile(tlk_filename->GetValue().ToStdString().c_str(), &rawtlk))
        return;

    Tlk::Friendly::Tlk tlk(std::move(rawtlk), true);

    for (const wxString& file : import_2da_list->GetStrings())
    {
        std::string twoda = fs::path(file.ToStdString()).stem();

        TwoDA::Raw::TwoDA raw;
        if (!TwoDA::Raw::TwoDA::ReadFromFile(file.ToStdString().c_str(), &raw))
            continue; // TODO

        TwoDA::Friendly::TwoDA aux(std::move(raw));
        try
        {
            TwoDA::Friendly::TwoDA* original = configuration->Get2da(twoda, true);
            TwoDA::Friendly::TwoDA* current = configuration->Get2da(twoda);

            const std::size_t base_size = original->Size();
            std::size_t current_size = current->Size();

            std::vector<std::size_t> rows_to_add;

            for (std::size_t i = 0; i < aux.Size(); i++)
            {
                if (i < base_size && Compare2daRows((*original)[i], aux[i]))
                    continue;

                if (i < current_size && Compare2daRows((*current)[i], aux[i]))
                    continue;

                // if we got here were adding the row
                rows_to_add.emplace_back(i);
            }

            // Now we should have list of row_ids to add to the current
            // Also we need to make sure to transfer
            if (rows_to_add.size() > 0)
            {
                const std::size_t rowsize = (*current)[0].Size();
                for (const std::size_t& i : rows_to_add)
                {
                    for (std::size_t j = 0; j < rowsize; j++)
                    {
                        // TLK column will have to have it's value copied to our tlk
                        if (IsTlkColumn(j, twoda))
                        {
                            std::uint32_t strref_origin = GetUintFromString(aux[i][j].m_Data);
                            // if strref is not customised we don't need to create new one
                            if (strref_origin < BASE_TLK_LIMIT)
                                (*current)[current_size][j] = aux[i][j];
                            else
                            {
                                std::uint32_t strref = configuration->SetTlkString(tlk[strref_origin]);
                                (*current)[current_size][j].m_Data = std::to_string(strref);
                            }
                        }
                        else
                            (*current)[current_size][j] = aux[i][j];
                    }
                    current_size++;
                }
            }
        }
        catch (const std::string& ex)
        {
            wxMessageBox(ex.c_str(), "Error", wxOK|wxICON_ERROR);
        }
    }
}

void ImportForm::Overwrite()
{
    Tlk::Raw::Tlk rawtlk;
    if (!Tlk::Raw::Tlk::ReadFromFile(tlk_filename->GetValue().ToStdString().c_str(), &rawtlk))
        return;

    Tlk::Friendly::Tlk tlk(std::move(rawtlk), true);
    for (const wxString& file : import_2da_list->GetStrings())
    {
        std::string twoda = fs::path(file.ToStdString()).stem();

        TwoDA::Raw::TwoDA raw;
        if (!TwoDA::Raw::TwoDA::ReadFromFile(file.ToStdString().c_str(), &raw))
            continue; // TODO

        TwoDA::Friendly::TwoDA aux(std::move(raw));
        try
        {
            TwoDA::Friendly::TwoDA* original = configuration->Get2da(twoda, true);
            TwoDA::Friendly::TwoDA* current = configuration->Get2da(twoda);

            const std::size_t base_size = original->Size();
            std::size_t current_size = current->Size();

            for (std::size_t i = 0; i < aux.Size(); i++)
            {
                if (i < base_size && Compare2daRows((*original)[i], aux[i]))
                    continue;

                if (i < current_size && Compare2daRows((*current)[i], aux[i]))
                    continue;

                // if we got here we're overwriting the row
                const std::size_t rowsize = (*current)[0].Size();
                for (std::size_t j = 0; j < rowsize; j++)
                {
                    // TLK column will have to have it's value copied to our tlk
                    if (IsTlkColumn(j, twoda))
                    {
                        std::uint32_t strref_origin = GetUintFromString(aux[i][j].m_Data);
                        // if strref is not customised we don't need to create new one
                        if (strref_origin < BASE_TLK_LIMIT)
                            (*current)[i][j] = aux[i][j];
                        else
                        {
                            std::uint32_t current_strref = GetUintFromString((*current)[i][j].m_Data);
                            // if current row has custom tlk we will
                            std::uint32_t strref = configuration->SetTlkString(tlk[strref_origin],
                                current_strref >= BASE_TLK_LIMIT ? current_strref : 0);
                            (*current)[i][j].m_Data = std::to_string(strref);
                        }
                    }
                    else
                        (*current)[i][j] = aux[i][j];
                }
            }
        }
        catch (const std::string& ex)
        {
            wxMessageBox(ex.c_str(), "Error", wxOK|wxICON_ERROR);
        }
    }
}

bool ImportForm::Compare2daRows(const TwoDA::Friendly::TwoDARow& row1, const TwoDA::Friendly::TwoDARow& row2)
{
    if (row1.Size() != row2.Size())
        return false;

    for (std::size_t i = 0; i < row1.Size(); i++)
        if (row1.AsStr(i) != row2.AsStr(i))
            return false;

    return true;
}


bool ImportForm::IsTlkColumn(const std::size_t& column, const std::string& twoda)
{
    if (twoda == "spells")
        switch (column)
        {
            case GETIDX(SPELL_2DA::Name):
            case GETIDX(SPELL_2DA::SpellDesc):
            case GETIDX(SPELL_2DA::AltMessage): return true;
        }
    else if (twoda == "skills")
        switch (column)
        {
            case GETIDX(SKILL_2DA::Name):
            case GETIDX(SKILL_2DA::Description): return true;
        }
    else if (twoda == "feat")
        switch (column)
        {
            case GETIDX(FEAT_2DA::Feat):
            case GETIDX(FEAT_2DA::Description): return true;
        }
    else if (twoda == "masterfeats")
        switch (column)
        {
            case GETIDX(MASTERFEAT_2DA::Strref):
            case GETIDX(MASTERFEAT_2DA::Description): return true;
        }

    return false;
}
