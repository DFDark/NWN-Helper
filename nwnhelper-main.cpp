#include "nwnhelper-main.hpp"
#include "Components/spell-form.hpp"
#include "Components/feat-form.hpp"
#include "Components/spell-column-form.hpp"
#include "Components/feat-column-form.hpp"
#include "Components/export-form.hpp"

enum
{
    SPELLS = wxID_HIGHEST + 1,
    FEATS,
    SPELL_COLUMNS_MENU,
    FEAT_COLUMNS_MENU,
    EXPORT_MENU,
};

wxBEGIN_EVENT_TABLE(NWNHelperMain, wxFrame)
    EVT_MENU(wxID_EXIT,  NWNHelperMain::OnExit)
    EVT_MENU(SPELL_COLUMNS_MENU, NWNHelperMain::OnSpellColumnMenu)
    EVT_MENU(FEAT_COLUMNS_MENU, NWNHelperMain::OnFeatColumnMenu)
    EVT_MENU(EXPORT_MENU, NWNHelperMain::OnExportMenu)
    EVT_DATAVIEW_ITEM_ACTIVATED(SPELLS, NWNHelperMain::OnSpellActivated)
    EVT_DATAVIEW_ITEM_ACTIVATED(FEATS, NWNHelperMain::OnFeatActivated)
wxEND_EVENT_TABLE()

NWNHelperMain::NWNHelperMain(const wxString& title, ConfigurationManager* _configuration) :
    wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, _configuration->GetWindowResolution())
{
    configuration = _configuration;

    menu_file = new wxMenu;
    menu_file->Append(wxID_ANY, "Export", "Exports saved files into selected directory");
    menu_file->Append(wxID_EXIT);// , "Exit", "Shuts down the application");
    menu_columns = new wxMenu;
    menu_columns->Append(SPELL_COLUMNS_MENU, "Spells", "Sets up visible columns for spells!");
    menu_columns->Append(FEAT_COLUMNS_MENU, "Feats", "Sets up visible columns for feats!");
    menu_bar = new wxMenuBar;
    menu_bar->Append(menu_file, "&File");
    menu_bar->Append(menu_columns, "&Columns");
    SetMenuBar(menu_bar);

    CreateStatusBar();
    SetStatusText("NWNHelperMain status bar");

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

    main_panel = new wxPanel(this, wxID_ANY);

    wxSize size = configuration->GetWindowResolution();
    tabs = new wxNotebook(main_panel, wxID_ANY, wxPoint(0, 0),
        wxSize(size.GetWidth() - 100, size.GetHeight() - 100));

    spells = new wxDataViewCtrl(tabs, SPELLS);
    feats = new wxDataViewCtrl(tabs, FEATS);

    tabs->AddPage(spells, wxString("Spells"));
    tabs->AddPage(feats, wxString("Feats"));

    TwoDA::Friendly::TwoDA* _2da = configuration->Get2da("spells");
    TwoDA::Friendly::TwoDA* _feats = configuration->Get2da("feat");
    Tlk::Friendly::Tlk* tlk = configuration->GetTlk();

    sp_model = new SpellListModel(_2da, configuration);
    spells->AssociateModel(sp_model);

    ft_model = new FeatListModel(_feats, tlk);
    feats->AssociateModel(ft_model);

    SetSpellColumns();
    SetFeatColumns();

    wxBoxSizer* tab_sizer = new wxBoxSizer(wxHORIZONTAL);
    tab_sizer->Add(tabs, 2, wxEXPAND|wxALL);
    main_panel->SetSizer(tab_sizer);

    main_sizer->Add(main_panel, 1, wxEXPAND|wxALL);
    this->SetSizer(main_sizer);
}

void NWNHelperMain::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void NWNHelperMain::OnSpellActivated(wxDataViewEvent& event)
{
    unsigned int row = sp_model->GetRow(event.GetItem());
    TwoDA::Friendly::TwoDARow* spell = sp_model->Get2daRow(row);

    SpellForm form(main_panel, configuration, spell->RowId());
    form.ShowModal();
}

void NWNHelperMain::OnFeatActivated(wxDataViewEvent& event)
{
    unsigned int row = ft_model->GetRow(event.GetItem());
    TwoDA::Friendly::TwoDARow* feat = ft_model->Get2daRow(row);

    FeatForm form(main_panel, feat);
    form.ShowModal();
}

void NWNHelperMain::SetSpellColumns()
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
    }
}

void NWNHelperMain::SetFeatColumns()
{
    feats->ClearColumns();
    feats->AppendTextColumn("ID", FeatListModel::ID);
    for (auto const& col : configuration->GetFeatColumns())
    {
        if (col == "label")
            feats->AppendTextColumn("Label", FeatListModel::LABEL);
        else if (col == "feat")
            feats->AppendTextColumn("Feat", FeatListModel::FEAT);
    }
}

void NWNHelperMain::OnSpellColumnMenu(wxCommandEvent& event)
{
    SpellColumnForm form(main_panel, configuration);
    if (form.ShowModal() == wxID_OK)
    {
        SetSpellColumns();
        configuration->SaveCurrentSettings();
    }
}

void NWNHelperMain::OnFeatColumnMenu(wxCommandEvent& event)
{
    FeatColumnForm form(main_panel, configuration);
    if (form.ShowModal() == wxID_OK)
    {
        SetFeatColumns();
        configuration->SaveCurrentSettings();
    }
}

void NWNHelperMain::OnExportMenu(wxCommandEvent& event)
{
    ExportForm form(main_panel, configuration);
    if (form.ShowModal() == wxID_OK)
        wxMessageBox("Files were exported successfully!", "Success", wxOK|wxICON_SUCCESS);
    else
        wxMessageBox("Error while exporting files!", "Error", wxOK|wxICON_ERROR);
}