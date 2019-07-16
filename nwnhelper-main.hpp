#ifndef NWN_HELPER_MAIN
#define NWN_HELPER_MAIN

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/notebook.h>
#include <wx/dataview.h>

#include "configuration-manager.hpp"

class NWNHelperMain : public wxFrame
{
public:
	NWNHelperMain(const wxString& title, const wxPoint& position, const wxSize& size, ConfigurationManager* _configuration);
	~NWNHelperMain();

    virtual void OnExit(wxCommandEvent& event);
private:
    wxMenuBar* menu_bar;
    wxMenu* menu_file;
    wxPanel* main_panel;
    wxDataViewListCtrl* spells;
    wxNotebook* tabs;

    ConfigurationManager* configuration;

    wxDECLARE_EVENT_TABLE();
};

#endif
