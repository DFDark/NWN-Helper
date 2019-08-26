#include "configuration-manager.hpp"
#include <fstream>

ConfigurationManager::ConfigurationManager()
{
    config = new CSimpleIniA(true, true, true);
    loaded = false;
    spell_list = new wxArrayString();
    feat_list = new wxArrayString();
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

bool ConfigurationManager::LoadProjectData(const std::string& project_file)
{
    bool loaded = true;
    bool has_project_file = project_file.size() > 0;
    try
    {
        CSimpleIniA project(true, true, true);
        if (has_project_file)
        {
            if (project.LoadFile(project_file) < 0)
                throw std::string("Loading project '" + project_file + "' has failed!");
        }

        base_key = LoadNWNBaseDataKEYFile("nwn_base.key");
        base_2da = LoadNWNBaseDataBIFFile("base_2da.bif");
        base_dialog = LoadNWNBaseDataTLKFile("dialog.tlk");

        Tlk::Raw::Tlk raw_tlk;
        if (has_project_file)
        {
            std::string tlk_file = config->GetValue("Files", "TLK");
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
    std::string result = "";
    if (strref > BASE_TLK_LIMIT)
        result = (*custom_tlk)[strref];
    else
        result = (*base_dialog)[strref];

    return result;
}

std::uint32_t ConfigurationManager::SetTlkString(const std::string& value, std::uint32_t strref)
{
    if (strref > 0)
    {
        if (strref < BASE_TLK_LIMIT)
            strref = current_tlk_row_count++;
    }
    else
        strref = current_tlk_row_count++;

    Tlk::Friendly::TlkEntry entry;
    entry.m_String = value;
    custom_tlk->Set(strref, entry);

    return strref;
}

bool ConfigurationManager::ExportCurrentFiles(const std::string& destination, const std::string& tlk_filename)
{
#if defined _WIN32 || _WIN64
    std::string separator = "\\";
#else
    std::string separator = "/";
#endif

    bool result = true;
    std::string directory = destination + separator;
    if ((BASE_TLK_LIMIT + 2) < current_tlk_row_count)
        result &= custom_tlk->WriteToFile((directory + tlk_filename).c_str());

    for (auto const& entry : twoda_edit_list)
    {
        if (entry.second)
            result &= twoda_list[entry.first]->WriteToFile((directory + entry.first + ".2da").c_str());
    }

    return result;
}
