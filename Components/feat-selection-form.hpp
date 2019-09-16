#ifndef FEAT_SELECTION_FORM_HPP
#define FEAT_SELECTION_FORM_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "configuration-manager.hpp"
#include "ListModels/featlist-model.hpp"

#include "FileFormats/2da.hpp"
#include "FileFormats/Tlk.hpp"

class FeatSelectionForm : public wxDialog
{
public:
    FeatSelectionForm(wxWindow* parent, ConfigurationManager* _configuration, const std::uint32_t& _selection = 0);

    std::uint32_t GetFeatSelection();

private:
    void OnShow(wxShowEvent& event);
    void OnOk(wxCommandEvent& event);
    void SetFeatColumns();
    void OnDoubleClick(wxDataViewEvent& event);

    std::uint32_t selection;
    ConfigurationManager* configuration;

    FeatListModel* ft_model;

    wxDataViewCtrl* feats;

    wxButton* ok_button;
    wxButton* cancel_button;

    wxDECLARE_EVENT_TABLE();
};


#endif