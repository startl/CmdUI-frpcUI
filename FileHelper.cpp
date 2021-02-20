#include "FileHelper.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDesktopServices>
#include <QTextStream>
#include <QCryptographicHash>
#include <QUrl>
#include <stdio.h>
#include <QDesktopWidget>
#include <QDebug>
#include <QTextCodec>
#include <QProcess>
#ifdef Q_OS_WIN
#include <Windows.h>
#endif


#pragma execution_character_set("utf-8")




QString CFileHelper::getUniqueName(const QString& fileName)
{
	QString filePath = fileName;
	QFileInfo infoOriginal(fileName);
	uint i = 0;
	QFileInfo info(fileName);
	while (info.isFile())
	{
		++i;
		filePath = infoOriginal.absolutePath();
		if (filePath.right(1) != "/")
			filePath += "/";
		filePath += infoOriginal.baseName() + "(" + QString::number(i) + ")." + infoOriginal.completeSuffix();
		info.setFile(filePath);
	}
	return filePath;
}

bool CFileHelper::isFileExist(const QString& fullFileName)
{
	QFileInfo fileInfo(fullFileName);
	return fileInfo.exists();
}

QString CFileHelper::getFileExt(const QString& fullFileName)
{
	QFileInfo fileInfo(fullFileName);
	return std::move(fileInfo.suffix());
}

QString CFileHelper::getFileName(const QString& fullFileName)
{
	QFileInfo fileInfo(fullFileName);
	return std::move(fileInfo.fileName());
}

QString CFileHelper::getFileBaseName(const QString& fullFileName)
{
	QFileInfo fileInfo(fullFileName);
	return std::move(fileInfo.baseName());
}

QString CFileHelper::getFilePath(const QString& fullFileName)
{
	QFileInfo fileInfo(fullFileName);
	return std::move(fileInfo.path());
}

bool CFileHelper::isDirExist(const QString& fullPath)
{
	QDir dir(fullPath);
	return dir.exists();
}

QString CFileHelper::readFile(const QString& fileName)
{
	QString displayString;
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return "";

	QTextStream in(&file);
	QString line = in.readLine();
	displayString.append(line);

	while (!line.isNull())
	{
		line = in.readLine();
		displayString.append(line);
	}

	return std::move(displayString);
}

bool CFileHelper::ensureDirExist(QString fullPath)
{
	if (!isDirExist(fullPath))
	{
		QDir d;
		return d.mkpath(fullPath); //只创建一级子目录，即必须保证上级目录存在
	}

	return true;
}

QString CFileHelper::ensurePathLastDelimiter(const QString& path)
{
	QString dir = path;
	if (dir.right(1) != "/") dir += "/";
	return std::move(dir);
}

QByteArray CFileHelper::getFileMd5(const QString& filePath)
{
	QFile localFile(filePath);

	if (!localFile.open(QFile::ReadOnly)) //E~J$%0A[3YPI6YP%7DEO4KZJG.jpg ，这种，不用readwrite会失败
	{
		//qDebug() << "file open error.";
		return nullptr;
	}

	QCryptographicHash ch(QCryptographicHash::Md5);

	quint64 totalBytes = 0;
	quint64 bytesWritten = 0;
	quint64 bytesToWrite = 0;
	quint64 jumpSize = 1024 * 1024 * 10;
	quint64 readSize = 1024 * 1024;
	QByteArray buf;

	totalBytes = localFile.size();
	bytesToWrite = totalBytes;

	while (true)
	{
		if (bytesToWrite > 0)
		{
			buf = localFile.read(qMin(bytesToWrite, readSize));
			ch.addData(buf);

			bytesWritten += jumpSize;
			if (bytesToWrite < jumpSize) break;
			bytesToWrite -= jumpSize;

			localFile.seek(bytesWritten);

			buf.resize(0);
		}
		else
		{
			break;
		}

		if (bytesWritten == totalBytes)
		{
			break;
		}
	}

	localFile.close();
	QByteArray md5 = ch.result();

	return md5; //字符串 tohex
}

