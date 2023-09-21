#include "MainDialog.h"

#include <wx/intl.h>
#include <wx/string.h>


const long TMainDialog::ID_RICHTEXTCTRL1 = wxNewId();


void TMainDialog::OnCloseWindow(wxCloseEvent& event)
{
	static bool _closed = false;

	if (_closed) return; //·ÀÖ¹ÖØ¸´¼¤·¢

	_closed = true;
	//Destroy();
	wxCommandEvent e(TCommandEvent);
	//e.SetEventObject(&event);
	e.SetString("BtnClose");
	//ProcessWindowEvent(e);
	gApp->AddPendingEvent(e);
}

BEGIN_EVENT_TABLE(TMainDialog, wxDialog)
	EVT_CLOSE(TMainDialog::OnCloseWindow)
END_EVENT_TABLE()

TMainDialog::TMainDialog(wxWindow* parent,wxWindowID id)
{
	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, 
		wxDEFAULT_DIALOG_STYLE | wxMAXIMIZE_BOX | wxMINIMIZE_BOX | wxRESIZE_BORDER, _T("id"));
	SetClientSize(wxSize(582,400));

	wxBoxSizer* BoxSizer1;
	BoxSizer1 = new wxBoxSizer(wxHORIZONTAL);
	
	RichTextCtrl1 = new wxRichTextCtrl(this, ID_RICHTEXTCTRL1, wxEmptyString, wxDefaultPosition, wxSize(582, 400), wxRE_MULTILINE, wxDefaultValidator, _T("ID_RICHTEXTCTRL1"));
	wxRichTextAttr rchtxtAttr_1;
	rchtxtAttr_1.SetBulletStyle(wxTEXT_ATTR_BULLET_STYLE_ALIGN_LEFT);
	BoxSizer1->Add(RichTextCtrl1, 1, wxALL|wxEXPAND, 1); //wxExpand ºáÊú¶¼À­Éì

	SetSizer(BoxSizer1);

	Layout();
	
	Center();

	SetTitle("CmdUI");

	mDropTarget = new TDropTarget;
	RichTextCtrl1->SetDropTarget(mDropTarget);
}

//
//TMainDialog::TMainDialog(const wxString& title): wxDialog(NULL, wxID_ANY, title)
//{
//
//}

TMainDialog::~TMainDialog()
{
	
}


bool TDropTarget::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
{
	size_t nFiles = filenames.GetCount();
	wxString str;
	str.Printf(wxT("%d files dropped"), (int)nFiles);
	/*m_owner->Append(str);
	for (size_t n = 0; n &lt; nFiles; n++)
	{
		m_owner->Append(filenames[n]);
	}*/

	wxCommandEvent e(TCommandEvent);
	e.SetClientData((void*)&filenames);
	e.SetString("DropFiles");
	gApp->AddPendingEvent(e);

	return true;
}
