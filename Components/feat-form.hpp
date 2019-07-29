#ifndef FEAT_FORM_HPP
#define FEAT_FORM_HPP

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "FileFormats/2da.hpp"

using namespace FileFormats;

class FeatForm : public wxDialog
{
public:
    FeatForm(wxWindow* parent, TwoDA::Friendly::TwoDARow* row);

private:
    void OnOk(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
    
    void InitFormValues();

    TwoDA::Friendly::TwoDARow* feat;

    wxPanel* panel;

    wxButton* ok_button;
    wxButton* cancel_button;

    /*
    * Form components
    */
    wxStaticText* label_label;
    wxStaticText* name_label;

    wxTextCtrl* label;
    wxTextCtrl* name;

    wxDECLARE_EVENT_TABLE();
};

#endif
