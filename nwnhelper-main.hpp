#ifndef NWN_HELPER_MAIN
#define NWN_HELPER_MAIN

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/notebook.h>
#include <wx/dataview.h>

#include "configuration-manager.hpp"
#include "ListModels/spelllist-model.hpp"
#include "ListModels/featlist-model.hpp"
#include "ListModels/masterfeatlist-model.hpp"
#include "FileFormats/2da.hpp"
#include "FileFormats/Tlk.hpp"

using namespace FileFormats;

class NWNHelperMain : public wxFrame
{
public:
    NWNHelperMain(const wxString& title, ConfigurationManager* _configuration);

    virtual void OnExit(wxCommandEvent& event);
    void OnSpellColumnMenu(wxCommandEvent& event);
    void OnFeatColumnMenu(wxCommandEvent& event);

    void OnSpellRightClick(wxDataViewEvent& event);
private:
    void OnSpellActivated(wxDataViewEvent& event);
    void OnFeatActivated(wxDataViewEvent& event);

    void OnSpellPopupAdd(wxCommandEvent& event);
    void OnSpellPopupEdit(wxCommandEvent& event);
    void OnSpellPopupCopy(wxCommandEvent& event);
    void OnSpellPopupDelete(wxCommandEvent& event);

    void OnNewProject(wxCommandEvent& event);
    void OnLoadProject(wxCommandEvent& event);
    void OnSaveProject(wxCommandEvent& event);
    void OnSaveProjectAs(wxCommandEvent& event);

    void SetSpellColumns();
    void SetFeatColumns();
    void SetMasterFeatColumns();

    wxMenuBar* menu_bar;
    wxMenu* menu_file;
    wxMenu* menu_columns;
    wxPanel* main_panel;

    wxDataViewCtrl* spells;
    wxDataViewCtrl* feats;
    wxDataViewCtrl* master_feats;

    wxNotebook* tabs;

    ConfigurationManager* configuration;
    SpellListModel* sp_model;
    FeatListModel* ft_model;
    MasterFeatListModel* master_ft_model;

    wxDECLARE_EVENT_TABLE();
};

#endif
