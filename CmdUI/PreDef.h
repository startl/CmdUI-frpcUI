#pragma once

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

// the application icon (under Windows and OS/2 it is in resources)
#ifndef wxHAS_IMAGES_IN_RESOURCES
#include "../sample.xpm"
#endif

#include <wx/app.h>
#include <wx/taskbar.h>

#include <wx/event.h>
wxDECLARE_EVENT(TCommandEvent, wxCommandEvent);
wxDECLARE_EVENT(TProcessEvent, wxCommandEvent);

extern wxApp* gApp;

#include <thread>
#define THREAD_BEGIN std::thread([=](){
#define  THREAD_END 	}).detach();