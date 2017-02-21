#include "CCQGLWidget.h"
#include "CCQGLView.h"

#include <QDesktopWidget>
#include <QApplication>
#include <QLayout>

NS_CC_BEGIN

static CCQGLView* s_pMainWindow = nullptr;

static void mouseMove(QMouseEvent *event)
{
    s_pMainWindow->mouseMove(event);
}

static void mousePress(QMouseEvent *event)
{
    s_pMainWindow->mousePress(event);
}

static void mouseRelease(QMouseEvent *event)
{
    s_pMainWindow->mouseRelease(event);
}

static void wheel(QWheelEvent *event)
{
    s_pMainWindow->wheel(event);
}

CCQGLView::CCQGLView()
    : _isInit(false)
    , _window(nullptr)
    , _screenScaleFactor(1.0f)
    , _lastWindowHeight(0.0f)
    , _bgColor(0, 0, 0, 1)
{
    setViewName("cocos2d-x QT Host Win32");
}

CCQGLView::~CCQGLView()
{

}

bool CCQGLView::Create()
{
    return Create(960, 540);
}

bool CCQGLView::Create(int width, int height, QWidget *parentWidget)
{
    if (s_pMainWindow == nullptr)
    {
        CCQGLView *view = new CCQGLView();
        if (!view->Init(width, height, parentWidget))
        {
            delete view;
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}

bool CCQGLView::Init(int width, int height, QWidget *parentWidget)
{
    bool ret = false;
    do
    {
        CC_BREAK_IF(_window);

        _window = new CCQGLWidget(width, height, parentWidget);
        parentWidget->layout()->addWidget(_window);

        _window->setMouseMoveFunc(&cocos2d::mouseMove);
        _window->setMousePressFunc(&cocos2d::mousePress);
        _window->setMouseReleaseFunc(&cocos2d::mouseRelease);
        _window->setWheelFunc(&cocos2d::wheel);

        _window->setWindowFlags(_window->windowFlags() & ~Qt::WindowMaximizeButtonHint);
        _window->setBaseSize(width, height);
        _window->show();

        setDesignResolutionSize(width, height, ResolutionPolicy::EXACT_FIT);
        ret = initGlew();
        CC_BREAK_IF(!ret);

        _isInit = true;
        s_pMainWindow = this;
        ret = true;
    } while (0);

    return ret;
}

bool CCQGLView::isOpenGLReady()
{
    return _isInit;
}

void CCQGLView::end()
{
    CC_SAFE_DELETE(_window);

    s_pMainWindow = nullptr;
    delete this;
}

void CCQGLView::swapBuffers()
{
    if (_isInit)
    {
        /* Swap buffers */
        _window->makeCurrent();
        _window->swapBuffers();
    }
}

void CCQGLView::setIMEKeyboardState(bool /*bOpen*/)
{
    /* Open or close IME keyboard */
}

void CCQGLView::setViewName(const char* pszViewName)
{
    GLViewImpl::setViewName(pszViewName);
    if (_window) {
        _window->setWindowTitle(getViewName().c_str());
    }
}

void CCQGLView::setFrameSize(float width, float height)
{
    GLView::setFrameSize(width, height);
    setDesignResolutionSize(_window->baseSize().width(), _window->baseSize().height(), ResolutionPolicy::NO_BORDER);
    cocos2d::Director::getInstance()->setProjection(cocos2d::Director::getInstance()->getProjection());
    glClearColor(_bgColor.r, _bgColor.g, _bgColor.b, _bgColor.a);
}

void CCQGLView::resize()
{
    if (_window != nullptr)
    {
        float ratio = (float)(_window->baseSize().width()) / (float)(_window->baseSize().height());
        setFrameSize(_window->size().height() * ratio * _window->devicePixelRatio(), _window->size().height() * _window->devicePixelRatio());
        //        float height = std::fmin((float)_window->size().height(), (float)_window->size().width() / ratio);
        //        setFrameSize(height * ratio, height);
    }
}

void CCQGLView::updateWindowSize()
{
    if (_window != nullptr)
    {
        /* resize window if window height changed */
        if (_lastWindowHeight != _window->height())
        {
            _lastWindowHeight = _window->height();
            resize();
        }
    }
}

void CCQGLView::setBgColor(Color4B &color)
{
    _bgColor = Color4F((float)(color.r) / 255.f, (float)(color.g) / 255.f, (float)(color.b) / 255.f, (float)(color.a) / 255.f);
    glClearColor(_bgColor.r, _bgColor.g, _bgColor.b, _bgColor.a);
}

CCQGLView* CCQGLView::getInstance()
{
    return s_pMainWindow;
}

void CCQGLView::mouseMove(QMouseEvent *event)
{
    if (!_captured)
        return;

    _mouseX = (float)(event->x()) / _screenScaleFactor;
    _mouseY = (float)(event->y()) / _screenScaleFactor;

    _mouseX /= this->getFrameZoomFactor();
    _mouseY /= this->getFrameZoomFactor();

    if (_isInRetinaMonitor)
    {
        if (_retinaFactor == 1)
        {
            _mouseX *= 2;
            _mouseY *= 2;
        }
    }

    if (_captured)
    {
        intptr_t id = 0;
        this->handleTouchesMove(1, &id, &_mouseX, &_mouseY);
    }

    //Because OpenGL and cocos2d-x uses different Y axis, we need to convert the coordinate here
    float cursorX = (_mouseX - _viewPortRect.origin.x) / _scaleX;
    float cursorY = (_viewPortRect.origin.y + _viewPortRect.size.height - _mouseY) / _scaleY;

    EventMouse ccEvent(EventMouse::MouseEventType::MOUSE_MOVE);
    // Set current button
    if (event->button() == Qt::LeftButton)
    {
        ccEvent.setMouseButton(GLFW_MOUSE_BUTTON_LEFT);
    }
    else if (event->button() == Qt::RightButton)
    {
        ccEvent.setMouseButton(GLFW_MOUSE_BUTTON_RIGHT);
    }
    else if (event->button() == Qt::MiddleButton)
    {
        ccEvent.setMouseButton(GLFW_MOUSE_BUTTON_MIDDLE);
    }
    ccEvent.setCursorPosition(cursorX, cursorY);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&ccEvent);
}

void CCQGLView::mousePress(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    _captured = true;
    _mouseX = (float)(event->x()) / _screenScaleFactor;
    _mouseY = (float)(event->y()) / _screenScaleFactor;

    if (this->getViewPortRect().equals(Rect::ZERO) || this->getViewPortRect().containsPoint(Vec2(_mouseX, _mouseY)))
    {
        intptr_t id = 0;
        this->handleTouchesBegin(1, &id, &_mouseX, &_mouseY);
    }

    //Because OpenGL and cocos2d-x uses different Y axis, we need to convert the coordinate here
    float cursorX = (_mouseX - _viewPortRect.origin.x) / _scaleX;
    float cursorY = (_viewPortRect.origin.y + _viewPortRect.size.height - _mouseY) / _scaleY;

    EventMouse ccEvent(EventMouse::MouseEventType::MOUSE_DOWN);
    ccEvent.setCursorPosition(cursorX, cursorY);
    ccEvent.setMouseButton(event->button());
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&ccEvent);
}

