#ifndef NWN_HELPER_MAIN
#define NWN_HELPER_MAIN

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class NWNHelperMain : public wxFrame
{
public:
	NWNHelperMain(const wxString& title, const wxPoint& position, const wxSize& size);
	~NWNHelperMain() {}

    virtual void OnExit();
private:
    wxMenuBar* menu_bar;
    wxMenu* menu_file;

    wxDECLARE_EVENT_TABLE();
};

#endif