QString CFileHelper::getFileMd5Str(const QString& filePath)
{
	return std::move(QString(getFileMd5(filePath).toHex()));
}

//#include <QImage>
//QByteArray getImageMd5(const QImage& image)
//{
//	QCryptographicHash ch(QCryptographicHash::Md5);
//
//	quint64 totalBytes = 0;
//	quint64 bytesWritten = 0;
//	quint64 bytesToWrite = 0;
//	quint64 jumpSize = 1024 * 1024 * 10;
//	quint64 readSize = 1024 * 1024;
//	QByteArray buf;
//
//	totalBytes = image.byteCount();
//	bytesToWrite = totalBytes;
//
//	while (1)
//	{
//		if (bytesToWrite > 0)
//		{
//			buf = localFile.read(qMin(bytesToWrite, readSize));
//			ch.addData(buf);
//
//			bytesWritten += jumpSize;
//			if (bytesToWrite < jumpSize) break;
//			bytesToWrite -= jumpSize;
//
//			localFile.seek(bytesWritten);
//
//			buf.resize(0);
//		}
//		else
//		{
//			break;
//		}
//
//		if (bytesWritten == totalBytes)
//		{
//			break;
//		}
//	}
//
//	QByteArray md5 = ch.result();
//
//	return md5; //字符串 tohex
//}

QString CFileHelper::getFileSizeDesc(ulong llBytes)
{
	double bytes = static_cast<double>(llBytes);
	unsigned long cIter = 0;
	const char* pszUnits[] = {("B"), ("KB"), ("MB"), ("GB"), ("TB")};
	unsigned long cUnits = sizeof(pszUnits) / sizeof(pszUnits[0]);

	// move from bytes to KB, to MB, to GB and so on diving by 1024
	while (bytes >= 1024 && cIter < (cUnits - 1))
	{
		bytes /= 1024;
		cIter++;
	}
	char sc[256];

	snprintf(sc, sizeof(sc), ("%.2f %s"), bytes, pszUnits[cIter]);

	return std::move(QString::fromStdString(sc));
}

qint64 CFileHelper::getFileSize(const QString& filePath)
{
	QFileInfo info(filePath);
	return info.size();
}