void CCQGLView::mouseRelease(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    if (_captured)
    {
        _captured = false;
        intptr_t id = 0;
        this->handleTouchesEnd(1, &id, &_mouseX, &_mouseY);
    }

    //Because OpenGL and cocos2d-x uses different Y axis, we need to convert the coordinate here
    float cursorX = (_mouseX - _viewPortRect.origin.x) / _scaleX;
    float cursorY = (_viewPortRect.origin.y + _viewPortRect.size.height - _mouseY) / _scaleY;

    EventMouse ccEvent(EventMouse::MouseEventType::MOUSE_UP);
    ccEvent.setCursorPosition(cursorX, cursorY);
    ccEvent.setMouseButton(event->button());
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&ccEvent);
}

void CCQGLView::wheel(QWheelEvent *event)
{
    //Because OpenGL and cocos2d-x uses different Y axis, we need to convert the coordinate here
    float cursorX = (_mouseX - _viewPortRect.origin.x) / _scaleX;
    float cursorY = (_viewPortRect.origin.y + _viewPortRect.size.height - _mouseY) / _scaleY;

    EventMouse ccEvent(EventMouse::MouseEventType::MOUSE_SCROLL);
    ccEvent.setScrollData((float)event->x(), -(float)event->y());
    ccEvent.setCursorPosition(cursorX, cursorY);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&ccEvent);
}

QWidget *CCQGLView::getGLWidget()
{
    return _window;
}

NS_CC_END
