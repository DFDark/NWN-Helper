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
    delete base_2da;
    delete base_key;

    config = NULL;
    base_2da = NULL;
    base_key = NULL;
}

bool ConfigurationManager::AttemptLoad()
{
    config->Reset();
    loaded = config->LoadFile("nwnhelper.ini") >= 0;

    if (!loaded)
        loaded = InitialConfiguration();

    if (loaded)
    {
        try
        {
            base_key = LoadNWNBaseDataKEYFile("nwn_base.key");
            base_2da = LoadNWNBaseDataBIFFile("base_2da.bif");
        }
        catch (std::string& message)
        {
            wxMessageBox(message, "Error", wxOK | wxICON_ERROR );
            loaded = false;
        }

        //TODO: Load
        /*for (auto const& kvp : base_2da->GetResources())
        {
            // kvp.first => BIF ID
            // kvp.second => BifResource
            //std::printf("\n%s [%u | %u]: %zu bytes", StringFromResourceType(kvp.second.m_ResType), kvp.first, kvp.second.m_ResId, kvp.second.m_DataBlock->GetDataLength());
            for (auto const& res : base_key->GetReferencedResources())
            {
                if (res.m_ReferencedBifResId == kvp.first)
                {
                    printf(" RESREF: %s\n", res.m_ResRef.c_str());
                    break;
                }
            }
        }*/
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

Key::Friendly::Key* ConfigurationManager::LoadNWNBaseDataKEYFile(const char* filename)
{
    char* path = new char[255];
    memset(path, '\0', sizeof(char) * 255);
    sprintf(path, "%s/%s", config->GetValue("General", "DATA_FOLDER"), filename);

    Key::Raw::Key raw;
    if (!Key::Raw::Key::ReadFromFile(path, &raw))
    {
        std::string error = std::string("Couldn't load ") + std::string(path);
        throw error;
    }

    Key::Friendly::Key* result = new Key::Friendly::Key(std::move(raw));

    delete [] path;
    path = NULL;

    return result;
}

Bif::Friendly::Bif* ConfigurationManager::LoadNWNBaseDataBIFFile(const char* filename)
{
    char* path = new char[255];
    memset(path, '\0', sizeof(char) * 255);
    sprintf(path, "%s/%s", config->GetValue("General", "DATA_FOLDER"), filename);

    Bif::Raw::Bif raw;
    if (!Bif::Raw::Bif::ReadFromFile(path, &raw))
    {
        std::string error = std::string("Couldn't load ") + std::string(path);
        throw error;
    }

    Bif::Friendly::Bif* result = new Bif::Friendly::Bif(std::move(raw));

    delete [] path;
    path = NULL;

    return result;
}
