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
    bool SaveCurrentSettings();
    void SetSpellColumns(const std::vector<std::string>& columns);
    void SetFeatColumns(const std::vector<std::string>& columns);
    void AddOrEdit2DARow(const std::string& twoda, const TwoDA::Friendly::TwoDARow& row);

    TwoDA::Friendly::TwoDA* Get2da(std::string name);
    Tlk::Friendly::Tlk* GetTlk();
    wxSize GetWindowResolution();
    std::vector<std::string> GetSpellColumns();
    std::vector<std::string> GetFeatColumns();
    wxArrayString* GetSpellList();
    wxArrayString* GetFeatList();

    TwoDA::Friendly::TwoDARow* Get2daRow(const std::string& twoda, const std::uint32_t& row_id);
private:
    bool loaded;
    CSimpleIniA* config;
    wxArrayString* spell_list;
    wxArrayString* feat_list;

    Key::Friendly::Key* base_key;
    Bif::Friendly::Bif* base_2da;
    Tlk::Friendly::Tlk* base_dialog;

    std::map<std::string, TwoDA::Friendly::TwoDA* > twoda_list;

    /*
    * Probably better to later replace this with it's own "Project" class
    */
    std::map<std::string, TwoDA::Friendly::TwoDA* > twoda_edit_list;

    bool InitialConfiguration();
    Key::Friendly::Key* LoadNWNBaseDataKEYFile(const char* filename);
    Bif::Friendly::Bif* LoadNWNBaseDataBIFFile(const char* filename);
    Tlk::Friendly::Tlk* LoadNWNBaseDataTLKFile(const char* filename);
    TwoDA::Friendly::TwoDA* LoadTwoDAFile(std::string name, std::byte const* entry, std::size_t length);
};

#endif
