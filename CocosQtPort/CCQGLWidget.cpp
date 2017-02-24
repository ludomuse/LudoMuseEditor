#include "CCQGLWidget.h"
#include "CCQGLView.h"

NS_CC_BEGIN

CCQGLWidget::CCQGLWidget(int width, int height, QWidget *parent)
    : QGLWidget(QGLFormat(QGL::DoubleBuffer), parent)
    , mouseMoveFunc(nullptr)
    , mousePressFunc(nullptr)
    , mouseReleaseFunc(nullptr)
    , wheelFunc(nullptr)
    , keyEventFunc(nullptr)
{
    resize(width, height);
    m_iOldHeight = height;
    m_iOldWidth = width;

    QGLFormat format;
    format.setVersion(2,0);
    format.setDoubleBuffer(true);
    setFormat(format);

}

CCQGLWidget::~CCQGLWidget()
{
}

void CCQGLWidget::setMouseMoveFunc(MOUSE_PTRFUN func)
{
    mouseMoveFunc = func;
}

void CCQGLWidget::setMousePressFunc(MOUSE_PTRFUN func)
{
    mousePressFunc = func;
}

void CCQGLWidget::setMouseReleaseFunc(MOUSE_PTRFUN func)
{
    mouseReleaseFunc = func;
}

void CCQGLWidget::setWheelFunc(WHEEL_PTRFUN func)
{
    wheelFunc = func;
}

void CCQGLWidget::setKeyEventFunc(KEY_PTRFUN func)
{
    keyEventFunc = func;
}

void CCQGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (mouseMoveFunc)
        mouseMoveFunc(event);

    QGLWidget::mouseMoveEvent(event);
}

void CCQGLWidget::mousePressEvent(QMouseEvent *event)
{
    if (mousePressFunc)
        mousePressFunc(event);

    QGLWidget::mousePressEvent(event);
}

void CCQGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (mouseReleaseFunc)
        mouseReleaseFunc(event);

    QGLWidget::mouseReleaseEvent(event);
}

void CCQGLWidget::wheelEvent(QWheelEvent *event)
{
    if (wheelFunc)
        wheelFunc(event);

    QGLWidget::wheelEvent(event);
}

void CCQGLWidget::keyPressEvent(QKeyEvent *event)
{
    if (keyEventFunc)
        keyEventFunc(event);

    QGLWidget::keyPressEvent(event);
}

void CCQGLWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (keyEventFunc)
        keyEventFunc(event);

    QGLWidget::keyReleaseEvent(event);
}

void CCQGLWidget::resizeEvent(QResizeEvent *event)
{
    QGLWidget::resizeEvent(event);
    int width = event->size().width();
    int height = event->size().height();
    if (parentWidget())
    {
        width = std::min(width, parentWidget()->size().width());
        height = std::min(height, parentWidget()->size().height());
    }
    if (m_iOldHeight != height ||
            m_iOldWidth != width)
    {
        float ratio = (float)baseSize().width() / (float)baseSize().height();
        if ((float)width / (float)height > ratio)
        {
            m_iOldHeight = height;
            m_iOldWidth = (int)(height * ratio);
        }
        else
        {
            m_iOldHeight = (int)(width / ratio);
            m_iOldWidth = width;
        }
        if (parentWidget())
        {
            setGeometry((parentWidget()->size().width() - m_iOldWidth) * parentWidget()->devicePixelRatio() /2,
                        (parentWidget()->size().height() - m_iOldHeight) * parentWidget()->devicePixelRatio() /2,
                        m_iOldWidth * parentWidget()->devicePixelRatio() , m_iOldHeight * parentWidget()->devicePixelRatio() );
        }
        else
        {
            setGeometry(0, 0, m_iOldWidth, m_iOldHeight);
        }
    }
}

QSize CCQGLWidget::sizeHint() const
{
    return baseSize();
}

NS_CC_END
