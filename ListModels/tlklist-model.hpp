#ifndef TLKLIST_MODEL_HPP
#define TLKLIST_MODEL_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/dataview.h>

#include "configuration-manager.hpp"
#include "FileFormats/Tlk.hpp"

using namespace FileFormats;

class TlkListModel : public wxDataViewVirtualListModel
{
public:
    enum
    {
        ID,
        TEXT
    };

    TlkListModel(Tlk::Friendly::Tlk* _file, const bool& _custom = false);

    virtual unsigned int GetColumnCount() const wxOVERRIDE;
    virtual wxString GetColumnType(unsigned int col) const wxOVERRIDE;
    virtual void GetValueByRow(wxVariant &variant, unsigned int row, unsigned int col) const wxOVERRIDE;
    virtual bool SetValueByRow(const wxVariant &variant, unsigned int row, unsigned int col) wxOVERRIDE;
    virtual bool GetAttrByRow(unsigned int row, unsigned int col, wxDataViewItemAttr& attr) const wxOVERRIDE;

private:
    Tlk::Friendly::Tlk* file;
    bool custom;
};

#endif
