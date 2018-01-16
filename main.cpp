#include "CMainWindow.h"
#include <QApplication>
#include "LudoMuse_src/Classes/AppDelegate.h"

int main(int argc, char *argv[])
{
#ifdef TARGET_OS_MAC
    QDir dir(argv[0]); // e.g. appdir/Contents/MacOS/appname
    assert(dir.cdUp());
    assert(dir.cdUp());
    assert(dir.cd("PlugIns")); // e.g. appdir/Contents/PlugIns
    QCoreApplication::setLibraryPaths(QStringList(dir.absolutePath()));

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

#endif
    AppDelegate a(argc, argv);
    CMainWindow w;
    return cocos2d::CCQApplication::getInstance()->exec();
    //return a.exec();
}
