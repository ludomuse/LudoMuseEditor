#-------------------------------------------------
#
# Project created by QtCreator 2016-06-02T10:47:42
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LudoMuseEditor
TEMPLATE = app

CONFIG += c++14


SOURCES += main.cpp\
        CMainWindow.cpp\
    LudoMuse_src/Classes/Engine/Source/CCallback.cpp \
    LudoMuse_src/Classes/Engine/Source/CCameraFeedNode.cpp \
    LudoMuse_src/Classes/Engine/Source/CDispatchMessageVisitor.cpp \
    LudoMuse_src/Classes/Engine/Source/CEditBoxNode.cpp \
    LudoMuse_src/Classes/Engine/Source/CEntityNode.cpp \
    LudoMuse_src/Classes/Engine/Source/CFindEntityFromIDVisitor.cpp \
    LudoMuse_src/Classes/Engine/Source/CFindEntityTouchVisitor.cpp \
    LudoMuse_src/Classes/Engine/Source/CFindEntityVisitor.cpp \
    LudoMuse_src/Classes/Engine/Source/CGotoSceneVisitor.cpp \
    LudoMuse_src/Classes/Engine/Source/CGridNode.cpp \
    LudoMuse_src/Classes/Engine/Source/CGroupNode.cpp \
    LudoMuse_src/Classes/Engine/Source/CInputManager.cpp \
    LudoMuse_src/Classes/Engine/Source/CJsonParser.cpp \
    LudoMuse_src/Classes/Engine/Source/CKernel.cpp \
    LudoMuse_src/Classes/Engine/Source/CLabelNode.cpp \
    LudoMuse_src/Classes/Engine/Source/CMenuNode.cpp \
    LudoMuse_src/Classes/Engine/Source/CNode.cpp \
    LudoMuse_src/Classes/Engine/Source/CParallelNode.cpp \
    LudoMuse_src/Classes/Engine/Source/CPeerNode.cpp \
    LudoMuse_src/Classes/Engine/Source/CSceneNode.cpp \
    LudoMuse_src/Classes/Engine/Source/CSelectorNode.cpp \
    LudoMuse_src/Classes/Engine/Source/CSequenceNode.cpp \
    LudoMuse_src/Classes/Engine/Source/CSoundManager.cpp \
    LudoMuse_src/Classes/Engine/Source/CSpriteNode.cpp \
    LudoMuse_src/Classes/Engine/Source/CTouchBeganVisitor.cpp \
    LudoMuse_src/Classes/Engine/Source/CTransitionVisitor.cpp \
    LudoMuse_src/Classes/Engine/Source/CValidateSceneVisitor.cpp \
    LudoMuse_src/Classes/Engine/Source/CValidator.cpp \
    LudoMuse_src/Classes/Engine/Source/CVisitor.cpp \
    LudoMuse_src/Classes/Modules/Util/Source/CStats.cpp \
    LudoMuse_src/Classes/Modules/Util/Source/Util.cpp \
    LudoMuse_src/Classes/HelloWorldScene.cpp \
    LudoMuse_src/Classes/Modules/Networking/linux/Source/CNetworkManager.cpp \
    CThreadCocos.cpp \
    LudoMuse_src/Classes/AppDelegate.cpp \
    CThumbnailWidget.cpp \
    LudoMuse_src/Classes/Engine/Source/CEditorFindEntityTouchVisitor.cpp \
    CLabelInspector.cpp \
    CSpriteInspector.cpp


