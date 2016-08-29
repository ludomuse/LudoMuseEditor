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

# LUDOMUSE_PATH = "C:\WORKSPACE\LudoMuse"
LUDOMUSE_PATH = "/home/babouchot/workspace/IHMTEK/LudoMuse/"
COCOS_PATH = $${LUDOMUSE_PATH}/cocos2d
JAVA_PATH = "C:\Program Files\Java\jdk1.8.0_66"

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
    LudoMuse_src/Classes/Engine/Source/CInfoNode.cpp \
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
    CThreadCocos.cpp \
    LudoMuse_src/Classes/AppDelegate.cpp \
    CThumbnailWidget.cpp \
    LudoMuse_src/Classes/Engine/Source/CEditorFindEntityTouchVisitor.cpp \
    CLabelInspector.cpp \
    CSpriteInspector.cpp \
    CAddSceneWizard.cpp \
    CFileListView.cpp \
    CLineEdit.cpp \
    CSceneInspector.cpp \
    CTemplate.cpp \
    CTemplateManager.cpp \
    CTemplatePushButton.cpp \
    CLoaderWidget.cpp \
    CNewProjectWizard.cpp \
    CProjectManager.cpp \
    UtilsQt.cpp \
    CLoadProjectWizard.cpp \
    CProjectPushButton.cpp \
    CEditorKernel.cpp \
    CMenuNodeInspector.cpp


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
    LudoMuse_src/Classes/Engine/Include/CInfoNode.h \
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
    CSpriteInspector.h \
    CAddSceneWizard.h \
    CFileListView.h \
    CLineEdit.h \
    CSceneInspector.h \
    CTemplate.h \
    CTemplateManager.h \
    CTemplatePushButton.h \
    CLoaderWidget.h \
    CNewProjectWizard.h \
    CProjectManager.h \
    UtilsQt.h \
    CLoadProjectWizard.h \
    CProjectPushButton.h \
    CEditorKernel.h \
    CMenuNodeInspector.h

FORMS    += cmainwindow.ui \
    CLoaderWidget.ui \
    CNewProjectWizard.ui \
    CLoadProjectWizard.ui \
    CMenuNodeInspector.ui

INCLUDEPATH += "./LudoMuse_src" \
                $${COCOS_PATH}/cocos \
                $${COCOS_PATH}/cocos/audio/include \
                $${COCOS_PATH}/external/json \
                ./lib/external/json \
                /usr/include/GLFW


linux {

INCLUDEPATH +=  /usr/include/GLFW

}

win32 {

SOURCES +=     LudoMuse_src/Classes/Modules/Networking/win/Source/CNetworkManager.cpp \


HEADERS += LudoMuse_src/Classes/Modules/Networking/win/Include/CNetworkManager.h \
           LudoMuse_src/Classes/Modules/Networking/INetworkManager.h \

INCLUDEPATH +=  $${JAVA_PATH}/include \
                $${JAVA_PATH}/include/win32 \
                $${LUDOMUSE_PATH}/proj.win32/lib \
                $${COCOS_PATH}/external/win32-specific/gles/include/OGLES \
                $${COCOS_PATH}/external/win32-specific/angle/include/EGL \
                $${COCOS_PATH}/external/win32-specific/angle/include/GLES2 \
                $${COCOS_PATH}/external/win32-specific/angle/include/GLES3 \
                $${COCOS_PATH}/external/win32-specific/angle/include/KHR \
                $${COCOS_PATH}/external/glfw3/include/win32 \


LIBS += -L$${COCOS_PATH}/external/png/prebuilt/win32 -llibpng-2015
LIBS += -L$${COCOS_PATH}/external/jpeg/prebuilt/win32 -llibjpeg-2015
LIBS += -L$${COCOS_PATH}/external/tiff/prebuilt/win32 -llibtiff-2015
LIBS += -L$${COCOS_PATH}/external/glfw3/prebuilt/win32 -lglfw3dll
LIBS += -L$${COCOS_PATH}/external/chipmunk/prebuilt/win32/debug-lib -llibchipmunk-2015
LIBS += -L$${LUDOMUSE_PATH}/proj.win32/Debug.win32 -llibcocos2d  -lpthreadVC2
LIBS += -lOpenGL32
LIBS += -L$${COCOS_PATH}/external/win10-specific/angle/prebuilt/win32/ -llibEGL
LIBS += -L$${COCOS_PATH}/external/win10-specific/angle/prebuilt/win32/ -llibGLESv2
LIBS += -L$${PWD}/lib

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

linux {

SOURCES +=     LudoMuse_src/Classes/Modules/Networking/linux/Source/CNetworkManager.cpp \

HEADERS +=     LudoMuse_src/Classes/Modules/Networking/linux/Include/CNetworkManager.h \


LIBS += -L/home/babouchot/workspace/IHMTEK/LudoMuseEditor/lib/ -lcocos2d -lcocos2dInternal -lX11 -lgobject-2.0 -lglib-2.0 -lfreetype -lGLEW -lfmod -ltinyxml2 -lbullet -lrecast -lflatbuffers -lxxhash -lunzip -lgtk-3 -lfontconfig -lchipmunk -lwebp -lz -lpng -ljpeg -ltiff -lglfw -lpthread -lEGL
}
