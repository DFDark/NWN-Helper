#include "configuration-manager.hpp"
#include <fstream>

ConfigurationManager::ConfigurationManager()
{
    config = new CSimpleIniA(true, true, true);
    loaded = false;
}

ConfigurationManager::~ConfigurationManager()
{
    delete config;
    config = NULL;
}

bool ConfigurationManager::AttemptLoad()
{
    config->Reset();
    loaded = config->LoadFile("nwnhelper.ini") >= 0;

    if (!loaded)
        loaded = InitialConfiguration();

    if (loaded)
    {
        Friendly::Key base_key = LoadNWNBaseDataKEYFile("NWNBase.key");
        Friendly::Bif bif_2da = LoadNWNBaseDataBIFFile("base_2da.bif");
        
        //TODO: Load 
        for (auto kvp : bif_2da.GetResources())
        {
            // kvp.first => BIF ID
            // kvp.second => BifResource
        }
    }

    return loaded;
}

bool ConfigurationManager::InitialConfiguration()
{
    bool result = false;

    wxMessageBox("NWNHelper failed to locate nwnhelper.ini. Configuration manager will be required to run before you're able to run the application.",
                  "Warning", wxOK | wxICON_WARNING );

    FirstTimeConfiguration* first_time_configuration = new FirstTimeConfiguration(
        "Configuration manager", wxSize(400, 200));

    if (first_time_configuration->ShowModal() == wxID_OK)
    {
        wxString data_folder = first_time_configuration->GetDataFolderPath();
        wxString main_ini = first_time_configuration->GetNWNIniPath();

        CSimpleIniA* nwnini = new CSimpleIniA(true, true, true);
        if (nwnini->LoadFile(main_ini.ToStdString().c_str()) >= 0)
        {
            config->SetValue("General", "HAK", nwnini->GetValue("Alias", "HAK"));
            config->SetValue("General", "OVERRIDE", nwnini->GetValue("Alias", "OVERRIDE"));
            config->SetValue("General", "TLK", nwnini->GetValue("Alias", "TLK"));

            config->SetValue("General", "DATA_FOLDER", data_folder.ToStdString().c_str());

            if (config->SaveFile("nwnhelper.ini") >= 0)
                result = true;
        }

        delete nwnini;
        nwnini = NULL;
    }


    if (!result)
    {
        wxMessageBox("Unable to determine NWN installation. Application will now close.",
            "Error", wxOK | wxICON_ERROR );
    }

    return result;
}

Friendly::Key ConfigurationManager::LoadNWNBaseDataKEYFile(const char* filename)
{
    char* path = new char[255];
    memset(path, '\0', sizeof(char) * 255);
    sprintf(path, "%s/%s", config->GetValue("General", "DATA_FOLDER"), filename);
    
    
    
    delete [] path;
    path = NULL;
}

Friendly::Bif ConfigurationManager::LoadNWNBaseDataBIFFile(const char* filename)
{
    char* path = new char[255];
    memset(path, '\0', sizeof(char) * 255);
    sprintf(path, "%s/%s", config->GetValue("General", "DATA_FOLDER"), filename);
    
    Raw::Bif aux;
    if (!Raw::Bif::ReadFromFile(path, &aux))
        return NULL;
    
    Friendly::Bif result = Friendly::Bif(std::move(aux));
    
    delete [] path;
    path = NULL;
    
    return result;
}