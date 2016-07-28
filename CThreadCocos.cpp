#include "CThreadCocos.h"
#include "Classes/AppDelegate.h"
#include "cocos2d.h"
#include <QDebug>

// Include GLFW3
#define GLFW_DLL
#include "glfw3.h"
#ifndef GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#endif
#ifndef GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WGL
#endif

#ifdef _WIN32
#include "glfw3native.h"

// Include for native-native gl

#include <wingdi.h>
#endif


CThreadCocos::CThreadCocos():m_bContinue(true)
{
    //qDebug("Build a new CThreadCocos");
}


CThreadCocos::~CThreadCocos(){
    qDebug("destroy CThreadCocos");
}


void CThreadCocos::process(){

    AppDelegate app;
    int HWNDnumber = cocos2d::Application::getInstance()->runAndLink();
    emit sendHWND(HWNDnumber);
    emit sendKernel(app.getKernel());
    cocos2d::Application::getInstance()->runLoop();
    emit finished();
}
