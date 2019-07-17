#ifndef SPELL_FORM_HPP
#define SPELL_FORM_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

class SpellForm : public wxDialog
{
public:
    SpellForm(wxWindow* parent, const wxString& title, const wxSize& size);
    ~SpellForm();

private:
    void OnOk(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);

    wxPanel* panel;

    wxButton* ok_button;
    wxButton* cancel_button;

    wxDECLARE_EVENT_TABLE();
};

#endif
