#ifndef NWNHELPER_HPP
#define NWNHELPER_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "configuration-manager.hpp"

class NWNHelper : public wxApp
{
public:
    NWNHelper();
    ~NWNHelper();
    virtual bool OnInit();
    virtual int OnRun();

private:
    int exit_code;
    ConfigurationManager* configuration_manager;
};

wxIMPLEMENT_APP(NWNHelper);

#endif /* end of include guard:  */
