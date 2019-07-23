#include "nwnhelper-main.hpp"
#include "Components/spell-form.hpp"
#include "Components/feat-form.hpp"

enum
{
    SPELLS = wxID_HIGHEST + 1,
    FEATS,
    SPELL_COLUMNS_MENU,
    FEAT_COLUMNS_MENU,
};

wxBEGIN_EVENT_TABLE(NWNHelperMain, wxFrame)
    EVT_MENU(wxID_EXIT,  NWNHelperMain::OnExit)
    EVT_MENU(SPELL_COLUMNS_MENU, NWNHelperMain::OnSpellColumnMenu)
    EVT_MENU(Feat_COLUMNS_MENU, NWNHelperMain::OnFeatColumnMenu)
    EVT_DATAVIEW_ITEM_ACTIVATED(SPELLS, NWNHelperMain::OnSpellActivated)
    EVT_DATAVIEW_ITEM_ACTIVATED(FEATS, NWNHelperMain::OnFeatActivated)
wxEND_EVENT_TABLE()

NWNHelperMain::NWNHelperMain(const wxString& title, ConfigurationManager* _configuration) :
    wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, _configuration->GetWindowResolution())
{
    configuration = _configuration;

    menu_file = new wxMenu;
    menu_file->Append(wxID_EXIT);// , "Exit", "Shuts down the application");
    menu_columns = new wxMenu;
    menu_columns->Append(SPELL_COLUMNS_MENU, "Spells", "Sets up visible columns for spells!");
    menu_columns->Append(SPELL_COLUMNS_MENU, "Feats", "Sets up visible columns for feats!");
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

    sp_model = new SpellListModel(_2da, tlk);
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

NWNHelperMain::~NWNHelperMain()
{
    delete menu_file;
    delete menu_bar;
    delete spells;
    delete tabs;

    menu_file = NULL;
    menu_bar = NULL;
    spells = NULL;
    tabs = NULL;
}

void NWNHelperMain::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void NWNHelperMain::OnSpellActivated(wxDataViewEvent& event)
{
    unsigned int row = sp_model->GetRow(event.GetItem());
    TwoDA::Friendly::TwoDARow* spell = sp_model->Get2daRow(row);
    Tlk::Friendly::Tlk* tlk = configuration->GetTlk();

    SpellForm form(main_panel, spell, tlk);
    form.ShowModal();
}

void NWNHelperMain::OnFeatActivated(wxDataViewEvent& event)
{
    unsigned int row = ft_model->GetRow(event.GetItem());
    TwoDA::Friendly::TwoDARow* feat = ft_model->Get2daRow(row);
    // Tlk::Friendly::Tlk* tlk = configuration->GetTlk();

    FeatForm form(main_panel, feat);
    form.ShowModal();
}

void NWNHelperMain::SetSpellColumns()
{
    spells->AppendTextColumn("ID", SpellListModel::ID);
    for (auto const& col : configuration->GetSpellColumns())
    {
        if (col == "label")
            spells->AppendTextColumn("Label", SpellListModel::LABEL);
        else if (col == "spell")
            spells->AppendTextColumn("Spell", SpellListModel::NAME);
        else if (col == "school")
            spells->AppendTextColumn("School", SpellListModel::SCHOOL);
    }
}

void NWNHelperMain::SetFeatColumns()
{
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
    // TODO: Add Column display form
    printf("NWNHelperMain::OnSpellColumnMenu Click\n");
}

void NWNHelperMain::OnFeatColumnMenu(wxCommandEvent& event)
{
    // TODO: Add Column display form
    printf("NWNHelperMain::OnFeatColumnMenu Click\n");
}