HEADERS  += CMainWindow.h \
    LudoMuse_src/Classes/Engine/Include/CCallback.h \
    LudoMuse_src/Classes/Engine/Include/CCameraFeedNode.h \
    LudoMuse_src/Classes/Engine/Include/CDispatchMessageVisitor.h \
    LudoMuse_src/Classes/Engine/Include/CEditBoxNode.h \
    LudoMuse_src/Classes/Engine/Include/CEntityNode.h \
    LudoMuse_src/Classes/Engine/Include/CFindEntityFromIDVisitor.h \
    LudoMuse_src/Classes/Engine/Include/CFindEntityFromTypeVisitor.h \
    LudoMuse_src/Classes/Engine/Include/CFindEntityTouchVisitor.h \
    LudoMuse_src/Classes/Engine/Include/CFindEntityVisitor.h \
    LudoMuse_src/Classes/Engine/Include/CGotoSceneVisitor.h \
    LudoMuse_src/Classes/Engine/Include/CGridNode.h \
    LudoMuse_src/Classes/Engine/Include/CGroupNode.h \
    LudoMuse_src/Classes/Engine/Include/CInputManager.h \
    LudoMuse_src/Classes/Engine/Include/CJsonParser.h \
    LudoMuse_src/Classes/Engine/Include/CJsonParser_impl.h \
    LudoMuse_src/Classes/Engine/Include/CKernel.h \
    LudoMuse_src/Classes/Engine/Include/CLabelNode.h \
    LudoMuse_src/Classes/Engine/Include/CMenuNode.h \
    LudoMuse_src/Classes/Engine/Include/CNode.h \
    LudoMuse_src/Classes/Engine/Include/CParallelNode.h \
    LudoMuse_src/Classes/Engine/Include/CPeerNode.h \
    LudoMuse_src/Classes/Engine/Include/CSceneNode.h \
    LudoMuse_src/Classes/Engine/Include/CSelectorNode.h \
    LudoMuse_src/Classes/Engine/Include/CSequenceNode.h \
    LudoMuse_src/Classes/Engine/Include/CSoundManager.h \
    LudoMuse_src/Classes/Engine/Include/CSpriteNode.h \
    LudoMuse_src/Classes/Engine/Include/CTouchBeganVisitor.h \
    LudoMuse_src/Classes/Engine/Include/CTransitionVisitor.h \
    LudoMuse_src/Classes/Engine/Include/CValidateSceneVisitor.h \
    LudoMuse_src/Classes/Engine/Include/CValidator.h \
    LudoMuse_src/Classes/Engine/Include/CVisitor.h \
    LudoMuse_src/Classes/Engine/Include/SUser.h \
    LudoMuse_src/Classes/Modules/Util/Include/CStats.h \
    LudoMuse_src/Classes/Modules/Util/Include/Desc.h \
    LudoMuse_src/Classes/Modules/Util/Include/Util.h \
    LudoMuse_src/Classes/AppDelegate.h \
    LudoMuse_src/Classes/HelloWorldScene.h \
    LudoMuse_src/Classes/Modules/Networking/Networking.h \
    LudoMuse_src/Classes/Modules/Networking/android/Include/LmBytesMessage.h \
    CThreadCocos.h \
    CThumbnailWidget.h \
    LudoMuse_src/Classes/Engine/Include/CEditorFindEntityTouchVisitor.h \
    CLabelInspector.h \
    CSpriteInspector.h

FORMS    += cmainwindow.ui

INCLUDEPATH += "./LudoMuse_src" \
                "./lib" \
                "D:/IHMTEK/cocos_project/LudoMuse/cocos2d/cocos" \
                "./lib/cocos/audio/include" \
                "./lib/external/win32-specific/gles/include/OGLES" \
                "./lib/external/win32-specific/angle/include/EGL" \
                "./lib/external/win32-specific/angle/include/GLES2" \
                "./lib/external/win32-specific/angle/include/GLES3" \
                "./lib/external/win32-specific/angle/include/KHR" \
                "./lib/external/glfw3/include/win32" \
                ./lib/external/json \
                "C:/Program Files/Java/jdk1.8.0/include" \
                "C:/Program Files/Java/jdk1.8.0/include/win32" \
                /usr/include/GLFW

