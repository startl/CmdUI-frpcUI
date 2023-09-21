#pragma once

#include "PreDef.h"
#include "TrayIcon.h"
#include "MainDialog.h"

#include <asl/IniFile.h>
#include <asl/Process.h>
using namespace asl;

class MyApp : public wxApp
{
	TTrayIcon   *mTrayIcon = nullptr;
	TMainDialog *mMainDialog = nullptr;
#if defined(__WXOSX__) && wxOSX_USE_COCOA
	TTrayIcon   *m_dockIcon;
#endif
	IniFile* mConfig = nullptr;

	void OnCommandEvent(wxCommandEvent& event);
	void OnProcessEvent(wxCommandEvent& event);

	void clear();
	void execCmds(const String &cmdStart);
public:
    virtual bool OnInit();

	

	~MyApp();
};


