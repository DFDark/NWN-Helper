#ifndef SKILL_SELECTION_FORM_HPP
#define SKILL_SELECTION_FORM_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "configuration-manager.hpp"
#include "ListModels/skilllist-model.hpp"

#include "FileFormats/2da.hpp"
#include "FileFormats/Tlk.hpp"

class SkillSelectionForm : public wxDialog
{
public:
    SkillSelectionForm(wxWindow* parent, ConfigurationManager* _configuration, const std::uint32_t& _selection = 0);

    std::uint32_t GetSkillSelection();

private:
    void OnShow(wxShowEvent& event);
    void OnOk(wxCommandEvent& event);
    void SetSkillColumns();
    void OnDoubleClick(wxDataViewEvent& event);

    std::uint32_t selection;
    ConfigurationManager* configuration;

    SkillListModel* sk_model;

    wxDataViewCtrl* skills;

    wxButton* ok_button;
    wxButton* cancel_button;

    wxDECLARE_EVENT_TABLE();
};


#endif