#include "CmdUI.h"

#include <iostream>

#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/config.h>
#include <wx/dnd.h>

#include <asl/time.h>

#include "process.hpp"

using namespace std;
using namespace TinyProcessLib;

IMPLEMENT_APP(MyApp)


void MyApp::OnCommandEvent(wxCommandEvent& event)
{
	static auto _exit = [&]()
	{
		static bool _exited = false;

		if (_exited) return;
		_exited = true;

		String path = (*mConfig)["action/on_exit"] | "";
		if (path.ok()){
			asl::Process p;
			p.run(path);
		}

		clear();

		this->Exit();
	};

	auto param = event.GetString();
	if (param == "BtnClose") {
		if (!mConfig) return; //exited

		int b = (*mConfig)["func/close_btn_exit"] | 1;
		if (b) {
			_exit();
		}
		else {
			if (mMainDialog)
				mMainDialog->Hide();
		}
	}
	else if (param == "MenuExit") {
		_exit();
	}
	else if (param == "DropFiles")
	{
		wxArrayString* filesNames = static_cast<wxArrayString*>(event.GetClientData());
		wxString f = filesNames->Item(0);
		String fileName = f.ToStdWstring().c_str();
		fileName = "\"" + fileName + "\"";

		String cmdDrop = (*mConfig)["action/on_drop"] | "";
		String finalCmds;
		if (cmdDrop.ok())
		{
			auto cmds = cmdDrop.split("|");
			for (String cmd: cmds) {
				cmd = cmd.replace("%s", fileName);

				finalCmds += cmd + "|";
			}
			//finalCmds = finalCmds.substr(0, finalCmds.length() - 1);
			
			execCmds(finalCmds);
		}
	}
}


void MyApp::OnProcessEvent(wxCommandEvent& event)
{
	auto param = event.GetString();
	mMainDialog->RichTextCtrl1->AppendText(param); // + "\n"
}

void MyApp::clear()
{
	delete mConfig;
	mConfig = nullptr;

	if (mTrayIcon)
	{
		mTrayIcon->RemoveIcon();

		delete mTrayIcon;
#if defined(__WXCOCOA__)
		delete m_dockIcon;
#endif
	}

	if (mMainDialog)
	{
		mMainDialog->Close(true);
		mMainDialog = nullptr;
	}
}

bool MyApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

    if ( !wxTaskBarIcon::IsAvailable() )
    {
        wxMessageBox
        (
            "There appears to be no system tray support in your current environment. This sample may not behave as expected.",
            "Warning",
            wxOK | wxICON_EXCLAMATION
        );
    }

	gApp = this;

	mMainDialog = new TMainDialog(nullptr);
	mMainDialog->RichTextCtrl1->SetEditable(false);
	
	Bind(TCommandEvent, &MyApp::OnCommandEvent, this);
	Bind(TProcessEvent, &MyApp::OnProcessEvent, this);

	wxFileName f(wxStandardPaths::Get().GetExecutablePath());
	wxString appPath(f.GetPath());
	appPath.Append("/" + this->GetAppName() + ".ini");

	String s(appPath.ToStdWstring().c_str());
	mConfig = new IniFile(s, false);

	//title
	String title = (*mConfig)["base/title"] | this->GetAppName().ToStdWstring().c_str();
	mMainDialog->SetTitle(title.dataw());

	int t = (*mConfig)["func/show_tray"] | 1;
	if (t)
	{
		mTrayIcon = new TTrayIcon();
		mTrayIcon->SetMainDialog(mMainDialog);
		mTrayIcon->SetApp(this);
		
		// we should be able to show up to 128 characters on recent Windows versions (and 64 on Win9x)
		/*if (!mTrayIcon->SetIcon(wxICON(sample),
			"wxTaskBarIcon Sample\n"
			"With a very, very, very, very\n"
			"long tooltip whose length is\n"
			"greater than 64 characters."))*/
		if (!mTrayIcon->SetIcon(wxICON(cmd), title.dataw() ))
		{
			wxLogError(wxT("Could not set icon."));
		}

#if defined(__WXOSX__) && wxOSX_USE_COCOA
		m_dockIcon = new TTrayIcon(wxTBI_DOCK);
		m_dockIcon->SetMainDialog(mMainDialog);
		m_dockIcon->SetApp(this);
		if (!m_dockIcon->SetIcon(wxICON(cmd)))
		{
			wxLogError(wxT("Could not set icon."));
		}
#endif
	}

	int nDelay = (*mConfig)["func/start_delay"] | 0;
	if (nDelay > 0)
		sleep(nDelay); //seconds

	int h = (*mConfig)["func/start_hide"] | 0;
	if (!h)
		mMainDialog->Show();

	
	String cmdStart = (*mConfig)["action/on_start"] | "";
	if (cmdStart.ok())
	{
		execCmds(cmdStart);
	}

    return true;
}