win32 {

SOURCES +=     LudoMuse_src/Classes/Modules/Networking/win/Source/CNetworkManager.cpp \


HEADERS += -    LudoMuse_src/Classes/Modules/Networking/win/Include/CNetworkManager.h \
-    LudoMuse_src/Classes/Modules/Networking/INetworkManager.h \


LIBS += -LD:/IHMTEK/LudoMuseEditorCocos/LudoMuseEditor/lib/external/png/prebuilt/win32 -llibpng-2015
LIBS += -LD:/IHMTEK/LudoMuseEditorCocos/LudoMuseEditor/lib/external/jpeg/prebuilt/win32 -llibjpeg-2015
LIBS += -LD:/IHMTEK/LudoMuseEditorCocos/LudoMuseEditor/lib/external/tiff/prebuilt/win32 -llibtiff-2015
LIBS += -LD:/IHMTEK/LudoMuseEditorCocos/LudoMuseEditor/lib/external/glfw3/prebuilt/win32 -lglfw3dll
LIBS += -LD:/IHMTEK/LudoMuseEditorCocos/LudoMuseEditor/lib/external/chipmunk/prebuilt/win32/debug-lib -llibchipmunk-2015
LIBS += -LD:/IHMTEK/LudoMuseEditorCocos/LudoMuseEditor/lib/cocos/ -llibcocos2d
LIBS += -LD:/IHMTEK/LudoMuseEditorCocos/LudoMuseEditor/lib/ -lpthreadVC2
LIBS += -LD:/IHMTEK/LudoMuseEditorCocos/LudoMuseEditor/lib/ -lOpenGL32
LIBS += -LD:/IHMTEK/LudoMuseEditorCocos/LudoMuseEditor/lib/external/win10-specific/angle/prebuilt/win32/ -llibEGL
LIBS += -LD:/IHMTEK/LudoMuseEditorCocos/LudoMuseEditor/lib/external/win10-specific/angle/prebuilt/win32/ -llibGLESv2

LIBS += -LD:/IHMTEK/LudoMuseEditorCocos/LudoMuseEditor/lib/lib -lfreetype \
         -lglew32 \
         -llibbox2d \
         -llibbullet \
         -llibcurl_imp \
         -llibeay32 \
         -llibiconv \
         -llibmpg123 \
         -llibogg \
         -llibrecast \
         -llibSpine \
         -llibvorbis \
         -llibvorbisfile \
         -llibwebp \
         -llibzlib \
         -lOpenAL32 \
         -lsqlite3 \
         -lssleay32 \
         -lwebsockets \
         -lGdi32 \
         -lUser32 \
         -lWs2_32 \
}


DEPENDPATH += D:/IHMTEK/LudoMuseEditorCocos/LudoMuseEditor/lib/
DEPENDPATH += D:/IHMTEK/LudoMuseEditorCocos/LudoMuseEditor/lib/lib
DEPENDPATH += D:/IHMTEK/cocos_project/LudoMuse/cocos2d/cocos
DEPENDPATH += D:/IHMTEK/LudoMuseEditorCocos/LudoMuseEditor/lib/external/png/prebuilt/win32
DEPENDPATH += D:/IHMTEK/LudoMuseEditorCocos/LudoMuseEditor/lib/external/jpeg/prebuilt/win32
DEPENDPATH += D:/IHMTEK/LudoMuseEditorCocos/LudoMuseEditor/lib/external/tiff/prebuilt/win32
DEPENDPATH += D:/IHMTEK/LudoMuseEditorCocos/LudoMuseEditor/lib/external/glfw3/prebuilt/win32
DEPENDPATH += D:/IHMTEK/LudoMuseEditorCocos/LudoMuseEditor/lib/external/chipmunk/prebuilt/win32/debug-lib

!win32 {
LIBS += -L/home/babouchot/workspace/IHMTEK/LudoMuseEditor/lib/ -lcocos2d -lcocos2dInternal -lX11 -lgobject-2.0 -lglib-2.0 -lfreetype -lGLEW -lfmod -ltinyxml2 -lbullet -lrecast -lflatbuffers -lxxhash -lunzip -lgtk-3 -lfontconfig -lchipmunk -lwebp -lz -lpng -ljpeg -ltiff -lglfw -lpthread -lEGL
}
