#ifndef SKILLLIST_MODEL_HPP
#define SKILLLIST_MODEL_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/dataview.h>

#include "configuration-manager.hpp"
#include "FileFormats/2da.hpp"
#include "FileFormats/Tlk.hpp"

using namespace FileFormats;

class SkillListModel : public wxDataViewVirtualListModel
{
public:
    enum
    {
        ID,
        LABEL,
        SKILL,
        UNTRAINED,
        KEY_ABILITY,
        ARMOR_CHECK_PENALTY,
        ALL_CLASSES_CAN_USE,
        CATEGORY,
        MAX_CR,
        CONSTANT,
        HOSTILE_SKILL
    };

    SkillListModel(TwoDA::Friendly::TwoDA* _file, ConfigurationManager* _configuration, const bool& _has_none = false);

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
    bool has_none;

    unsigned int GetUIntFromHex(const std::string& hex) const;
    std::size_t GetColumnID(unsigned int col) const;
};

#endif
