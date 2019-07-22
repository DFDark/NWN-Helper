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

    for (auto const& data : twoda_list)
        delete data.second;
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
            base_dialog = LoadNWNBaseDataTLKFile("dialog.tlk");

            std::vector<Key::Friendly::KeyBifReferencedResource> resourcelist;
            for (auto const& r : base_key->GetReferencedResources())
            {
                if (r.m_ReferencedBifIndex == 11) // base_2da.bif index
                    resourcelist.emplace_back(r);
            }

            for (auto const& kvp : base_2da->GetResources())
            {
                if (resourcelist.size() <= kvp.first)
                    continue;

                std::string filename = resourcelist[kvp.first].m_ResRef;
                twoda_list[filename] = LoadTwoDAFile(filename,
                    kvp.second.m_DataBlock->GetData(),
                    kvp.second.m_DataBlock->GetDataLength()
                );
            }
        }
        catch (std::string& message)
        {
            wxMessageBox(message, "Error", wxOK | wxICON_ERROR );
            loaded = false;
        }
        catch (std::exception& e)
        {
            wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR );
            loaded = false;
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

            config->SetValue("Display", "WIDTH", "1024");
            config->SetValue("Display", "HEIGHT", "768");
            
            config->SetValue("SpellList", "COLUMN0", "label");
            config->SetValue("SpellList", "COLUMN1", "spell");

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

Tlk::Friendly::Tlk* ConfigurationManager::LoadNWNBaseDataTLKFile(const char* filename)
{
    char* path = new char[255];
    memset(path, '\0', sizeof(char) * 255);
    sprintf(path, "%s/%s", config->GetValue("General", "DATA_FOLDER"), filename);

    Tlk::Raw::Tlk raw;
    if (!Tlk::Raw::Tlk::ReadFromFile(path, &raw))
    {
        std::string error = std::string("Couldn't load ") + std::string(path);
        throw error;
    }

    Tlk::Friendly::Tlk* result = new Tlk::Friendly::Tlk(std::move(raw));

    delete [] path;
    path = NULL;

    return result;
}

TwoDA::Friendly::TwoDA* ConfigurationManager::LoadTwoDAFile(std::string name, std::byte const* entry, std::size_t length)
{
    TwoDA::Raw::TwoDA raw;
    if (!TwoDA::Raw::TwoDA::ReadFromBytes(entry, length, &raw))
    {
        std::string error = std::string("Couldn't load ") + name;
        throw error;
    }

    TwoDA::Friendly::TwoDA* result = new TwoDA::Friendly::TwoDA(std::move(raw));
    return result;
}

TwoDA::Friendly::TwoDA* ConfigurationManager::Get2da(std::string name)
{
    if (twoda_list.find(name) == twoda_list.end())
        throw (std::string("Cannot find ") + name + std::string(".2da data!"));

    return twoda_list[name];
}

Tlk::Friendly::Tlk* ConfigurationManager::GetTlk()
{
    return base_dialog;
}

wxSize ConfigurationManager::GetWindowResolution()
{
    std::uint32_t width;
    std::uint32_t height;
    try
    {
        width = std::stoul(config->GetValue("Display", "WIDTH"));
        height = std::stoul(config->GetValue("Display", "HEIGHT"));
    }
    catch (std::exception& e)
    {
        width = 1024;
        height = 768;
    }
    
    return wxSize(width, height);
}

std::vector<std::string> ConfigurationManager::GetSpellColumns()
{
    std::vector<std::string> result;
    try
    {
        for (unsigned int i = 0; i < 10; i++)
        {
            std::string key = std::string("COLUMN") + std::string(i);
            std::string aux = std::string(config->GetValue("SpellList", key, ""));
            if (aux.size > 0)
                result.emplace_back(aux);
        }
    }
    catch (std::exception& e)
    {
        result = std::vector<std::string>("label", "name");
    }
    
    return result;
}