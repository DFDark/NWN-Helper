#include "nwnhelper-main.hpp"
#include "spell-form.hpp"


wxBEGIN_EVENT_TABLE(NWNHelperMain, wxFrame)
    EVT_MENU(wxID_EXIT,  NWNHelperMain::OnExit)
    // EVT_MENU(wxID_ABOUT, NWNHelperMain::OnAbout)
    //EVT_MENU(2, NWNHelperMain::Test)
wxEND_EVENT_TABLE()

NWNHelperMain::NWNHelperMain(const wxString& title, const wxPoint& position, const wxSize& size, ConfigurationManager* _configuration) :
    wxFrame(NULL, wxID_ANY, title, position, size)
{
    configuration = _configuration;

    menu_file = new wxMenu;
    // menu_file->Append(2, "Spells", "sdfsdf");
    menu_file->Append(wxID_EXIT);// , "Exit", "Shuts down the application");
    menu_bar = new wxMenuBar;
    menu_bar->Append(menu_file, "&File");
    SetMenuBar(menu_bar);

    CreateStatusBar();
    SetStatusText("NWNHelperMain status bar");

    main_panel = new wxPanel(this, wxID_ANY);
    
    tabs = new wxNotebook(main_panel, wxID_ANY, wxPoint(50, 10), wxSize(size.GetWidth() - 100, size.GetHeight() - 100));

    spells = new wxDataViewCtrl(tabs, wxID_ANY);
    feats = new wxDataViewCtrl(tabs, wxID_ANY);
    
    tabs->AddPage(spells, wxString("Spells"));
    tabs->AddPage(feats, wxString("Feats"));
    
    sp_model = new SpellListModel(configuration->Get2da("spells"));
    spells->AssociateModel(sp_model.get());
    
    spells->AppendTextColumn("ID", SpellListModel::ID);
    spells->AppendTextColumn("Spell Label", SpellListModel::LABEL);
    
    ft_model = new FeatListModel(configuration->Get2da("feats"));
    feats->AssociateModel(ft_model.get());
    
    feats->AppendTextColumn("ID", FeatListModel::ID);
    feats->AppendTextColumn("Feat Label", FeatListModel::LABEL);
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
