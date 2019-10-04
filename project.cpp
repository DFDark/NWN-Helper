#include "project.hpp"
#include "Components/export-form.hpp"

Project::Project()
{
    loaded = false;
}

Project::~Project()
{
    delete base_key;
    delete base_2da;
    delete base_dialog;

    base_key = NULL;
    base_2da = NULL;
    base_dialog = NULL;
}

bool Project::Initialize(const std::string& data_folder)
{
    base_key = LoadKEYFile(data_folder + std::string(SEPARATOR) + "nwn_base.key");
    base_2da = LoadBIFFile(data_folder + std::string(SEPARATOR) + "base_2da.bif");
    base_dialog = LoadTLKFile(data_folder + std::string(SEPARATOR) + "dialog.tlk");

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
        twoda_list[filename] = Load2DAFile(filename,
            kvp.second.m_DataBlock->GetData(),
            kvp.second.m_DataBlock->GetDataLength()
        );
    }

    // Create empty TLK
    custom_tlk = LoadTLKFile();
    current_tlk_row_count = static_cast<std::uint32_t>(BASE_TLK_LIMIT);
    return true;
}

bool Project::LoadProject(const std::string& project_file)
{
    if (project_file.size() < 1)
        return false;

    CSimpleIniA project(true, true, true);
    if (project.LoadFile(project_file.c_str()) < 0)
        return false;

    project_name = std::string(project.GetValue("General", "ProjectName"));
    project_filename = project_file;
    base_path = std::string(project.GetValue("General", "BaseDir"));
    tlk_filename = std::string(project.GetValue("General", "TlkFilename"));

    std::uint32_t twoda_count = std::stoul(std::string(project.GetValue("General", "2daCount")));
    for (std::uint32_t i = 0; i < twoda_count; i++)
    {
        try
        {
            std::string twoda = project.GetValue("2da", ("_" + std::to_string(i)).c_str());
            std::string filepath = base_path + std::string(SEPARATOR);
            filepath += "2da" + std::string(SEPARATOR) + twoda ;//+ ".2da";

            TwoDA::Friendly::TwoDA* aux = Load2DAFile(filepath);

            if (aux == NULL)
                {} // Todo: some throw prob?

            delete twoda_list[twoda];
            twoda_list[twoda] = aux;
            // Replace2daRows(twoda_list[twoda], aux);
            twoda_edit_list[twoda] = true;
        }
        catch (const std::string& msg)
        {
            wxMessageBox(msg, "Error", wxOK|wxICON_ERROR);
        }
    }

    custom_tlk = LoadTLKFile(tlk_filename, true);

    for (auto const& entry : (*custom_tlk))
        current_tlk_row_count = std::max((entry.first + BASE_TLK_LIMIT), current_tlk_row_count);
    current_tlk_row_count = std::max(current_tlk_row_count, static_cast<std::uint32_t>(BASE_TLK_LIMIT));

    loaded = true;
    return loaded;
}

void Project::SaveProject(const bool& force_prompt)
{
    if (force_prompt || !loaded)
    {
        ExportForm project_form(project_name, base_path, tlk_filename);
        if (project_form.ShowModal() == wxID_CANCEL)
            return;

        base_path = project_form.GetBasePath();
        project_name = project_form.GetProjectName();
        tlk_filename = project_form.GetTLKName();
        loaded = true;
    }

    CSimpleIniA project(true, true, true);
    project.SetValue("General", "ProjectName", project_name.c_str());
    project.SetValue("General", "BaseDir", base_path.c_str());
    if (BASE_TLK_LIMIT < current_tlk_row_count)
    {
        // TODO: Create "ProjectSettings" to setup things like tlk filename etc.
        std::string path = base_path + std::string(SEPARATOR) + tlk_filename + ".tlk";
        project.SetValue("General", "TlkFilename", path.c_str());
        if (!custom_tlk->WriteToFile(path.c_str()))
            throw std::string("Unable save " + path + " file!");
    }

    unsigned int file_count = 0;
    std::string twoda_dir = base_path + std::string(SEPARATOR) + "2da" + std::string(SEPARATOR);
    if (!wxDirExists(wxString(twoda_dir)))
        wxMkdir(twoda_dir);

    for (auto const& entry : twoda_edit_list)
    {
        if (!entry.second)
            continue;

        std::string path = twoda_dir + entry.first + ".2da";
        project.SetValue("2da", (std::string("_") + std::to_string(file_count++)).c_str(), (entry.first + ".2da").c_str());
        if (!twoda_list[entry.first]->WriteToFile(path.c_str()))
            throw std::string("Unable save " + path + " file!");
    }

    if (file_count > 0)
        project.SetValue("General", "2daCount", std::to_string(file_count).c_str());

    std::string project_file_path = base_path + std::string(SEPARATOR) + project_name + ".nwh";
    if (project.SaveFile(project_file_path.c_str()) < 0)
        throw std::string("Unable save " + project_file_path + " file!");
}

