#ifndef PROJECT_HPP
#define PROJECT_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "constants.hpp"

#define SI_SUPPORT_IOSTREAMS
#include "simpleini/SimpleIni.h"

#include "FileFormats/Bif.hpp"
#include "FileFormats/Key.hpp"
#include "FileFormats/2da.hpp"
#include "FileFormats/Tlk.hpp"

using namespace FileFormats;

class Project
{
public:
    Project();

    bool Initialize(const std::string& data_folder);
    bool SaveProject();
    bool LoadProject(const std::string& project_file);

    std::string GetTlkString(const std::uint32_t& strref);

    std::uint32_t SetTlkString(const std::string& value, std::uint32_t strref = 0);

    std::string project_name;
    std::string base_path;
    std::string tlk_filename;
    std::string project_filename;

    bool loaded;

private:
    Key::Friendly::Key* base_key;
    Bif::Friendly::Bif* base_2da;
    Tlk::Friendly::Tlk* base_dialog;

    Tlk::Friendly::Tlk* custom_tlk;
    std::map<std::string, TwoDA::Friendly::TwoDA* > twoda_list;
    std::map<std::string, bool> twoda_edit_list;

    std::uint32_t current_tlk_row_count;

    bool SetUpProject();

    Key::Friendly::Key* LoadKEYFile(const std::string& filename);
    Bif::Friendly::Bif* LoadBIFFile(const std::string& filename);
    Tlk::Friendly::Tlk* LoadTLKFile(const std::string& filename);
    TwoDA::Friendly::TwoDA* Load2DAFile(const std::string& filename);
    TwoDA::Friendly::TwoDA* Load2DAFile(const std::string& filename, std::byte const* entry, std::size_t length);
};

#endif