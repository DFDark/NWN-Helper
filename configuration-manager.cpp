#include "configuration-manager.hpp"
#include <fstream>

ConfigurationManager::ConfigurationManager()
{
    config = new CSimpleIniA(true, true, true);
}

ConfigurationManager::~ConfigurationManager()
{
    delete config;
    config = NULL;
}

void ConfigurationManager::AttemptLoad()
{
    config->Reset();
    bool result = config->LoadFile("nwnhelper.ini") >= 0;

    if (!result && !InitialConfiguration())
        throw std::string("Initial configuration failed!");

    if (result && !project.Initialize(std::string(config->GetValue("General", "DATA_FOLDER"))))
        throw std::string("Project initialization failed!");
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

            config->SetValue("FeatList", "COLUMN0", "label");
            config->SetValue("FeatList", "COLUMN1", "feat");

            config->SetValue("MasterFeatList", "COLUMN0", "label");
            config->SetValue("MasterFeatList", "COLUMN1", "master_feat");

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

bool ConfigurationManager::LoadProjectData(const std::string& _path)
{
    return project.LoadProject(_path);
}

TwoDA::Friendly::TwoDA* ConfigurationManager::Get2da(std::string name)
{
    return project.Get2da(name);
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
            std::string index = std::to_string(i);
            std::string key = std::string("COLUMN") + index;
            std::string aux = std::string(config->GetValue("SpellList", key.c_str(), ""));
            if (aux.size() > 0)
                result.emplace_back(aux);
        }
    }
    catch (std::exception& e)
    {
        result.clear();
        wxMessageBox("Unable to load SpellList columns from ini file! Columns will be set to default.",
            "Warning", wxOK | wxICON_WARNING );
    }

    if (result.size() < 1)
    {
        result.emplace_back(std::string("label"));
        result.emplace_back(std::string("spell"));
    }

    return result;
}

std::vector<std::string> ConfigurationManager::GetFeatColumns()
{
    std::vector<std::string> result;
    try
    {
        for (unsigned int i = 0; i < 10; i++)
        {
            std::string index = std::to_string(i);
            std::string key = std::string("COLUMN") + index;
            std::string aux = std::string(config->GetValue("FeatList", key.c_str(), ""));
            if (aux.size() > 0)
                result.emplace_back(aux);
        }
    }
    catch (std::exception& e)
    {
        result.clear();
        wxMessageBox("Unable to load FeatList columns from ini file! Columns will be set to default.",
            "Warning", wxOK | wxICON_WARNING );
    }

    if (result.size() < 1)
    {
        result.emplace_back(std::string("label"));
        result.emplace_back(std::string("feat"));
    }

    return result;
}

std::vector<std::string> ConfigurationManager::GetMasterFeatColumns()
{
    std::vector<std::string> result;
    try
    {
        for (unsigned int i = 0; i < 10; i++)
        {
            std::string index = std::to_string(i);
            std::string key = std::string("COLUMN") + index;
            std::string aux = std::string(config->GetValue("MasterFeatList", key.c_str(), ""));
            if (aux.size() > 0)
                result.emplace_back(aux);
        }
    }
    catch (std::exception& e)
    {
        result.clear();
        wxMessageBox("Unable to load MasterFeatList columns from ini file! Columns will be set to default.",
            "Warning", wxOK | wxICON_WARNING );
    }

    if (result.size() < 1)
    {
        result.emplace_back(std::string("label"));
        result.emplace_back(std::string("master_feat"));
    }

    return result;
}

void ConfigurationManager::SetSpellColumns(const std::vector<std::string>& columns)
{
    for (unsigned int i = 0; i < 10; i++)
    {
        std::string index = std::to_string(i);
        std::string key = std::string("COLUMN") + index;
        config->Delete("SpellList", key.c_str());
    }

    unsigned int counter = 0;
    for (auto const& column : columns)
    {
        std::string index = std::to_string(counter++);
        std::string key = std::string("COLUMN") + index;
        config->SetValue("SpellList", key.c_str(), column.c_str());
    }
}

void ConfigurationManager::SetFeatColumns(const std::vector<std::string>& columns)
{
    for (unsigned int i = 0; i < 10; i++)
    {
        std::string index = std::to_string(i);
        std::string key = std::string("COLUMN") + index;
        config->Delete("FeatList", key.c_str());
    }

    unsigned int counter = 0;
    for (auto const& column : columns)
    {
        std::string index = std::to_string(counter++);
        std::string key = std::string("COLUMN") + index;
        config->SetValue("FeatList", key.c_str(), column.c_str());
    }
}

void ConfigurationManager::SetMasterFeatColumns(const std::vector<std::string>& columns)
{
    for (unsigned int i = 0; i < 10; i++)
    {
        std::string index = std::to_string(i);
        std::string key = std::string("COLUMN") + index;
        config->Delete("MasterFeatList", key.c_str());
    }

    unsigned int counter = 0;
    for (auto const& column : columns)
    {
        std::string index = std::to_string(counter++);
        std::string key = std::string("COLUMN") + index;
        config->SetValue("MasterFeatList", key.c_str(), column.c_str());
    }
}

bool ConfigurationManager::SaveCurrentSettings()
{
    return config->SaveFile("nwnhelper.ini") >= 0;
}


void ConfigurationManager::AddOrEdit2DARow(const std::string& twoda, const TwoDA::Friendly::TwoDARow& row)
{
    // TODO: Change this to 'Add2daRow'
}


void ConfigurationManager::Set2daModified(const std::string& twoda, const bool& modified)
{
    project.Set2daModified(twoda, modified);
}

TwoDA::Friendly::TwoDARow* ConfigurationManager::Get2daRow(const std::string& twoda, const std::uint32_t& row_id)
{
    return project.Get2daRow(twoda, row_id);
}

std::string ConfigurationManager::GetTlkString(const std::uint32_t& strref)
{
    return project.GetTlkString(strref);
}

std::uint32_t ConfigurationManager::SetTlkString(const std::string& value, std::uint32_t strref)
{
    return project.SetTlkString(value, strref);
}

void ConfigurationManager::SaveProject(const bool& force_prompt)
{
    project.SaveProject(force_prompt);
}

void ConfigurationManager::NewProject()
{
    project.NewProject();
}
