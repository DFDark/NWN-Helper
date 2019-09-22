#include "spell-selection-form.hpp"

enum
{
    SPELLS = wxID_HIGHEST + 1,
};

wxBEGIN_EVENT_TABLE(SpellSelectionForm, wxDialog)
    EVT_MENU(wxID_OK, SpellSelectionForm::OnOk)
    EVT_SHOW(SpellSelectionForm::OnShow)
    EVT_DATAVIEW_ITEM_ACTIVATED(SPELLS, SpellSelectionForm::OnDoubleClick)
wxEND_EVENT_TABLE()

SpellSelectionForm::SpellSelectionForm(wxWindow* parent, ConfigurationManager* _configuration, const std::uint32_t& _selection) :
    wxDialog(parent, wxID_ANY, wxString("Spell selection form"), wxDefaultPosition, wxSize(480, 480))
{
    configuration = _configuration;
    selection = _selection;

    spells = new wxDataViewCtrl(this, SPELLS);
    TwoDA::Friendly::TwoDA* twoda = configuration->Get2da("spells");

    sp_model = new SpellListModel(twoda, configuration, true);
    spells->AssociateModel(sp_model);
    SetSpellColumns();

    ok_button = new wxButton(this, wxID_OK, wxString("Ok"));
    Connect(wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SpellSelectionForm::OnOk));
    cancel_button = new wxButton(this, wxID_CANCEL, wxString("Cancel"));

    Centre();

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* destination_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* buttons_sizer = new wxBoxSizer(wxHORIZONTAL);

    destination_sizer->Add(spells, 1, wxEXPAND);
    buttons_sizer->Add(cancel_button);
    buttons_sizer->Add(ok_button);

    main_sizer->Add(destination_sizer, 1, wxEXPAND);
    main_sizer->Add(buttons_sizer, 0, wxALIGN_RIGHT|wxRIGHT|wxBOTTOM, 2);

    this->SetSizer(main_sizer);
}

void SpellSelectionForm::OnOk(wxCommandEvent& event)
{
    if (!spells->HasSelection()) {
        wxMessageBox("No value selected!", "Error", wxOK|wxICON_ERROR);
        return;
    }

    TwoDA::Friendly::TwoDARow* row = sp_model->Get2daRow(sp_model->GetRow(spells->GetSelection()));
    selection = row->RowId();

    this->EndModal(wxID_OK);
}

void SpellSelectionForm::OnDoubleClick(wxDataViewEvent& event)
{
    TwoDA::Friendly::TwoDARow* row = sp_model->Get2daRow(sp_model->GetRow(event.GetItem()));
    selection = row->RowId();

    this->EndModal(wxID_OK);
}

void SpellSelectionForm::SetSpellColumns()
{
    spells->ClearColumns();
    spells->AppendTextColumn("ID", SpellListModel::ID);
    for (auto const& col : configuration->GetSpellColumns())
    {
        if (col == "label")
            spells->AppendTextColumn("Label", SpellListModel::LABEL);
        else if (col == "spell")
            spells->AppendTextColumn("Spell", SpellListModel::SPELL);
        else if (col == "school")
            spells->AppendTextColumn("School", SpellListModel::SCHOOL);
        else if (col == "range")
            spells->AppendTextColumn("Range", SpellListModel::RANGE);
        else if (col == "vs")
            spells->AppendTextColumn("VS", SpellListModel::VS);
        else if (col == "metamagic")
            spells->AppendTextColumn("MetaMagic", SpellListModel::META_MAGIC);
        else if (col == "targettype")
            spells->AppendTextColumn("TargetType", SpellListModel::TARGET_TYPE);
        else if (col == "impactscript")
            spells->AppendTextColumn("ImpactScript", SpellListModel::IMPACT_SCRIPT);
        else if (col == "bard")
            spells->AppendTextColumn("Bard", SpellListModel::BARD);
        else if (col == "cleric")
            spells->AppendTextColumn("Cleric", SpellListModel::CLERIC);
        else if (col == "druid")
            spells->AppendTextColumn("Druid", SpellListModel::DRUID);
        else if (col == "paladin")
            spells->AppendTextColumn("Paladin", SpellListModel::PALADIN);
        else if (col == "ranger")
            spells->AppendTextColumn("Ranger", SpellListModel::RANGER);
        else if (col == "wiz_sorc")
            spells->AppendTextColumn("Wiz/Sorc", SpellListModel::WIZ_SORC);
        else if (col == "innate")
            spells->AppendTextColumn("Innate", SpellListModel::INNATE);
    }
}

std::uint32_t SpellSelectionForm::GetSpellSelection()
{
    return selection;
}

void SpellSelectionForm::OnShow(wxShowEvent& event)
{
    if (event.IsShown())
    {
        spells->Select(sp_model->GetItem(selection));
        spells->EnsureVisible(sp_model->GetItem(selection));
    }
}
