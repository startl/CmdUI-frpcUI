#include "TimeHelper.h"
#if defined(Q_OS_WIN)
#include <qt_windows.h>
#else
#include <unistd.h>
#endif

#pragma execution_character_set("utf-8")

QTime CTimeHelper::time;

void CTimeHelper::sleepIdle(uint msec)
{
	QElapsedTimer t;
	t.start();
	while (t.elapsed() < msec) {
		QCoreApplication::processEvents();
		CTimeHelper::sleep(1);
	}
}

void CTimeHelper::sleep(uint msec)
{
#if defined(Q_OS_WIN)
	Sleep(msec);
#else
	usleep(msec * 1000);
#endif
}

QString CTimeHelper::secToTimeStr(uint curSec, bool showHourAlways /*= true*/)
{
	QString time;
	if (curSec < 60)
	{
		if (showHourAlways)
			time = QString("00:00:%1").arg(curSec, 2, 10, QChar('0'));
		else
			time = QString("00:%1").arg(curSec, 2, 10, QChar('0'));
	}
	else if (curSec > 60 && curSec < 3600)
	{
		uint min = curSec / 60;
		uint sec = curSec - min * 60;

		if (showHourAlways)
		    time = QString("00:%1:%2").arg(min, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0'));
		else
			time = QString("%1:%2").arg(min, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0'));

	}
	else if (curSec > 3600)
	{
		uint hour = curSec / 3600;
		uint min = (curSec - hour * 3600) / 60;
		uint sec = curSec - hour * 3600 - min * 60;
		time = QString("%1:%2:%3").arg(hour, 2, 10, QChar('0')).arg(min, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0'));
	}

	return time;
}

QString CTimeHelper::getFriendlyDesc(const QDateTime& dt)
{
	QString strDateTime;
	QLocale locale = QLocale::Chinese; //指定中文显示
	QDateTime now = QDateTime::currentDateTime();
	if (now.date() == dt.date())
	{
		 strDateTime = locale.toString(dt, "AP hh:mm");
	}
	else if (dt.daysTo(now) == 1)
	{
		 strDateTime = locale.toString(dt, ("昨天 hh:mm"));

	}
	else if (dt.daysTo(now) == 2)
	{
		 strDateTime = locale.toString(dt, ("前天 hh:mm"));

	}
	else
	{

		if (now.date().year() != dt.date().year())
		{
			int n = now.date().year() - dt.date().year();
			if (n == 1)
				strDateTime = locale.toString(dt, "去年/M/d hh:mm");
			else if (n == 2)
				strDateTime = locale.toString(dt, "前年/M/d hh:mm");
			else
				strDateTime = locale.toString(dt, "yy/M/d hh:mm");
		}
		else
			strDateTime = locale.toString(dt, "M/d hh:mm");
	}

	return std::move(strDateTime);
}
