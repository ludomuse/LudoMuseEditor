#pragma once

#include "cocos2d.h"

#include <QMouseEvent>
#include <QWidget>
#include "glfw3.h"
#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include "glfw3native.h"
#endif
#ifdef __linux__
#define GLFW_EXPOSE_NATIVE_X11
#endif

#ifndef _WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#endif
#ifdef __linux__
#define GLFW_EXPOSE_NATIVE_EGL
#endif

typedef void(*KEY_PTRFUN)(QKeyEvent *event);

class CCQGLWidget;

NS_CC_BEGIN

class CCQGLView : public GLViewImpl
{
public:
    CCQGLView();
    virtual ~CCQGLView();

    /* override functions */
    virtual bool isOpenGLReady() override;
    virtual void end() override;
    virtual void swapBuffers() override;
    virtual void setFrameSize(float width, float height) override;
    virtual void setIMEKeyboardState(bool bOpen) override;
    virtual void setViewName(const char* pszViewName);

    /* Qt functions */
    void mouseMove(QMouseEvent *event);
    void mousePress(QMouseEvent *event);
    void mouseRelease(QMouseEvent *event);
    void wheel(QWheelEvent *event);
    QWidget * getGLWidget();

    void resize(); // auto adjust frame size according to current window size
    void updateWindowSize();
    void setBgColor(Color4B &color);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    HWND getWin32Window() { return NULL; }
#endif /* (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) */

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    id getCocoaWindow() override { return nil; }
#endif // #if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)


    /* get the shared main open gl window */
    static CCQGLView* getInstance();
    /* create instance with base widget size */
    static bool Create();
    static bool Create(int width, int height);

private:

    bool Init(int width, int height);

    bool			_isInit;
    float			_screenScaleFactor;
    float			_lastWindowHeight;
    Color4F			_bgColor;
    CCQGLWidget*	_window;
};

NS_CC_END

