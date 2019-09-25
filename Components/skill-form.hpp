#ifndef SKILL_FORM_HPP
#define SKILL_FORM_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "configuration-manager.hpp"
#include "FileFormats/2da.hpp"

using namespace FileFormats;

class SkillForm : public wxDialog
{
public:
    SkillForm(wxWindow* parent, ConfigurationManager* _configuration, std::uint32_t row_id);

private:
    void OnOk(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);

    void LoadCategoryValues();
    void LoadMiscellaneousValues();
    void InitFormValues();

    ConfigurationManager* configuration;
    TwoDA::Friendly::TwoDARow* skill;

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
    wxStaticText* category_label;
    wxStaticText* constant_label;
    wxStaticText* key_ability_label;
    wxStaticText* max_cr_label;

    wxTextCtrl* label;
    wxTextCtrl* name;
    wxTextCtrl* description;
    wxTextCtrl* icon;

    wxComboBox* category;
    wxTextCtrl* constant;
    wxComboBox* key_ability;
    wxTextCtrl* max_cr;

    wxCheckBox* untrained;
    wxCheckBox* armor_check_penalty;
    wxCheckBox* all_classes_can_use;
    wxCheckBox* hostile_skill;

    wxDECLARE_EVENT_TABLE();
};

#endif
