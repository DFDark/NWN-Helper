#include "nwnhelper.hpp"
#include <cstdlib>
#include <cstdio>

NWNHelper::NWNHelper()
{
    exit_code = 0;
    configuration_manager = new ConfigurationManager();
}

NWNHelper::~NWNHelper()
{
    delete configuration_manager;
    configuration_manager = NULL;
}

bool NWNHelper::OnInit()
{
    if (!configuration_manager->AttemptLoad())
        exit_code = 1;

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
