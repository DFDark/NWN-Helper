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
#include "Components/spell-selection-form.hpp"
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

    void OnSpellPopupEdit(wxCommandEvent& event);
    void OnSpellPopupDelete(wxCommandEvent& event);

    void OnNewProject(wxCommandEvent& event);
    void OnLoadProject(wxCommandEvent& event);
    void OnSaveProject(wxCommandEvent& event);
    void OnSaveProjectAs(wxCommandEvent& event);

    void SetSpellColumns();
    void SetFeatColumns();

    wxMenuBar* menu_bar;
    wxMenu* menu_file;
    wxMenu* menu_columns;
    wxPanel* main_panel;

    wxDataViewCtrl* spells;
    wxDataViewCtrl* feats;

    wxNotebook* tabs;

    ConfigurationManager* configuration;
    SpellListModel* sp_model;
    FeatListModel* ft_model;

    wxDECLARE_EVENT_TABLE();
};

#endif
