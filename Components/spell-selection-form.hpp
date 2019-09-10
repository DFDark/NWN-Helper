#ifndef VALUE_SELECTION_FORM_HPP
#define VALUE_SELECTION_FORM_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "configuration-manager.hpp"
#include "ListModels/spelllist-model.hpp"

#include "FileFormats/2da.hpp"
#include "FileFormats/Tlk.hpp"

class SpellSelectionForm : public wxDialog
{
public:
    SpellSelectionForm(wxWindow* parent, ConfigurationManager* _configuration);

    std::uint32_t GetSpellSelection();

private:
    void OnOk(wxCommandEvent& event);
    void SetSpellColumns();
    void OnDoubleClick(wxDataViewEvent& event);


    std::uint32_t selection;
    ConfigurationManager* configuration;

    SpellListModel* sp_model;

    wxDataViewCtrl* spells;

    wxButton* ok_button;
    wxButton* cancel_button;

    wxDECLARE_EVENT_TABLE();
};

#endif
