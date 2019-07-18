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

using namespace FileFormats;

class NWNHelperMain : public wxFrame
{
public:
	NWNHelperMain(const wxString& title, const wxPoint& position, const wxSize& size, ConfigurationManager* _configuration);
	~NWNHelperMain();

    virtual void OnExit(wxCommandEvent& event);
private:
    void OnSpellActivated(wxDataViewEvent& event);
    void OnFeatActivated(wxDataViewEvent& event);
    
    wxMenuBar* menu_bar;
    wxMenu* menu_file;
    wxPanel* main_panel;

    wxDataViewListCtrl* spells;
    wxDataViewListCtrl* feats;

    wxNotebook* tabs;

    ConfigurationManager* configuration;
    SpellListModel* sp_model;
    FeatListModel* ft_model;

    wxDECLARE_EVENT_TABLE();
};

#endif
