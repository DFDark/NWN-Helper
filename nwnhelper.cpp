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
    try
    {
        configuration_manager->AttemptLoad();

        nwnhelper_main = new NWNHelperMain("NWNHelperMain", configuration_manager);
        nwnhelper_main->Show(true);
        SetTopWindow(nwnhelper_main);
    }
    catch (const std::string& e)
    {
        wxMessageBox(e, "Error", wxOK|wxICON_ERROR);
        exit_code = 1;
    }

    return true;
}

int NWNHelper::OnRun()
{
    if (exit_code == 0)
        wxApp::OnRun();

    return exit_code;
}
