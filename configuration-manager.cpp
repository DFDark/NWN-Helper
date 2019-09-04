#include "configuration-manager.hpp"
#include <fstream>

ConfigurationManager::ConfigurationManager()
{
    config = new CSimpleIniA(true, true, true);
    loaded = false;
    spell_list = new wxArrayString();
    feat_list = new wxArrayString();
    project_loaded = false;

    project = Project();
}

ConfigurationManager::~ConfigurationManager()
{
    delete config;
    config = NULL;

    spell_list->clear();
    feat_list->clear();

    delete spell_list;
    delete feat_list;
}

bool ConfigurationManager::AttemptLoad()
{
    config->Reset();
    loaded = config->LoadFile("nwnhelper.ini") >= 0;

    if (!loaded)
        loaded = InitialConfiguration();

    if (loaded)
        loaded = project.Initialize(std::string(config->GetValue("General", "DATA_FOLDER")));

    if (loaded)
        loaded = LoadProjectData();

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

            config->SetValue("FeatList", "COLUMN0", "label");
            config->SetValue("FeatList", "COLUMN1", "feat");

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

bool ConfigurationManager::LoadProjectData(const std::string& _directory, const std::string& _filename)
{
    bool loaded = true;
    try
    {
        CSimpleIniA project(true, true, true);
        if (_filename.size() > 0)
        {
            project_directory = _directory;
            project_file = _filename;
            if (project.LoadFile((project_directory + std::string(SEPARATOR) + project_file).c_str()) < 0)
                throw std::string("Loading project '" + (project_directory + std::string(SEPARATOR) + project_file) + "' has failed!");
            project_loaded = true;
        }

        base_key = LoadNWNBaseDataKEYFile("nwn_base.key");
        base_2da = LoadNWNBaseDataBIFFile("base_2da.bif");
        base_dialog = LoadNWNBaseDataTLKFile("dialog.tlk");

        Tlk::Raw::Tlk raw_tlk;
        if (_filename.size())
        {
            std::string tlk_file = project.GetValue("Files", "TLK");
            if (tlk_file.size() > 0 && !Tlk::Raw::Tlk::ReadFromFile(tlk_file.c_str(), &raw_tlk))
            {
                std::string error = std::string("Couldn't load ") + tlk_file;
                throw error;
            }
        }
        custom_tlk = new Tlk::Friendly::Tlk(std::move(raw_tlk));

        std::vector<Key::Friendly::KeyBifReferencedResource> resourcelist;
        for (auto const& r : base_key->GetReferencedResources())
        {
            if (r.m_ReferencedBifIndex == 11) // base_2da.bif index
                resourcelist.emplace_back(r);
        }

        std::map<std::string, bool> project_2da_list;
        if (_filename.size())
        {
            std::uint32_t files = std::stoul(project.GetValue("Files", "2DA_COUNT"));
            for (unsigned int i = 0; i < files; i++)
            {
                std::string aux = (std::string("_") + std::to_string(i));
                project_2da_list[project.GetValue("2da", aux.c_str())] = true;
            }
        }

        std::string twoda_dir = std::string(SEPARATOR) + "2da" + std::string(SEPARATOR);
        for (auto const& kvp : base_2da->GetResources())
        {
            if (resourcelist.size() <= kvp.first)
                continue;

            std::string filename = resourcelist[kvp.first].m_ResRef;
            if (project_2da_list[filename])
                twoda_list[filename] = Load2daFromFile(project_directory + twoda_dir + filename + std::string(".2da"));
            else
                twoda_list[filename] = LoadTwoDAFile(filename,
                    kvp.second.m_DataBlock->GetData(),
                    kvp.second.m_DataBlock->GetDataLength()
                );
        }

        // Preloading lists (for large arrays like spells/feats)
        // to save time from loading them on SpellForm init
        spell_list->Add("None");
        feat_list->Add("None");
        for (auto const& row : (*twoda_list["spells"]))
            spell_list->Add(row[GETIDX(SPELL_2DA::Label)].m_Data);
        for (auto const& row : (*twoda_list["feat"]))
            feat_list->Add(row[GETIDX(SPELL_2DA::Label)].m_Data);

        for (auto const& entry : (*custom_tlk))
            current_tlk_row_count = std::max(entry.first, current_tlk_row_count);
        current_tlk_row_count = std::max(current_tlk_row_count, static_cast<std::uint32_t>(BASE_TLK_LIMIT + 1));
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

    return loaded;
}

TwoDA::Friendly::TwoDA* ConfigurationManager::Get2da(std::string name)
{
    return project.Get2da(name);
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
    twoda_edit_list[twoda] = modified;
}

TwoDA::Friendly::TwoDARow* ConfigurationManager::Get2daRow(const std::string& twoda, const std::uint32_t& row_id)
{
    return &(*(twoda_list[twoda]))[row_id];
}

wxArrayString* ConfigurationManager::GetSpellList()
{
    return spell_list;
}

wxArrayString* ConfigurationManager::GetFeatList()
{
    return feat_list;
}

std::string ConfigurationManager::GetTlkString(const std::uint32_t& strref)
{
    return project.GetTlkString(strref);
}

std::uint32_t ConfigurationManager::SetTlkString(const std::string& value, std::uint32_t strref)
{
    return project.SetTlkString(value, strref);
}

bool ConfigurationManager::ExportCurrentFiles(const std::string& destination, const std::string& tlk_filename)
{
    bool result = true;
    std::string directory = destination + SEPARATOR;
    if ((BASE_TLK_LIMIT + 2) < current_tlk_row_count)
        result &= custom_tlk->WriteToFile((directory + tlk_filename).c_str());

    for (auto const& entry : twoda_edit_list)
    {
        if (entry.second)
            result &= twoda_list[entry.first]->WriteToFile((directory + entry.first + ".2da").c_str());
    }

    return result;
}

void ConfigurationManager::ClearProjectData()
{

    // No need to redeclare these since we're emptying them here
    spell_list->clear();
    feat_list->clear();
}

void ConfigurationManager::SaveProject(const bool& force_prompt)
{
    project.SaveProject(force_prompt);
}
