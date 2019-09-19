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
    SPELL_POPUP_ADD,
    SPELL_POPUP_EDIT,
    SPELL_POPUP_COPY,
    SPELL_POPUP_DELETE,
    NEW_PROJECT,
    LOAD_PROJECT,
    SAVE_PROJECT,
    SAVE_PROJECT_AS,
};

wxBEGIN_EVENT_TABLE(NWNHelperMain, wxFrame)
    EVT_MENU(wxID_EXIT,  NWNHelperMain::OnExit)
    EVT_MENU(SPELL_COLUMNS_MENU, NWNHelperMain::OnSpellColumnMenu)
    EVT_MENU(FEAT_COLUMNS_MENU, NWNHelperMain::OnFeatColumnMenu)
    EVT_MENU(SPELL_POPUP_ADD, NWNHelperMain::OnSpellPopupAdd)
    EVT_MENU(SPELL_POPUP_EDIT, NWNHelperMain::OnSpellPopupEdit)
    EVT_MENU(SPELL_POPUP_COPY, NWNHelperMain::OnSpellPopupCopy)
    EVT_MENU(SPELL_POPUP_DELETE, NWNHelperMain::OnSpellPopupDelete)
    EVT_MENU(NEW_PROJECT, NWNHelperMain::OnNewProject)
    EVT_MENU(LOAD_PROJECT, NWNHelperMain::OnLoadProject)
    EVT_MENU(SAVE_PROJECT, NWNHelperMain::OnSaveProject)
    EVT_MENU(SAVE_PROJECT_AS, NWNHelperMain::OnSaveProjectAs)
    EVT_DATAVIEW_ITEM_ACTIVATED(SPELLS, NWNHelperMain::OnSpellActivated)
    EVT_DATAVIEW_ITEM_ACTIVATED(FEATS, NWNHelperMain::OnFeatActivated)
    EVT_DATAVIEW_ITEM_CONTEXT_MENU(SPELLS, NWNHelperMain::OnSpellRightClick)
wxEND_EVENT_TABLE()

