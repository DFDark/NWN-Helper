#ifndef CONFIGURATION_MANAGER_HPP
#define CONFIGURATION_MANAGER_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "constants.hpp"

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

    bool AttemptLoad();
    bool LoadProjectData(const std::string& project_file = "");
    bool SaveCurrentSettings();
    bool ExportCurrentFiles(const std::string& destination, const std::string& tlk_filename);
    void SetSpellColumns(const std::vector<std::string>& columns);
    void SetFeatColumns(const std::vector<std::string>& columns);
    void AddOrEdit2DARow(const std::string& twoda, const TwoDA::Friendly::TwoDARow& row);
    void Set2daModified(const std::string& twoda, const bool& modified = false);
    std::uint32_t SetTlkString(const std::string& value, std::uint32_t strref = 0);

    TwoDA::Friendly::TwoDA* Get2da(std::string name);
    Tlk::Friendly::Tlk* GetTlk();
    wxSize GetWindowResolution();
    std::vector<std::string> GetSpellColumns();
    std::vector<std::string> GetFeatColumns();
    wxArrayString* GetSpellList();
    wxArrayString* GetFeatList();
    std::string GetTlkString(const std::uint32_t& strref);

    TwoDA::Friendly::TwoDARow* Get2daRow(const std::string& twoda, const std::uint32_t& row_id);
private:
    bool loaded;
    std::uint32_t current_tlk_row_count;
    CSimpleIniA* config;
    wxArrayString* spell_list;
    wxArrayString* feat_list;

    Key::Friendly::Key* base_key;
    Bif::Friendly::Bif* base_2da;
    Tlk::Friendly::Tlk* base_dialog;

    Tlk::Friendly::Tlk* custom_tlk;
    std::map<std::string, TwoDA::Friendly::TwoDA* > twoda_list;
    std::map<std::string, bool> twoda_edit_list;

    bool InitialConfiguration();
    Key::Friendly::Key* LoadNWNBaseDataKEYFile(const char* filename);
    Bif::Friendly::Bif* LoadNWNBaseDataBIFFile(const char* filename);
    Tlk::Friendly::Tlk* LoadNWNBaseDataTLKFile(const char* filename);
    TwoDA::Friendly::TwoDA* LoadTwoDAFile(std::string name, std::byte const* entry, std::size_t length);
};

#endif
