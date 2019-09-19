#ifndef FEATLIST_MODEL_HPP
#define FEATLIST_MODEL_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/dataview.h>

#include "configuration-manager.hpp"

using namespace FileFormats;

class FeatListModel : public wxDataViewVirtualListModel
{
public:
    enum
    {
        ID,
        LABEL,
        FEAT,
        MIN_ATTACK_BONUS,
        MIN_STR,
        MIN_DEX,
        MIN_INT,
        MIN_WIS,
        MIN_CON,
        MIN_CHA,
        MIN_SPELL_LVL
    };

    FeatListModel(TwoDA::Friendly::TwoDA* _file, ConfigurationManager* _configuration, const bool& _has_none = false);

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

    std::size_t GetColumnID(unsigned int col) const;
};

#endif
