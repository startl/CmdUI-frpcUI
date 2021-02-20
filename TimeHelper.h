#pragma once

#include <QElapsedTimer>
#include <QCoreApplication>
#include <QTime>
#include <QTimer>
#include <QDebug>

class CTimeHelper
{
	static QTime time;
public:
	static void sleepIdle(uint msec);
	static void sleep(uint msec);

	static void elapseBegin()
	{
		time.start();
	}

	static void elapseEnd(const QString& sDesc = "")
	{
		int time_Diff = time.elapsed(); //返回从上次start()或restart()开始以来的时间差，单位ms
		QString tr_timeDiff = QString("%1耗时%2ms").arg(sDesc).arg(time_Diff); //float->QString
		qDebug() << tr_timeDiff;
		//printf("operate time: %.2fms", (time_Finish - time_Start); //输出
	}

	static QString secToTimeStr(uint curSec, bool showHourAlways = true);

	template <typename F>
	static void delayDo(uint msec, QObject* receiver, F const& func)
	{
		QTimer::singleShot(msec, receiver, func);
	}

	template <typename F>
	static void delayDo(uint msec, F const& func)
	{
		QTimer::singleShot(msec, nullptr, func);
	}

	static QString getFriendlyDesc(const QDateTime& dt);
};
