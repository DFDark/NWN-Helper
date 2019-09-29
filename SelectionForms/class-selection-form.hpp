#ifndef CLASS_SELECTION_FORM_HPP
#define CLASS_SELECTION_FORM_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "configuration-manager.hpp"
#include "ListModels/classlist-model.hpp"

#include "FileFormats/2da.hpp"
#include "FileFormats/Tlk.hpp"

class ClassSelectionForm : public wxDialog
{
public:
    ClassSelectionForm(wxWindow* parent, ConfigurationManager* _configuration, const std::uint32_t& _selection = 0);

    std::uint32_t GetClassSelection();

private:
    void OnShow(wxShowEvent& event);
    void OnOk(wxCommandEvent& event);
    void SetClassColumns();
    void OnDoubleClick(wxDataViewEvent& event);

    std::uint32_t selection;
    ConfigurationManager* configuration;

    ClassListModel* cls_model;

    wxDataViewCtrl* classes;

    wxButton* ok_button;
    wxButton* cancel_button;

    wxDECLARE_EVENT_TABLE();
};


#endif