void MyApp::execCmds(const String &cmdStart)
{
	//THREAD_BEGIN 

	Array<String> cmds = cmdStart.split("|");
	for (String cmd : cmds)
	{
		//弃用，asl::Process会自作聪明的加入双引号，也不支持直接输入命令行，比如
		//Exe64bitDetector.exe -f "C:\Program Files (x86)\Internet Explorer\iexplore.exe"
		//这种命令行，不容易通过分割空格形成数组

		//if (cmd.indexOf(" ") > 0)
		//{
		//	auto params = cmd.split(" ");
		//	String c = params[0];
		//	params.remove(c);
		//	for (auto p : params)
		//		wxMessageBox(p.dataw());
		//	try
		//	{
		//		Process p;
		//		for (int i = 0; i < params.length(); i++) //run会自动加上双引号
		//		{
		//			params[i].replace("\"", "");
		//		}
		//		/*for (auto p : params)
		//			wxMessageBox(p.dataw());*/
		//		
		//		p.run(c, params);
		//		while (true)
		//		{
		//			String line = p.readOutputLine();
		//			if (line == "\n")
		//				break;

		//			wxCommandEvent e(TProcessEvent);
		//			e.SetString(line.dataw());
		//			gApp->AddPendingEvent(e);
		//			//printf("Subprocess wrote %s\n", *line);
		//		}
		//		//Process p = Process::execute(c, params);
		//		/*if (p.success()) {
		//			auto out = p.output();

		//			wxCommandEvent e(TProcessEvent);
		//			e.SetString(out.dataw());
		//			gApp->AddPendingEvent(e);
		//		}*/
		//	}
		//	catch (...)
		//	{
		//	}
		//}
		//else 
		if (cmd.ok())
		{
			TinyProcessLib::Process::string_type c = cmd.dataw();
			TinyProcessLib::Process process2(
				c, L"",
				[](const char *bytes, size_t n)
				{
					string out = string(bytes, n);

					cout << "Output from stdout: " << out;
						
					wxCommandEvent e(TProcessEvent);
					e.SetString(out);
					gApp->AddPendingEvent(e);
				},
				[](const char *bytes, size_t n)
				{
					string err = string(bytes, n);
					//wxMessageBox(string(bytes, n));

					cout << "Output from stderr: " << err;
					// Add a newline for prettier output on some platforms:
					if (bytes[n - 1] != '\n')
						cout << endl;

					wxCommandEvent e(TProcessEvent);
					e.SetString(err);
					gApp->AddPendingEvent(e);
				});


				
			/*Process p;
			p.run(cmd);
			while (true)
			{
				String line = p.readOutputLine();
				if (line == "\n")
					break;

				wxCommandEvent e(TProcessEvent);
				e.SetString(line.dataw());
				gApp->AddPendingEvent(e);
			}*/
		}
	}
	///THREAD_END
}

MyApp::~MyApp()
{
	
}





