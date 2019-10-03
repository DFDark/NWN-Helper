#ifndef TLK_EDIT_FORM
#define TLK_EDIT_FORM

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/dataview.h>

#include "configuration-manager.hpp"
#include "FileFormats/2da.hpp"
#include "FileFormats/Tlk.hpp"

#include "ListModels/tlklist-model.hpp"

using namespace FileFormats;
class TlkEditForm : public wxDialog
{
public:
    TlkEditForm(wxWindow* parent, ConfigurationManager* _configuration, const bool& _custom = false);

private:
    ConfigurationManager* configuration;
    bool custom;

    wxDataViewCtrl* dataview;

    TlkListModel* model;

    void OnItemEditingDone(wxDataViewEvent& event);

    wxDECLARE_EVENT_TABLE();
};

#endif
