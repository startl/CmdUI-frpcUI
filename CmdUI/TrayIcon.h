#pragma once

#include "PreDef.h"

class TMainDialog;
class TTrayIcon : public wxTaskBarIcon
{
	wxApp* mApp = nullptr;
	TMainDialog* mMainDialog = nullptr;
public:
#if defined(__WXOSX__) && wxOSX_USE_COCOA
    TTrayIcon(wxTaskBarIconType iconType = wxTBI_DEFAULT_TYPE)
    :   wxTaskBarIcon(iconType)
#else
    TTrayIcon()
#endif
    {}

	//TMainDialog* GetMainDialog() const { return mMainDialog; }
	void SetMainDialog(TMainDialog* val) { mMainDialog = val; }
	void SetApp(wxApp* val) { mApp = val; }

    void OnLeftButtonDClick(wxTaskBarIconEvent&);
    void OnMenuRestore(wxCommandEvent&);
    void OnMenuExit(wxCommandEvent&);
    void OnMenuCheckmark(wxCommandEvent&);
    void OnMenuUICheckmark(wxUpdateUIEvent&);
    //void OnMenuSub(wxCommandEvent&);
    virtual wxMenu *CreatePopupMenu();

    wxDECLARE_EVENT_TABLE();
};

