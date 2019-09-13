#ifndef CONFIGURATION_MANAGER_HPP
#define CONFIGURATION_MANAGER_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "constants.hpp"
#include "project.hpp"

#define SI_SUPPORT_IOSTREAMS
#include "simpleini/SimpleIni.h"
#include "Components/first-time-configuration.hpp"

#include "FileFormats/Bif.hpp"
#include "FileFormats/Key.hpp"
#include "FileFormats/2da.hpp"
#include "FileFormats/Tlk.hpp"

using namespace FileFormats;

class ConfigurationManager {
public:
    ConfigurationManager();
    ~ConfigurationManager();

    void AttemptLoad();
    void NewProject();
    bool LoadProjectData(const std::string& _path);
    bool SaveCurrentSettings();
    void ClearProjectData();
    void SetSpellColumns(const std::vector<std::string>& columns);
    void SetFeatColumns(const std::vector<std::string>& columns);
    void AddOrEdit2DARow(const std::string& twoda, const TwoDA::Friendly::TwoDARow& row);
    void Set2daModified(const std::string& twoda, const bool& modified = false);
    void SaveProject(const bool& force_prompt = false);
    std::uint32_t SetTlkString(const std::string& value, std::uint32_t strref = 0);

    TwoDA::Friendly::TwoDA* Get2da(std::string name);
    wxSize GetWindowResolution();
    std::vector<std::string> GetSpellColumns();
    std::vector<std::string> GetFeatColumns();
    std::string GetTlkString(const std::uint32_t& strref);

    TwoDA::Friendly::TwoDARow* Get2daRow(const std::string& twoda, const std::uint32_t& row_id);
private:
    std::uint32_t current_tlk_row_count;
    CSimpleIniA* config;

    Project project;

    bool InitialConfiguration();
};

#endif
