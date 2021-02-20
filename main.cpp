#include "frmMain.h"
#include "Global.h"
#include <QtWidgets/QApplication>

#pragma comment(lib, "version.lib")
#pragma comment(lib, "Netapi32.lib")
#pragma comment(lib, "userenv.lib")
#pragma comment(lib, "msvcrtd.lib")
#pragma comment(lib, "Dwmapi.lib")
#pragma comment(lib, "wtsapi32.lib")
#pragma comment(lib, "Wsock32.lib") 
#pragma comment(lib, "Imm32.lib") 
#pragma comment(lib, "Winmm.lib") 
#pragma comment(lib, "ws2_32.lib") 
#pragma comment(lib, "Iphlpapi.lib") 
#pragma comment(lib, "uxtheme.lib")

#pragma execution_character_set("utf-8")

// #include <QtPlugin>
//
// Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    gPath.init(QString::fromLocal8Bit(argv[0]), false);

    frmMain w;
    //w.show();
    return a.exec();
}
