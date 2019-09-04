#include "project.hpp"

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
    tlk_filename = std::string(project.GetValue("General", "TlkName"));

    std::uint32_t twoda_count = std::stoul(std::string(project.GetValue("General", "2DA_COUNT")));
    for (std::uint32_t i = 0; i < twoda_count; i++)
    {
        std::string twoda = project.GetValue("2da", ("_" + std::to_string(i)).c_str());
        std::string filepath = base_path + std::string(SEPARATOR) + twoda + ".2da";
        TwoDA::Friendly::TwoDA* aux = Load2DAFile(filepath);
        if (aux == NULL)
            {} // Todo: some throw prob?

        delete twoda_list[twoda];
        twoda_list[twoda] = aux;

        // Replace2daRows(twoda_list[twoda], aux);
        twoda_edit_list[twoda] = true;

        // Release the new 2da
        // delete aux;
        // aux = NULL;
    }

    custom_tlk = LoadTLKFile(tlk_filename);

    for (auto const& entry : (*custom_tlk))
        current_tlk_row_count = std::max(entry.first, current_tlk_row_count);
    current_tlk_row_count = std::max(current_tlk_row_count, static_cast<std::uint32_t>(BASE_TLK_LIMIT + 1));

    loaded = true;
    return loaded;
}

bool Project::SaveProject(const bool& force_prompt)
{
    if (force_prompt || !loaded)
    {
        // TODO: Change this to directory
        wxFileDialog project_dialog(NULL, wxString("Save as *.nwh file"), "", "",
            "*.nwh", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

        if (project_dialog.ShowModal() == wxID_CANCEL)
        {
            // TODO: return or throw? not really sure
            return false;
        }
        base_path = project_dialog.GetPath().ToStdString();
    }

    CSimpleIniA project(true, true, true);
    if ((BASE_TLK_LIMIT + 2) < current_tlk_row_count)
    {
        // TODO: Create "ProjectSettings" to setup things like tlk filename etc.
        std::string path = base_path + std::string(SEPARATOR) + tlk_filename;
        project.SetValue("Files", "TLK", path.c_str());
        custom_tlk->WriteToFile(path.c_str());
    }

    unsigned int file_count = 0;
    std::string twoda_dir = base_path + std::string(SEPARATOR) + "2da" + std::string(SEPARATOR);
    if (!wxDirExists(wxString(twoda_dir)))
        wxMkdir(twoda_dir);

    for (auto const& entry : twoda_edit_list)
    {
        if (!entry.second)
            continue;

        project.SetValue("2da", (std::string("_") + std::to_string(file_count++)).c_str(), entry.first.c_str());
        twoda_list[entry.first]->WriteToFile((twoda_dir + entry.first).c_str());
    }

    if (file_count > 0)
        project.SetValue("Files", "2DA_COUNT", std::to_string(file_count).c_str());

    if (project.SaveFile((base_path + project_name + ".nwh").c_str()) < 0)
        wxMessageBox("Unable save project file!", "Error", wxOK | wxICON_ERROR);

    return false;
}

bool Project::SetUpProject()
{
    return false;
}

void Project::Set2daModified(const std::string& twoda, const bool& modified)
{
    twoda_edit_list[twoda] = modified;
}

TwoDA::Friendly::TwoDA* Project::Get2da(const std::string& name)
{
    if (twoda_list.find(name) == twoda_list.end())
        throw (std::string("Cannot find ") + name + std::string(".2da data!"));

    return twoda_list[name];
}

TwoDA::Friendly::TwoDARow* Project::Get2daRow(const std::string& twoda, const std::uint32_t& row_id)
{
    return &((*twoda_list[twoda]))[row_id];
}

std::string Project::GetTlkString(const std::uint32_t& strref)
{
    std::string result = "";
    if (strref > BASE_TLK_LIMIT)
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

Tlk::Friendly::Tlk* Project::LoadTLKFile(const std::string& filename)
{
    Tlk::Raw::Tlk raw;
    if (filename.size() > 0 && !Tlk::Raw::Tlk::ReadFromFile(filename.c_str(), &raw))
        throw std::string("Couldn't load ") + filename;

    Tlk::Friendly::Tlk* result = new Tlk::Friendly::Tlk(std::move(raw));
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