bool Project::SetUpProject()
{
    return false;
}

void Project::Set2daModified(const std::string& twoda, const bool& modified)
{
    twoda_edit_list[twoda] = modified;
}

TwoDA::Friendly::TwoDA* Project::Get2da(const std::string& name, const bool& base)
{
    TwoDA::Friendly::TwoDA* result;
    if (base)
    {
        // TODO: Should consider optimizing this later
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
            if (name == filename)
            {
                result = Load2DAFile(filename,
                    kvp.second.m_DataBlock->GetData(),
                    kvp.second.m_DataBlock->GetDataLength()
                );
            }
        }

        if (result == NULL)
            throw (std::string("Failed to load ") + name + std::string(".2da data!"));
    }
    else
    {
        if (twoda_list.find(name) == twoda_list.end())
            throw (std::string("Cannot find ") + name + std::string(".2da data!"));
        result = twoda_list[name];
    }
    return result;
}

TwoDA::Friendly::TwoDARow* Project::Get2daRow(const std::string& twoda, const std::uint32_t& row_id)
{
    return &((*twoda_list[twoda]))[row_id];
}

std::string Project::GetTlkString(const std::uint32_t& strref)
{
    std::string result = "";
    if (strref >= BASE_TLK_LIMIT)
        result = (*custom_tlk)[strref];
    else
        result = (*base_dialog)[strref];

    return result;
}

std::uint32_t Project::SetTlkString(const std::string& value, std::uint32_t strref)
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

Key::Friendly::Key* Project::LoadKEYFile(const std::string& filename)
{
    Key::Raw::Key raw;
    if (!Key::Raw::Key::ReadFromFile(filename.c_str(), &raw))
        throw std::string("Couldn't load ") + filename;

    Key::Friendly::Key* result = new Key::Friendly::Key(std::move(raw));
    return result;
}

Bif::Friendly::Bif* Project::LoadBIFFile(const std::string& filename)
{
    Bif::Raw::Bif raw;
    if (!Bif::Raw::Bif::ReadFromFile(filename.c_str(), &raw))
        throw std::string("Couldn't load ") + filename;

    Bif::Friendly::Bif* result = new Bif::Friendly::Bif(std::move(raw));
    return result;
}

Tlk::Friendly::Tlk* Project::LoadTLKFile(const std::string& filename, const bool& custom)
{
    Tlk::Raw::Tlk raw;
    if (filename.size() > 0 && !Tlk::Raw::Tlk::ReadFromFile(filename.c_str(), &raw))
        throw std::string("Couldn't load ") + filename;

    Tlk::Friendly::Tlk* result = new Tlk::Friendly::Tlk(std::move(raw), custom);
    return result;
}

TwoDA::Friendly::TwoDA* Project::Load2DAFile(const std::string& filename)
{
    TwoDA::Raw::TwoDA raw;
    if (!TwoDA::Raw::TwoDA::ReadFromFile(filename.c_str(), &raw))
        throw std::string("Couldn't load ") + filename;

    TwoDA::Friendly::TwoDA* result = new TwoDA::Friendly::TwoDA(std::move(raw));
    return result;
}

TwoDA::Friendly::TwoDA* Project::Load2DAFile(const std::string& filename, std::byte const* entry, std::size_t length)
{
    TwoDA::Raw::TwoDA raw;
    if (!TwoDA::Raw::TwoDA::ReadFromBytes(entry, length, &raw))
        throw std::string("Couldn't load from data ") + filename;

    TwoDA::Friendly::TwoDA* result = new TwoDA::Friendly::TwoDA(std::move(raw));
    return result;
}


void Project::Replace2daRows(TwoDA::Friendly::TwoDA* to, TwoDA::Friendly::TwoDA* from)
{
    // TODO:
    // Remove rows from to
    // insert from rows to 'to'
}

void Project::NewProject()
{
    base_path = "";
    project_name = "";
    tlk_filename = "";

    loaded = false;

    current_tlk_row_count = static_cast<std::uint32_t>(BASE_TLK_LIMIT);
    if (custom_tlk != NULL)
    {
        delete custom_tlk;
        custom_tlk = LoadTLKFile();
    }

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
        if (!twoda_edit_list[filename])
            continue;

        delete twoda_list[filename];
        twoda_list[filename] = Load2DAFile(filename,
            kvp.second.m_DataBlock->GetData(),
            kvp.second.m_DataBlock->GetDataLength()
        );
        twoda_edit_list[filename] = false;
    }
}

Tlk::Friendly::Tlk* Project::GetTlk(const bool& custom)
{
    return custom ? custom_tlk : base_dialog;
}
