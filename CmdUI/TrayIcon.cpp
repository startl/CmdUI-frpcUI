#include "TrayIcon.h"
#include "MainDialog.h"
#include <wx/window.h>

enum
{
    PU_RESTORE = 10001,
    PU_NEW_ICON,
    PU_EXIT,
    PU_CHECKMARK,
    PU_SUB1,
    PU_SUB2,
    PU_SUBMAIN
};


wxBEGIN_EVENT_TABLE(TTrayIcon, wxTaskBarIcon)
    EVT_MENU(PU_RESTORE, TTrayIcon::OnMenuRestore)
    EVT_MENU(PU_EXIT,    TTrayIcon::OnMenuExit)
    EVT_MENU(PU_CHECKMARK,TTrayIcon::OnMenuCheckmark)
    EVT_UPDATE_UI(PU_CHECKMARK,TTrayIcon::OnMenuUICheckmark)
    EVT_TASKBAR_LEFT_DCLICK  (TTrayIcon::OnLeftButtonDClick)
	/*EVT_MENU(PU_SUB1, TTrayIcon::OnMenuSub)
	EVT_MENU(PU_SUB2, TTrayIcon::OnMenuSub)*/
wxEND_EVENT_TABLE()

void TTrayIcon::OnMenuRestore(wxCommandEvent& )
{
	if (mMainDialog)
	{
		mMainDialog->Restore();
		mMainDialog->Show(true);
	}
}

void TTrayIcon::OnMenuExit(wxCommandEvent& )
{
	wxCommandEvent e(TCommandEvent);
	e.SetString("MenuExit");
	gApp->AddPendingEvent(e);
}

static bool check = true;

void TTrayIcon::OnMenuCheckmark(wxCommandEvent& )
{
    check = !check;
}

void TTrayIcon::OnMenuUICheckmark(wxUpdateUIEvent &event)
{
    event.Check(check);
}


// Overridables
wxMenu *TTrayIcon::CreatePopupMenu()
{
    wxMenu *menu = new wxMenu;
    menu->Append(PU_RESTORE, wxT("&Restore main window"));
    menu->AppendSeparator();
    /* OSX has built-in quit menu for the dock menu, but not for the status item */
#ifdef __WXOSX__ 
    if ( OSXIsStatusItem() )
#endif
    {
        menu->AppendSeparator();
        menu->Append(PU_EXIT,    wxT("E&xit"));
    }
    return menu;
}

void TTrayIcon::OnLeftButtonDClick(wxTaskBarIconEvent&)
{
	if (mMainDialog)
	{
		mMainDialog->Restore();
		mMainDialog->Show(true);
	}
}

