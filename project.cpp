#include "project.hpp"

Project::Project()
{
    loaded = false;
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


        // Release the new 2da
        delete aux;
        aux = NULL;
    }

    loaded = true;
    return loaded;
}

bool Project::SaveProject()
{
    return false;
}

bool Project::SetUpProject()
{
    return false;
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
    if (!Tlk::Raw::Tlk::ReadFromFile(filename.c_str(), &raw))
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