NWNHelperMain::NWNHelperMain(const wxString& title, ConfigurationManager* _configuration) :
    wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, _configuration->GetWindowResolution())
{
    configuration = _configuration;

    menu_file = new wxMenu;
    menu_file->Append(NEW_PROJECT, "New Project", "Creates new NWN Helper project");
    menu_file->Append(LOAD_PROJECT, "Load Project", "Loads NWN Helper project");
    menu_file->Append(SAVE_PROJECT, "Save Project", "Save current project");
    menu_file->Append(SAVE_PROJECT_AS, "Save Project As", "Save current project as");
    menu_file->Append(wxID_SEPARATOR);
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

    sp_model = new SpellListModel(_2da, configuration);
    spells->AssociateModel(sp_model);

    ft_model = new FeatListModel(_feats, configuration);
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

    FeatForm form(main_panel, configuration, feat->RowId());
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
        else if (col == "min_attack_bonus")
            feats->AppendTextColumn("Min. AB", FeatListModel::MIN_ATTACK_BONUS);
        else if (col == "min_str")
            feats->AppendTextColumn("Min. Str.", FeatListModel::MIN_STR);
        else if (col == "min_dex")
            feats->AppendTextColumn("Min. Dex", FeatListModel::MIN_DEX);
        else if (col == "min_int")
            feats->AppendTextColumn("Min. Int", FeatListModel::MIN_INT);
        else if (col == "min_wiz")
            feats->AppendTextColumn("Min. Wiz", FeatListModel::MIN_WIS);
        else if (col == "min_con")
            feats->AppendTextColumn("Min. Con", FeatListModel::MIN_CON);
        else if (col == "min_cha")
            feats->AppendTextColumn("Min. Cha", FeatListModel::MIN_CHA);
        else if (col == "min_spell_level")
            feats->AppendTextColumn("Min. Spell Lvl.", FeatListModel::MIN_SPELL_LVL);
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

void NWNHelperMain::OnSpellRightClick(wxDataViewEvent& event)
{
    wxDataViewItem item = event.GetItem();
    if (item.IsOk())
    {
        wxMenu popup_menu;

        popup_menu.Append(SPELL_POPUP_ADD, "Add Spell");
        popup_menu.Append(SPELL_POPUP_EDIT, "Edit Spell");
        popup_menu.Append(SPELL_POPUP_COPY, "Copy Spell");
        popup_menu.Append(SPELL_POPUP_DELETE, "Delete Spell");

        PopupMenu(&popup_menu);
    }
}

void NWNHelperMain::OnSpellPopupAdd(wxCommandEvent& event)
{
    std::uint32_t spell_count = configuration->Get2da("spells")->Size();
    TwoDA::Friendly::TwoDARow* spell = configuration->Get2daRow("spells", spell_count);

    SpellForm form(main_panel, configuration, spell->RowId());
    form.ShowModal();
    // Reread 2da file, since we're adding row outside of wxDataViewModel
    sp_model->Reset(spell_count + 1);
    // Select new row
    spells->Select(sp_model->GetItem(spell_count));
    spells->EnsureVisible(sp_model->GetItem(spell_count));
}

void NWNHelperMain::OnSpellPopupEdit(wxCommandEvent& event)
{
    unsigned int row = sp_model->GetRow(spells->GetCurrentItem());
    TwoDA::Friendly::TwoDARow* spell = sp_model->Get2daRow(row);

    SpellForm form(main_panel, configuration, spell->RowId());
    form.ShowModal();
}

void NWNHelperMain::OnSpellPopupCopy(wxCommandEvent& event)
{
    unsigned int row = sp_model->GetRow(spells->GetCurrentItem());
    TwoDA::Friendly::TwoDARow* spell_origin = sp_model->Get2daRow(row);

    std::uint32_t spell_count = configuration->Get2da("spells")->Size();
    TwoDA::Friendly::TwoDARow* spell_target = configuration->Get2daRow("spells", spell_count);

    for (std::size_t i = 0; i < spell_origin->Size(); i++)
        (*spell_target)[i] = (*spell_origin)[i];

    SpellForm form(main_panel, configuration, spell_target->RowId());
    form.ShowModal();
    // Reread 2da file, since we're adding row outside of wxDataViewModel
    sp_model->Reset(spell_count + 1);
    // Select new row
    spells->Select(sp_model->GetItem(spell_count));
    spells->EnsureVisible(sp_model->GetItem(spell_count));
}

void NWNHelperMain::OnSpellPopupDelete(wxCommandEvent& event)
{
    // Will be implemented later
    // We will need to make sure we replace all row ids which are used as
    // a reference to the deleted row
    wxMessageBox("Not yet implemented",
        "Warning", wxOK | wxICON_WARNING );
}

void NWNHelperMain::OnNewProject(wxCommandEvent& event)
{
    sp_model->SetFile(NULL);
    ft_model->SetFile(NULL);

    // Todo: Check where is the memory leak from
    configuration->NewProject();

    sp_model->SetFile(configuration->Get2da("spells"));
    ft_model->SetFile(configuration->Get2da("feat"));

}

void NWNHelperMain::OnLoadProject(wxCommandEvent& event)
{
    wxFileDialog project_dialog(this, wxString("Locate *.nwh file"), "", "",
        "*.nwh", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (project_dialog.ShowModal() == wxID_CANCEL)
        return;

    std::string path = project_dialog.GetPath().ToStdString();

    sp_model->SetFile(NULL);
    ft_model->SetFile(NULL);

    configuration->LoadProjectData(path);

    sp_model->SetFile(configuration->Get2da("spells"));
    ft_model->SetFile(configuration->Get2da("feat"));
}

void NWNHelperMain::OnSaveProject(wxCommandEvent& event)
{
    try
    {
        configuration->SaveProject();
    }
    catch (const std::string& exception)
    {
        wxMessageBox(exception, "Error", wxOK|wxICON_ERROR);
    }
}

void NWNHelperMain::OnSaveProjectAs(wxCommandEvent& event)
{
    configuration->SaveProject(true);
}
