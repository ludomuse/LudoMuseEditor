#pragma once

#include "cocos2d.h"
#include <QtOpenGL/QGLWidget>

class QTimer;

NS_CC_BEGIN

typedef void(*MOUSE_PTRFUN)(QMouseEvent *event);
typedef void(*WHEEL_PTRFUN)(QWheelEvent *event);
typedef void(*KEY_PTRFUN)(QKeyEvent *event);

class CCQGLWidget : public QGLWidget
{
    Q_OBJECT

public:
    CCQGLWidget(){}
    CCQGLWidget(int width, int height, QWidget *parent = 0);
    ~CCQGLWidget();

    void setMouseMoveFunc(MOUSE_PTRFUN func);
    void setMousePressFunc(MOUSE_PTRFUN func);
    void setMouseReleaseFunc(MOUSE_PTRFUN func);
	void setWheelFunc(WHEEL_PTRFUN func);
    void setKeyEventFunc(KEY_PTRFUN func);

protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void wheelEvent(QWheelEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);

    virtual void resizeEvent(QResizeEvent *event) override;

    virtual QSize sizeHint() const override;


private:
    MOUSE_PTRFUN mouseMoveFunc;
    MOUSE_PTRFUN mousePressFunc;
    MOUSE_PTRFUN mouseReleaseFunc;
	WHEEL_PTRFUN wheelFunc;

    KEY_PTRFUN keyEventFunc;

    mutable int m_iOldWidth;
    mutable int m_iOldHeight;
};

NS_CC_END
