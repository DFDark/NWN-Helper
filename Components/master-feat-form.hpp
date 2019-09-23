#ifndef MASTER_FEAT_FORM_HPP
#define MASTER_FEAT_FORM_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "configuration-manager.hpp"
#include "FileFormats/2da.hpp"

using namespace FileFormats;

class MasterFeatForm : public wxDialog
{
public:
    MasterFeatForm(wxWindow* parent, ConfigurationManager* _configuration, std::uint32_t row_id);

private:
    void OnOk(wxCommandEvent& event);

    void InitFormValues();

    std::string GetNameStrref();
    std::string GetDescriptionStrref();

    ConfigurationManager* configuration;
    TwoDA::Friendly::TwoDARow* master_feat;

    wxPanel* panel;

    wxButton* ok_button;
    wxButton* cancel_button;

    wxStaticText* label_label;
    wxStaticText* name_label;
    wxStaticText* description_label;
    wxStaticText* icon_label;

    wxTextCtrl* label;
    wxTextCtrl* name;
    wxTextCtrl* description;
    wxTextCtrl* icon;

    wxDECLARE_EVENT_TABLE();
};

#endif
