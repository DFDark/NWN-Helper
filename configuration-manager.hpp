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

using namespace FileFormats::Bif;
using namespace FileFormats::Key;

class ConfigurationManager {
public:
    ConfigurationManager();
    ~ConfigurationManager();

    bool AttemptLoad();
private:
    bool loaded;
    CSimpleIniA* config;

    bool InitialConfiguration();
    Friendly::Key LoadNWNBaseDataKEYFile(const char* filename);
    Friendly::Bif LoadNWNBaseDataBIFFile(const char* filename);
};

#endif
