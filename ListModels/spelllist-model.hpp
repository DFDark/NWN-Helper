#ifndef SPELLLIST_MODEL_HPP
#define SPELLLIST_MODEL_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/dataview.h>

#include "configuration-manager.hpp"
#include "FileFormats/2da.hpp"
#include "FileFormats/Tlk.hpp"

using namespace FileFormats;

class SpellListModel : public wxDataViewVirtualListModel
{
public:
    enum
    {
        ID,
        LABEL,
        SPELL,
        SCHOOL,
        RANGE,
        VS,
        META_MAGIC,
        TARGET_TYPE,
        IMPACT_SCRIPT
    };

    SpellListModel(TwoDA::Friendly::TwoDA* _file, ConfigurationManager* _configuration);

    TwoDA::Friendly::TwoDARow* Get2daRow(unsigned int row);

    virtual unsigned int GetColumnCount() const wxOVERRIDE;
    virtual wxString GetColumnType(unsigned int col) const wxOVERRIDE;
    virtual void GetValueByRow(wxVariant &variant, unsigned int row, unsigned int col) const wxOVERRIDE;
    virtual bool SetValueByRow(const wxVariant &variant, unsigned int row, unsigned int col) wxOVERRIDE;
    virtual bool GetAttrByRow(unsigned int row, unsigned int col, wxDataViewItemAttr& attr) const wxOVERRIDE;

    void SetFile(TwoDA::Friendly::TwoDA* _file);
private:
    TwoDA::Friendly::TwoDA* file;
    ConfigurationManager* configuration;

    unsigned int GetUIntFromHex(const std::string& hex) const;
    std::size_t GetColumnID(unsigned int col) const;
    std::string GetSchool(std::string school) const;
    std::string GetRange(std::string range) const;
    std::string GetMetaMagic(std::string metamagic) const;
    std::string GetTargetType(std::string target_type) const;
};

#endif
