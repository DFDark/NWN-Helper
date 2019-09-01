#include "project.hpp"

Project::Project()
{
    loaded = false;
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
