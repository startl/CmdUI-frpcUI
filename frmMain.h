#pragma once

#include <QtWidgets/QMainWindow>
#include <QtNetwork/QTcpServer>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QSettings>
#include <QProcess>
#include <QDragEnterEvent>
#include <QDropEvent>

#include "ui_CmdUI.h"
#include "Global.h"

#pragma execution_character_set("utf-8")

class CTrayMenu : public QMenu
{
	Q_OBJECT
signals:
	void onActionQuit();
	void onActionPause(bool);
public:
	QAction* action_pause_browser = nullptr;

	CTrayMenu()
	{
		// action_pause_browser = new QAction(this);
		// action_pause_browser->setText(QString("暂停打开浏览器"));
		// action_pause_browser->setCheckable(true);

		QAction* action_quit = new QAction(this);
		action_quit->setText(QString("退出"));
		//action_about->setText(QString("关于"));
		//action_check_update->setText(QString("检查更新"));
		//action_setting->setText(QString("设置")

		// this->addAction(action_pause_browser);
		// this->addSeparator();
		this->addAction(action_quit);

		QObject::connect(action_quit, SIGNAL(triggered(bool)), this, SIGNAL(onActionQuit()));
		//QObject::connect(action_pause_browser, SIGNAL(triggered(bool)), this, SIGNAL(onActionPause(bool)));
		//QObject::connect(action_quit, SIGNAL(triggered(bool)), this, SLOT(onQuit(bool)));
	}
};

class frmMain : public QMainWindow
{
    Q_OBJECT
private slots:
	void trayIsActived(QSystemTrayIcon::ActivationReason reason);

	void onQuit();
	void onCmdReadOutput();
	void onCmdReadError();
public:
    frmMain(QWidget *parent = Q_NULLPTR);
protected:
	virtual void closeEvent(QCloseEvent* event) override;
	virtual void changeEvent(QEvent* event) override;
	virtual void dragEnterEvent(QDragEnterEvent* event) override;
	virtual void dropEvent(QDropEvent* event) override;
private:
    Ui::CmdUIClass ui;

	QSettings* mCfg = nullptr;
	QSystemTrayIcon mTray;

	bool mPaused = false;
	void showFront();
};
