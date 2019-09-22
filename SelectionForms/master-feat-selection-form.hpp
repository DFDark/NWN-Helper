#ifndef MASTER_FEAT_SELECTION_FORM_HPP
#define MASTER_FEAT_SELECTION_FORM_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "configuration-manager.hpp"
#include "ListModels/masterfeatlist-model.hpp"

#include "FileFormats/2da.hpp"
#include "FileFormats/Tlk.hpp"

class MasterFeatSelectionForm : public wxDialog
{
public:
    MasterFeatSelectionForm(wxWindow* parent, ConfigurationManager* _configuration, const std::uint32_t& _selection = 0);

    std::uint32_t GetMasterFeatSelection();

private:
    void OnShow(wxShowEvent& event);
    void OnOk(wxCommandEvent& event);
    void SetMasterFeatColumns();
    void OnDoubleClick(wxDataViewEvent& event);

    std::uint32_t selection;
    ConfigurationManager* configuration;

    MasterFeatListModel* master_ft_model;

    wxDataViewCtrl* master_feats;

    wxButton* ok_button;
    wxButton* cancel_button;

    wxDECLARE_EVENT_TABLE();
};

#endif
