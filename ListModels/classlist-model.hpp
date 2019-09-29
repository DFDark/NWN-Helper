#ifndef CLASSLIST_MODEL_HPP
#define CLASSLIST_MODEL_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/dataview.h>

#include "configuration-manager.hpp"
#include "FileFormats/2da.hpp"
#include "FileFormats/Tlk.hpp"

using namespace FileFormats;

class ClassListModel : public wxDataViewVirtualListModel
{
public:
    enum
    {
        ID,
        LABEL,
        CLASS,
        HIT_DIE,
        PLAYER_CLASS,
        SPELL_CASTER,
        STRENGTH,
        DEXTERITY,
        CONSTANT,
        WISDOM,
        INTELLIGENCE,
        CHARISMA,
        PRIMARY_ABILITY
    };

    ClassListModel(TwoDA::Friendly::TwoDA* _file, ConfigurationManager* _configuration, const bool& _has_none = false);

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
