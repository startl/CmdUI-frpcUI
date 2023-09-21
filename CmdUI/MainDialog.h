#ifndef MainDialog_H
#define MainDialog_H

#include <wx/dialog.h>
#include <wx/richtext/richtextctrl.h>
#include <wx/sizer.h>

#include "PreDef.h"

class TDropTarget : public wxFileDropTarget
{
public:
	//TDropTarget(wxListBox *owner) { m_owner = owner; }

	virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames);
private:
	//wxListBox *m_owner;
};

class TMainDialog: public wxDialog
{
public:
	TMainDialog(wxWindow* parent,wxWindowID id=wxID_ANY);
	virtual ~TMainDialog();

	wxRichTextCtrl* RichTextCtrl1;
protected:
	//(*Identifiers(MainDialog)
	static const long ID_RICHTEXTCTRL1;
	//*)

	/*void OnAbout(wxCommandEvent& event);
	void OnOK(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);*/
	void OnCloseWindow(wxCloseEvent& event);

private:
	TDropTarget* mDropTarget = nullptr;
	DECLARE_EVENT_TABLE()
};

#endif
