#ifndef IMPORT_FORM_HPP
#define IMPORT_FORM_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "configuration-manager.hpp"
#include "FileFormats/2da.hpp"
#include "FileFormats/Tlk.hpp"

using namespace FileFormats;

class ImportForm : public wxDialog
{
public:
    ImportForm(wxWindow* parent, ConfigurationManager* _configuration);

private:
    void OnOk(wxCommandEvent& event);
    void OnAdd2daFiles(wxCommandEvent& event);
    void OnAddTlkFile(wxCommandEvent& event);

    void Merge();
    void Overwrite();

    std::uint32_t import_mode;

    ConfigurationManager* configuration;

    wxListBox* import_2da_list;

    wxButton* add_2da_button;
    wxButton* add_tlk_button;

    wxStaticBox* import_2da_sb;
    wxStaticBox* import_mode_sb;
    wxStaticBox* import_tlk_sb;

    wxTextCtrl* tlk_filename;

    wxRadioButton* import_mode_merge;
    wxRadioButton* import_mode_overwrite;
    wxRadioButton* import_mode_manual;

    wxButton* ok_button;
    wxButton* cancel_button;

    wxDECLARE_EVENT_TABLE();
};

#endif
