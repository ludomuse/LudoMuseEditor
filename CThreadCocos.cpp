#include "CThreadCocos.h"
//#include "CCocosWindow.h"
#include "Classes/AppDelegate.h"
#include "cocos2d.h"
#include <QDebug>

// Include GLFW3
#define GLFW_DLL
#include "glfw3.h"

#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#endif
#ifdef __linux__
#define GLFW_EXPOSE_NATIVE_LINUX
#endif

#ifndef _WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#endif
#ifdef __linux__
#define GLFW_EXPOSE_NATIVE_EGL
#endif

#include "glfw3native.h"

// Include for native-native gl
#ifdef _WIN32
#include <wingdi.h>
#endif

CThreadCocos::CThreadCocos(const QString& a_sPath):m_sProjectPath(a_sPath)
{
    //qDebug("Build a new CThreadCocos");
}


CThreadCocos::~CThreadCocos()
{
    qDebug("destroy CThreadCocos");
}


void CThreadCocos::process()
{
    AppDelegate app(true, m_sProjectPath.toStdString());
    int HWNDnumber = cocos2d::Application::getInstance()->runAndLink();
    emit sendHWND(HWNDnumber);
    emit sendKernel(app.getKernel());
    cocos2d::Application::getInstance()->runLoop();
    emit finished();
}
