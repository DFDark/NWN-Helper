#ifndef SPELLLIST_MODEL_HPP
#define SPELLLIST_MODEL_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/dataview.h>

#include "FileFormats/2da.hpp"

using namespace FileFormats;

class SpellListModel : public wxDataViewVirtualListModel
{
public:
    enum
    {
        ID,
        LABEL,
        NAME,
        ICONRESREF,
        SCHOOL,
        RANGE,
        VS,
        METAMAGIC,
        TARGETTYPE
    };

    SpellListModel(TwoDA::Friendly::TwoDA* _file);

    TwoDA::Friendly::TwoDARow* Get2daRow(unsigned int row);

    virtual unsigned int GetColumnCount() const wxOVERRIDE;
    virtual wxString GetColumnType(unsigned int col) const wxOVERRIDE;
    virtual void GetValueByRow(wxVariant &variant, unsigned int row, unsigned int col) const wxOVERRIDE;
    virtual bool SetValueByRow(const wxVariant &variant, unsigned int row, unsigned int col) wxOVERRIDE;
    virtual bool GetAttrByRow(unsigned int row, unsigned int col, wxDataViewItemAttr& attr) const wxOVERRIDE;

private:
    TwoDA::Friendly::TwoDA* file;
};

#endif