quint64 CFileHelper::getDirSize(const QString& path)
{
	QDir dir(path);
	dir.setFilter(dir.filter() | QDir::Hidden | QDir::NoSymLinks);

	quint64 size = 0;

	//dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot)返回所有子目录，并进行过滤 
	foreach(QString subDir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
	{
		//若存在子目录，则递归调用dirFileSize()函数 
		size += getDirSize(path + QDir::separator() + subDir);
	}

	//dir.entryInfoList(QDir::Files)返回文件信息 
	foreach(QFileInfo fileInfo, dir.entryInfoList(QDir::Files))
	{
		//计算文件大小 
		size += fileInfo.size();
	}
	
	return size;
}

quint64 CFileHelper::getDirFilesCount(const QString& path)
{
	quint64 n = 0;
	QDir dir(path);
	dir.setFilter(dir.filter() | QDir::Hidden | QDir::NoSymLinks);

	//dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot)返回所有子目录，并进行过滤 
	foreach(QString subDir, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
	{
		//若存在子目录，则递归调用dirFileSize()函数 
		n += getDirFilesCount(path + QDir::separator() + subDir);
	}

	foreach(QFileInfo fileInfo, dir.entryInfoList(QDir::Files))
	{
		n++;
	}

	return n;
}

void CFileHelper::openDir(const QString& dir)
{
	QDesktopServices::openUrl(QUrl::fromLocalFile(dir));
}

void CFileHelper::openFile(const QString& file)
{
	bool is_open = QDesktopServices::openUrl(QUrl::fromLocalFile(file));
	//QString url = "file:///" + file;
	//bool is_open = QDesktopServices::openUrl(QUrl(url, QUrl::TolerantMode)); //有些机器返回failed 2
	if (!is_open)
	{
	}

	//#ifdef Q_OS_WIN
	//	
	//#else
	//	QString cmd = "xdg-open " + file;　//在linux下，可以通过system来xdg-open命令调用默认程序打开文件；
	//	system(cmd.toStdString().c_str());
	//#endif
}

void CFileHelper::recruseDirFiles(const QString& path, QStringList& outList, const QStringList& filterList)
{
	QDir dir(path);
	dir.setFilter(dir.filter() | QDir::Hidden|QDir::NoSymLinks);

	foreach(QFileInfo fi, dir.entryInfoList())
	{
		if (fi.isFile())
		{
			//qDebug() << fi.suffix();
			//qDebug() << "File :" << fi.fileName();
			//if (mfi.suffix())
			if (filterList.size() > 0)
			{
				if (filterList.contains(fi.suffix().toLower()))
					outList.append(fi.absoluteFilePath());
			}
			else
				outList.append(fi.absoluteFilePath());
		}
		else
		{
			if (fi.fileName() == "." || fi.fileName() == "..")continue;
			//qDebug() << "Entry Dir" << fi.absoluteFilePath();
			recruseDirFiles(fi.absoluteFilePath(), outList, filterList);
		}
	}
}

void CFileHelper::recruseDirFiles(const QString& path, QStringList& outList, bool includeSubDir /*= false*/)
{
	QDir dir(path);
	dir.setFilter(dir.filter() | QDir::Hidden | QDir::NoSymLinks);

	foreach(QFileInfo fi, dir.entryInfoList())
	{
		if (fi.isFile())
		{
			outList.append(fi.absoluteFilePath());
		}
		else if (includeSubDir)
		{
			if (fi.fileName() == "." || fi.fileName() == "..")continue;
			//qDebug() << "Entry Dir" << fi.absoluteFilePath();
			recruseDirFiles(fi.absoluteFilePath(), outList, includeSubDir);
		}
	}
}

void CFileHelper::locateFile(const QString& qsFilePath)
{
	QString path = qsFilePath;
	QString cmd;
#ifdef Q_OS_WIN
	path = path.replace("/", "\\");
	cmd = QString("/select,\"%1\"").arg(path);
	std::wstring s = cmd.toStdWString();
	ShellExecute(NULL, L"open", L"explorer.exe", s.c_str(), NULL, SW_SHOW);
#else
    path = path.replace("\\", "/");
	cmd = QString("open -R %1").arg(path);

	QProcess process;
	process.startDetached(cmd);
#endif
	
}

bool CFileHelper::deleteFile(const QString& filePath)
{
	QFile::setPermissions(filePath, QFile::WriteOwner);
	return QFile::remove(filePath);
}

void CFileHelper::readTxtFileToList(const QString& path, QStringList& outList, const char* codecName/*="UTF8"*/)
{
	QFile txtFile(path);
	if (txtFile.open(QFile::ReadOnly | QIODevice::Text))
	{
		QTextCodec* codec = QTextCodec::codecForName(codecName);
		QTextStream data(&txtFile);
		data.setCodec(codec);
		QString line;
		while (!data.atEnd())//逐行读取文本，并去除每行的回车
		{
			line = data.readLine();
			//line = codec->toUnicode(line.toLatin1());
			line.remove('\n');
			outList << line;
		}
	}
	txtFile.close();
}

void CFileHelper::writeTxtToFile(const QString& s, const QString& filePath)
{
	QFile file(filePath);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream stream(&file);
		stream << s << endl; 
	}
	file.close();
}

QString CFileHelper::getUpDir(const QString& sDir)
{
	QString s = sDir;
	s = s.replace("\\", "/");
	s = s.left(s.lastIndexOf("/"));
	return s;
}

int CFileHelper::deleteDir(QString dirName)
{
	QDir dir(dirName);
	return dir.removeRecursively();

	//以下疑似有缓冲效果，就是删除后会过段时间生效
	// QDir directory(dirName);
	// if (!directory.exists())
	// {
	// 	return true;
	// }
	//
	// QString srcPath = QDir::toNativeSeparators(dirName);
	// if (!srcPath.endsWith(QDir::separator()))
	// 	srcPath += QDir::separator();
	//
	//
	// QStringList fileNames = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
	// bool error = false;
	// for (QStringList::size_type i = 0; i != fileNames.size(); ++i)
	// {
	// 	QString filePath = srcPath + fileNames.at(i);
	// 	QFileInfo fileInfo(filePath);
	// 	if (fileInfo.isFile() || fileInfo.isSymLink())
	// 	{
	// 		QFile::setPermissions(filePath, QFile::WriteOwner);
	// 		if (!QFile::remove(filePath))
	// 		{
	// 			error = true;
	// 		}
	// 	}
	// 	else if (fileInfo.isDir())
	// 	{
	// 		if (!deleteDir(fileInfo.absoluteFilePath()))
	// 		{
	// 			error = true;
	// 		}
	// 	}
	// }
	//
	// if (!directory.rmdir(QDir::toNativeSeparators(directory.path())))
	// {
	// 	error = true;
	// }
	// return !error;
}

//拷贝文件：
bool CFileHelper::copyFile(const QString& sourceFilePath, const QString& toFilePath, bool overwrite)
{
	QString toPath = toFilePath;
	toPath = toPath.replace("\\", "/");
	if (sourceFilePath == toPath) {
		return true;
	}
	if (!QFile::exists(sourceFilePath)) {
		return false;
	}
	QDir* createfile = new QDir;
	bool exist = createfile->exists(toPath);
	if (exist) {
		if (overwrite) {
			createfile->remove(toPath);
		}
	}//end if

	if (!QFile::copy(sourceFilePath, toPath))
	{
		return false;
	}
	return true;
}

//拷贝文件夹：
bool CFileHelper::copyDir(const QString& fromDir, const QString& toDir, bool overwrite, bool errorContine, const QVector<QString>* ignoreNames)
{
	QDir sourceDir(fromDir);
	QDir targetDir(toDir);
	if (!targetDir.exists()) {    /**< 如果目标目录不存在，则进行创建 */
		if (!targetDir.mkpath(toDir))
			return false;
		// if (!targetDir.mkdir(targetDir.absolutePath()))
		// 	return false;
	}

	QFileInfoList fileInfoList = sourceDir.entryInfoList();
	foreach(QFileInfo fileInfo, fileInfoList) {
		if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
			continue;

		if (fileInfo.isDir()) {    /**< 当为目录时，递归的进行copy */

			bool b = true;
			if (ignoreNames)
			{
				b = !ignoreNames->contains(fileInfo.fileName());
			}

			if (b) {
				if (!copyDir(fileInfo.filePath(), targetDir.filePath(fileInfo.fileName()), overwrite, errorContine, ignoreNames))
				{
					return false;
				}
					
			}
		}
		else {            /**< 当允许覆盖操作时，将旧文件进行删除操作 */
			if (overwrite && targetDir.exists(fileInfo.fileName())) {
				targetDir.remove(fileInfo.fileName());
			}

			/// 进行文件copy
			bool b = true;
			if (ignoreNames)
			{
				b = !ignoreNames->contains(fileInfo.fileName());
			}

			if (b) {
				if (!QFile::copy(fileInfo.filePath(),
					targetDir.filePath(fileInfo.fileName()))) {
					QString s = fileInfo.fileName();
					qDebug() << "拷贝文件失败：" << s;

					if (errorContine)
						continue;
					else
					   return false;
				}
				else
				{
					// QString s = fileInfo.fileName();
					// qDebug() << "拷贝文件成功：" << s;
				}
			}
		}
	}
	return true;
}