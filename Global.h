#pragma once

#include <QDir>
#include <QMessageBox>
#include "FileHelper.h"

#pragma execution_character_set("utf-8")
#define debugout qDebug()

struct CPath
{
public:
	CPath()
	{
	};

	void init(QString fullPath, bool ensureDataDir = true)
	{
		this->appPath = fullPath;
		QDir appDir(appPath);
		QString appName = appDir.dirName();
		QString dir = appPath.left(appPath.length() - appName.length());
		this->appDir = dir;
		appName = appName.left(appName.lastIndexOf(".exe"));
		this->appName = appName;
		
		dataBase = this->appDir + "data/";
		cache = dataBase + "cache/";

		userBase = this->appDir + "users/";

		if (ensureDataDir)
		{
			CFileHelper::ensureDirExist(dataBase);
			CFileHelper::ensureDirExist(cache);
		}
	}

	QString appPath, appDir, appName, dataBase, cache, userBase, userData, userFiles;
};

extern CPath gPath;