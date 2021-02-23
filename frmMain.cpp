#include "frmMain.h"

#include <QDebug>

#include <QTextCodec>
#include <QMimeData>

#ifdef Q_OS_WIN
#include "qt_windows.h"
#endif

#include "TimeHelper.h"

frmMain::frmMain(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	//this->setAttribute(Qt::WA_DeleteOnClose);

	
	mCfg = new QSettings(gPath.appDir + "/" + gPath.appName + ".ini", QSettings::IniFormat);
	mCfg->setIniCodec(QTextCodec::codecForName("UTF8"));

	if (mCfg->value("func/show_tray").toBool())
	{
		CTrayMenu* tray_menu = new CTrayMenu();
		connect(tray_menu, SIGNAL(onActionQuit()), this, SLOT(onQuit()));
		//connect(tray_menu, SIGNAL(onActionPause(bool)), this, SLOT(onPauseBrowser(bool)));
		mTray.setContextMenu(tray_menu);

		mTray.setIcon(QIcon(":/icon/cmd.png"));

		connect(&mTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
			SLOT(trayIsActived(QSystemTrayIcon::ActivationReason)));

		mTray.show();
	}

	ui.recvEdit->setReadOnly(true);
	setAcceptDrops(true);
	ui.recvEdit->setAcceptDrops(false);

	QString title = mCfg->value("base/title", "CmdUI").toString() + "-[CmdUI]";
	mTray.setToolTip(title);
	this->setWindowTitle(title);

	QProcess* p = new QProcess(this);
	connect(p, SIGNAL(readyReadStandardOutput()), this, SLOT(onCmdReadOutput()));
	connect(p, SIGNAL(readyReadStandardError()), this, SLOT(onCmdReadError()));

	uint nDelay = mCfg->value("func/start_delay", 0).toUInt();
	CTimeHelper::sleep(nDelay * 1000);

	QString host = mCfg->value("base/host", "").toString();
	if (host.isEmpty()) return;
	p->start(host);
	p->waitForStarted();

	QString cmdStart = mCfg->value("action/on_start", "").toString();
	//!host.isEmpty() && !
	if (!cmdStart.isEmpty())
	{
		QStringList list = cmdStart.split("|");
		for (QString c:list)
		{
			p->write((c + "\n").toStdString().c_str());
		}
		//QProcess p(0);
		//p.waitForFinished();
		//QString strOut = QString::fromLocal8Bit(p.readAllStandardOutput());
		//ui.recvEdit->appendPlainText(strOut);
	}

	if (!mCfg->value("func/start_hide", 1).toBool())
		show();
}


void frmMain::onCmdReadOutput()
{
	QProcess* p = static_cast<QProcess*>(sender());
	QByteArray qba = p->readAllStandardOutput();
	QTextCodec* pTextCodec = QTextCodec::codecForName("System");
	assert(pTextCodec != nullptr);
	QString str = pTextCodec->toUnicode(qba);
	ui.recvEdit->appendPlainText(str);   
}

void frmMain::onCmdReadError()
{
	//QMessageBox::information(0, "Error", cmd->readAllStandardError().data());    
}

void frmMain::closeEvent(QCloseEvent* event)
{
	if (!mCfg->value("func/close_btn_exit", 0).toBool())
	{
		event->ignore();
		hide();
	}
	else
	{
		event->accept();
	}
}

void frmMain::changeEvent(QEvent* event)
{
	if ((event->type() == QEvent::WindowStateChange) && isMinimized())
	{
		if (!mCfg->value("func/close_btn_exit", 0).toBool()) 
		{
			event->ignore();
			hide();
		}
		else
		{
			event->accept();
		}
		//mTray.showMessage("SystemTrayIcon", QObject::trUtf8("程序最小化到托盘"), QSystemTrayIcon::Information, 5000);
	}
}

void frmMain::dragEnterEvent(QDragEnterEvent* event)
{
	if (event->mimeData()->hasFormat("text/uri-list"))
		event->acceptProposedAction();
}

void frmMain::dropEvent(QDropEvent* event)
{
	QList<QUrl> urls = event->mimeData()->urls();
	if (urls.isEmpty()) return;

	QString fileName = urls.first().toLocalFile();
	if (fileName.isEmpty()) return;

	QString host = mCfg->value("base/host", "").toString();
	if (host.isEmpty()) return;

	QProcess* p = new QProcess(this);
	connect(p, SIGNAL(readyReadStandardOutput()), this, SLOT(onCmdReadOutput()));
	connect(p, SIGNAL(readyReadStandardError()), this, SLOT(onCmdReadError()));

	QString cmdDrop = mCfg->value("action/on_drop", "").toString();
	if (!cmdDrop.isEmpty())
	{
		QStringList list = cmdDrop.split("|");
		for (QString c : list)
		{
			c = c.replace("%s", "%1");
			c = c.arg(fileName);
			p->start(host, QStringList() << "/c" << c); //start方式要加/c
			p->waitForStarted();
			p->waitForFinished();
		}
	}
	delete p;
}

void frmMain::showFront()
{
	showNormal();

#ifdef Q_OS_WIN
	::SetWindowPos(HWND(this->winId()), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	::SetWindowPos(HWND(this->winId()), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
#endif
	this->activateWindow();
}

void frmMain::trayIsActived(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
	case QSystemTrayIcon::Trigger:
	{
		showFront();
		break;
	}
	case QSystemTrayIcon::DoubleClick:
	{
		showFront();
		break;
	}
	default:
		break;
	}
}

void frmMain::onQuit()
{
	QString host = mCfg->value("base/host", "").toString();
	if (host.isEmpty()) return;

	QProcess* p = new QProcess(this);
	QString cmdExit = mCfg->value("action/on_exit", "").toString();
	if (!cmdExit.isEmpty())
	{
		QStringList list = cmdExit.split("|");
		for (QString c : list)
		{
			p->start(host, QStringList() << "/c" << c); //start方式要加/c
			p->waitForStarted();
			p->waitForFinished();
		}
	}

	// uint nDelay = mCfg->value("func/exit_delay", 0).toUInt();
	// CTimeHelper::sleep(nDelay * 1000);

	exit(0);
}
