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

    void LoadCategoryValues();
    void LoadSpellIdValue();
    void LoadSuccessorIdValue();
    void LoadMiscellaneousValues();
    void LoadSkillValues();

    void SetFeatRequirements();
    void InitFormValues();

    void OnPrereqFeat1(wxCommandEvent& event);
    void OnPrereqFeat2(wxCommandEvent& event);
    void OnSpell(wxCommandEvent& event);
    void OnSuccessor(wxCommandEvent& event);
    void OnMasterFeat(wxCommandEvent& event);
    void OnRequiredSkill1(wxCommandEvent& event);
    void OnRequiredSkill2(wxCommandEvent& event);

    ConfigurationManager* configuration;
    TwoDA::Friendly::TwoDARow* feat;

    std::uint32_t pre_req_feat_1_id;
    std::uint32_t pre_req_feat_2_id;
    std::uint32_t spellid;
    std::uint32_t successor_id;
    std::uint32_t master_feat_id;
    std::uint32_t or_req_feat_0_id;
    std::uint32_t or_req_feat_1_id;
    std::uint32_t or_req_feat_2_id;
    std::uint32_t or_req_feat_3_id;
    std::uint32_t or_req_feat_4_id;
    std::uint32_t req_skill_1_id;
    std::uint32_t req_skill_2_id;

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
    wxStaticText* category_label;
    wxStaticText* max_cr_label;
    wxStaticText* spell_label;
    wxStaticText* successor_label;
    wxStaticText* cr_value_label;
    wxStaticText* uses_per_day_label;
    wxStaticText* master_feat_label;

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
    wxTextCtrl* max_cr;
    wxTextCtrl* cr_value;
    wxTextCtrl* uses_per_day;
    wxTextCtrl* req_skill_min_rank_1;
    wxTextCtrl* req_skill_min_rank_2;

    wxButton* pre_req_feat_1;
    wxButton* pre_req_feat_2;
    wxButton* spell;
    wxButton* successor;
    wxButton* master_feat;
    wxButton* or_req_feat_0;
    wxButton* or_req_feat_1;
    wxButton* or_req_feat_2;
    wxButton* or_req_feat_3;
    wxButton* or_req_feat_4;
    wxButton* req_skill_1;
    wxButton* req_skill_2;

    wxStaticBox* req_feat_staticbox;
    wxStaticBox* min_req_staticbox;
    wxStaticBox* req_oneof_feat_staticbox;
    wxStaticBox* req_skill_staticbox;

    wxCheckBox* gain_multiple;
    wxCheckBox* effects_stack;
    wxCheckBox* all_classes_can_use;
    wxCheckBox* target_self;

    wxComboBox* category;

    wxDECLARE_EVENT_TABLE();
};

#endif
