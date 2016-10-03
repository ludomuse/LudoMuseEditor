
#include "CMainWindow.h"
#include <QApplication>
#include "LudoMuse_src/Classes/AppDelegate.h"
#include <QTimer>

int main(int argc, char *argv[])
{
    AppDelegate a(argc, argv);
    CMainWindow w;
    return cocos2d::CCQApplication::getInstance()->exec();
    //return a.exec();
}
