#include "nwnhelper.hpp"
#include <cstdlib>
#include <cstdio>

NWNHelper::NWNHelper()
{
    nwnhelper_main = NULL;
    exit_code = 0;
    configuration_manager = new ConfigurationManager();
}

NWNHelper::~NWNHelper()
{
    delete configuration_manager;
    configuration_manager = NULL;
    
    if (nwnhelper_main != NULL)
    {
        delete nwnhelper_main;
        nwnhelper_main = NULL;
    }
}

bool NWNHelper::OnInit()
{
    if (!configuration_manager->AttemptLoad())
        exit_code = 1;
    else
    {
        nwnhelper_main = new NWNHelperMain("NWNHelperMain", wxPoint(20, 20), wxSize(800, 600), configuration_manager);
        nwnhelper_main->Show(true);
        SetTopWindow(nwnhelper_main);
    }

    // Load base NWN files
    //return false;

    return true;
}

int NWNHelper::OnRun()
{
    if (exit_code == 0)
        wxApp::OnRun();

    return exit_code;
}
