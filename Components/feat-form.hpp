#ifndef FEAT_FORM_HPP
#define FEAT_FORM_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "configuration-manager.hpp"
#include "FileFormats/2da.hpp"

using namespace FileFormats;

class FeatForm : public wxDialog
{
public:
    FeatForm(wxWindow* parent, ConfigurationManager* _configuration, std::uint32_t row_id);

private:
    void OnOk(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);

    void SetFeatRequirements();
    void InitFormValues();

    void OnPrereqFeat1(wxCommandEvent& event);
    void OnPrereqFeat2(wxCommandEvent& event);

    ConfigurationManager* configuration;
    TwoDA::Friendly::TwoDARow* feat;

    std::uint32_t pre_req_feat_1_id;
    std::uint32_t pre_req_feat_2_id;

    wxPanel* panel;

    wxButton* ok_button;
    wxButton* cancel_button;

    /*
    * Form components
    */
    wxStaticText* label_label;
    wxStaticText* name_label;
    wxStaticText* description_label;
    wxStaticText* icon_label;
    wxStaticText* min_attack_label;
    wxStaticText* min_str_label;
    wxStaticText* min_dex_label;
    wxStaticText* min_int_label;
    wxStaticText* min_wis_label;
    wxStaticText* min_con_label;
    wxStaticText* min_cha_label;
    wxStaticText* min_spell_lvl_label;

    wxTextCtrl* label;
    wxTextCtrl* name;
    wxTextCtrl* description;
    wxTextCtrl* icon;

    wxTextCtrl* min_attack;
    wxTextCtrl* min_str;
    wxTextCtrl* min_dex;
    wxTextCtrl* min_int;
    wxTextCtrl* min_wis;
    wxTextCtrl* min_con;
    wxTextCtrl* min_cha;
    wxTextCtrl* min_spell_lvl;

    wxButton* pre_req_feat_1;
    wxButton* pre_req_feat_2;

    wxStaticBox* req_feat_staticbox;

    wxDECLARE_EVENT_TABLE();
};

#endif
