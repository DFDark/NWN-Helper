#ifndef CONFIGURATION_MANAGER_HPP
#define CONFIGURATION_MANAGER_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

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

    TwoDA::Friendly::TwoDA* Get2da(std::string name);
    Tlk::Friendly::Tlk* GetTlk();
private:
    bool loaded;
    CSimpleIniA* config;

    Key::Friendly::Key* base_key;
    Bif::Friendly::Bif* base_2da;
    Tlk::Friendly::Tlk* base_dialog;

    std::map<std::string, TwoDA::Friendly::TwoDA* > twoda_list;

    bool InitialConfiguration();
    Key::Friendly::Key* LoadNWNBaseDataKEYFile(const char* filename);
    Bif::Friendly::Bif* LoadNWNBaseDataBIFFile(const char* filename);
    Tlk::Friendly::Tlk* LoadNWNBaseDataTLKFile(const char* filename);
    TwoDA::Friendly::TwoDA* LoadTwoDAFile(std::string name, std::byte const* entry, std::size_t length);
};

#endif
