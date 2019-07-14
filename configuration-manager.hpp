#ifndef CONFIGURATION_MANAGER_HPP
#define CONFIGURATION_MANAGER_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#define SI_SUPPORT_IOSTREAMS
#include "simpleini/SimpleIni.h"
#include "first-time-configuration.hpp"

#include "FileFormats/Bif.hpp"
#include "FileFormats/Key.hpp"
#include "FileFormats/2da.hpp"

using namespace FileFormats;

enum class BifIndex : std::uint32_t {
    // Probably could be done better
    TwoDA = 11
};

class ConfigurationManager {
public:
    ConfigurationManager();
    ~ConfigurationManager();

    bool AttemptLoad();
private:
    bool loaded;
    CSimpleIniA* config;

    Key::Friendly::Key* base_key;
    Bif::Friendly::Bif* base_2da;

    std::map<std::string, TwoDA::Friendly::TwoDA* > twoda_list;

    bool InitialConfiguration();
    Key::Friendly::Key* LoadNWNBaseDataKEYFile(const char* filename);
    Bif::Friendly::Bif* LoadNWNBaseDataBIFFile(const char* filename);
    TwoDA::Friendly::TwoDA* LoadTwoDAFile(std::string name, std::byte const* entry, std::size_t length);
};

#endif
