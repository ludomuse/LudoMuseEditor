#ifdef _WIN32
#include <WinSock2.h>
#endif

// Include QT
//#include <QtWidgets>
#include <QThread>
#include <QProcess>
#include <QBoxLayout>
#include <QLayoutItem>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QDirModel>
#include <QMouseEvent>
#include <QString>
#include <QDebug>
#include <QFileDialog>

//#include <GL/glew.h>
#include <vector>
#include <string>

#ifdef __linux__
//linux code goes here
#elif _WIN32
#include <Windows.h>
// windows code goes here
#endif

#include "LudoMuse_src/Classes/AppDelegate.h"
#include "CocosQtPort/CCQApplication.h"
#include "CocosQtPort/CCQGLView.h"
#include "CocosQtPort/CCQGLWidget.h"

// Include custom classes
#include "CMainWindow.h"
#include "ui_cmainwindow.h"
#include "CEditorKernel.h"
#include "CThreadCocos.h"
#include "CThumbnailWidget.h"
#include "CLabelInspector.h"
#include "CSpriteInspector.h"
#include "CSceneInspector.h"
#include "CMenuNodeInspector.h"
#include "CAddSceneWizard.h"
#include "CLoaderWidget.h"
#include "CProjectManager.h"
#include "CThumbnailsLoaderThread.h"


// Include cocos
#include "cocos2d.h"

// Include LudoMuse
#include "Classes/Engine/Include/CNode.h"
#include "Classes/Engine/Include/CSceneNode.h"
#include "Classes/Engine/Include/CSequenceNode.h"
#include "Classes/Engine/Include/CEntityNode.h"
#include "Classes/Engine/Include/CSpriteNode.h"
#include "Classes/Engine/Include/CMenuNode.h"
#include "Classes/Engine/Include/CCallback.h"
#include "Classes/Engine/Include/CEditorFindEntityTouchVisitor.h"
#include "Classes/Modules/Util/Include/Util.h"



CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CMainWindow),
    // REPLACE
    //    m_pCurrentThumbnailWidget1(Q_NULLPTR),
    //    m_pCurrentThumbnailWidget2(Q_NULLPTR),
    // BY
    m_iCurrentThumbnailIndex1(-1),
    m_iCurrentThumbnailIndex2(-1),
    m_pThumbnailList1(new QList<CThumbnailWidget*>()),
    m_pThumbnailList2(new QList<CThumbnailWidget*>()),
    m_iActivePlayer(BOTH_PLAYER),
    m_pKernel(NULL),
    m_sSaveName(""),

    m_pLoader(new CThumbnailsLoaderThread()),

    m_oArchiver(this)
{
    ui->setupUi(this);

    // Init with loader widget
    ui->mmBotView->setVisible(false);
    ui->toolBarCocos->setVisible(false);
    ui->sceneInspectorContainer->setVisible(false);
    ui->toolBarInspector->setVisible(false);
    ui->inspectorContainer->setVisible(false);
    CLoaderWidget* loaderWidget = new CLoaderWidget();
    ui->glViewContainer->layout()->addWidget(loaderWidget);
    connect(loaderWidget, SIGNAL(closeEditor()), this, SLOT(close()));
    connect(loaderWidget, SIGNAL(loadProject(const QString&)), this, SLOT(loadExistingProject(const QString&)));

    //    ui->sceneInspectorContainer->setStyleSheet("#sceneInspectorContainer{background-color : rgb(50, 50, 50);border-right : 1px solid white                                           }");

    // disable save button
    if(m_sSaveName.isEmpty())
    {
        ui->save->setEnabled(false);
    }

    // Icon creation
    ui->emulateButton->setIcon(QIcon(CProjectManager::Instance()->QGetInstallPath().append("/resources/play_arrow.png")));
    ui->archiveButton->setIcon(QIcon(CProjectManager::Instance()->QGetInstallPath().append("/resources/add_playlist_w.png")));

    // Create Template Manager
    this->m_pTemplatesManager = CTemplateManager::Instance();

    // Connect Tool bar
    connect(ui->prevScreenButton, SIGNAL(clicked(bool)), this, SLOT(goToPreviousScene()));
    connect(ui->nextScreenButton, SIGNAL(clicked(bool)), this, SLOT(goToNextScene()));
    connect(ui->emulateButton, SIGNAL(clicked(bool)), this, SLOT(launchEmulator()));
    connect(ui->JsonGo, SIGNAL(clicked(bool)), this, SLOT(saveAs()));
    connect(ui->save, SIGNAL(clicked(bool)), this, SLOT(save()));
    //connect(ui->lmwTestButton, SIGNAL(clicked(bool)), this, SLOT(launchAddSceneWizard()));
    connect(ui->archiveButton, SIGNAL(clicked(bool)), this, SLOT(exportProject));

    // Connect kernel signal
    connect(CEditorKernel::Instance(), SIGNAL(sendMenuNodeSignal(LM::CMenuNode*)), this, SLOT(receiveMenu(LM::CMenuNode*)));

    this->showMaximized();
}

CMainWindow::~CMainWindow()
{
    delete ui;
}



void CMainWindow::loadExistingProject(const QString& a_sProjectFile)
{
    // Update CprojetManager
    CProjectManager::Instance()->SetProjectFile(a_sProjectFile);
    m_sSaveName = a_sProjectFile;

    ui->save->setEnabled(true);
    // Clear loader widget!
    QLayout* glViewContainerLayout = ui->glViewContainer->layout();
    QLayoutItem *child;
    while ((child = glViewContainerLayout->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }

    /*    AppDelegate app (true, a_sProjectFile.toStdString());
    QThread* thread = new QThread;
    CThreadCocos* worker = new CThreadCocos(a_sProjectFile);
    worker->moveToThread(thread);
    //connect(worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(worker, SIGNAL(sendHWND(int)), this, SLOT(receiveHWND(int)));
    connect(worker, SIGNAL(sendKernel(LM::CKernel*)), this, SLOT(receiveKernel(LM::CKernel*)));
    connect(thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();*/

    ui->mmBotView->setVisible(true);
    ui->inspectorContainer->setVisible(true);
    ui->sceneInspectorContainer->setVisible(true);
    //    ui->sceneInspectorContainer->setStyleSheet("#sceneInspectorContainer{background-color : rgb(50, 50, 50);border-right :none}");
    ui->toolBarCocos->setVisible(true);
    ui->toolBarInspector->setVisible(true);
    //    ui->toolBarCocos->setStyleSheet("#toolBarCocos{border-bottom: 1px solid black;border-right : 2px solid rgba(255,255,255,255);}");

    QString projectPath = QFileInfo(a_sProjectFile).absolutePath();

    m_pDirModel = new QFileSystemModel();
    m_pDirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    m_pDirModel->setRootPath(projectPath);
    ui->fileBrowser->setModel(m_pDirModel);
    ui->fileBrowser->setRootIndex(m_pDirModel->index(projectPath));
    ui->fileBrowser->setColumnHidden( 1, true );
    ui->fileBrowser->setColumnHidden( 2, true );
    ui->fileBrowser->setColumnHidden( 3, true );
    m_pFileModel = new QFileSystemModel(this);
    m_pFileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    ui->fileListView->setModel(m_pFileModel);
    ui->fileListView->setRootIndex(m_pFileModel->setRootPath(projectPath));
    ui->fileListView->SetCurrentPath(projectPath);

    AppDelegate* app = (AppDelegate*)(cocos2d::CCQApplication::getInstance());
    app->setPath(m_sSaveName.toStdString());
    app->setParentWidget(ui->glViewContainer);
    cocos2d::CCQApplication::getInstance()->run();
    m_pKernel = app->getKernel();
    LM::CEditorFindEntityTouchVisitor* dummyVisitor= this->m_pKernel->GetEditorVisitor();
    connect(dummyVisitor, SIGNAL(labelClicked(LM::CLabelNode*)), this, SLOT(receiveLabel(LM::CLabelNode*)));
    connect(dummyVisitor, SIGNAL(spriteClicked(LM::CSpriteNode*)), this, SLOT(receiveSprite(LM::CSpriteNode*)));
    //    connect(m_pKernel, SIGNAL(addingSceneFinished(std::string, int)), this, SLOT(addingSceneFinished(std::string, int)));
    //    connect(m_pKernel, SIGNAL(deletingSceneFinished()), this, SLOT(deletingSceneFinished()));
    connect(m_pKernel, SIGNAL(addingSceneFinished(const QString, const QString, int)),
            this, SLOT(addingSceneFinished(const QString, const QString, int)));
    connect(m_pKernel, SIGNAL(deletingSceneFinished(const QString, int)),
            this, SLOT(deletingSceneFinished(const QString, int)));
    connect(m_pKernel, SIGNAL(sendScene(LM::CSceneNode*, bool)),
            this, SLOT(receiveScene(LM::CSceneNode*, bool)));
    connect(m_pKernel, SIGNAL(captureFinished(QString)),
            this, SLOT(loadCapture(QString)));

    this->ProcessTree();
    this->InspectScene(m_pKernel->m_pCurrentScene);
    m_iActivePlayer = m_pKernel->GetActivePlayer();


    //    connect(pLoader, SIGNAL(finished()), pLoader, SLOT(deleteLater()));
    //    connect(this, SIGNAL(loadThumbnail(CThumbnailWidget*)),
    //            pLoader, SLOT(loadThumbnail(CThumbnailWidget*)));
    //    connect(this, SIGNAL(finishLoading()),
    //            pLoader, SLOT(terminate()));

    m_iCurrentThumbnailIndex1 = -1;
    m_iCurrentThumbnailIndex2 = -1;
    if (!m_pThumbnailList1->empty()){
        m_iCurrentThumbnailIndex1 = 0;
    }
    if (!m_pThumbnailList2->empty())
    {
        m_iCurrentThumbnailIndex2 = 0;
    }
    this->ActivateThumbnails();
    //    QString currentScene(m_pKernel->m_pCurrentScene->GetSceneID().c_str());
    //    m_iActivePlayer = m_pKernel->GetCurrentPlayer();
    //    this->activeThumbnail(currentScene, m_iActivePlayer);

    //    QWidget* ccWidget = cocos2d::CCQGLView::getInstance()->getGLWidget();
    //    ui->glViewContainer->layout()->addWidget(ccWidget);

    connect(m_pLoader, SIGNAL(finished()), m_pLoader, SLOT(deleteLater()));
    m_pLoader->start();
}


void CMainWindow::receiveHWND(int number)
{
    //    WId player1ID = (WId) number;
    //    QWindow *container = QWindow::fromWinId(player1ID);
    //    QWidget *cocosWidget= QWidget::createWindowContainer(container);
    //    cocosWidget->setParent(this);
    //    this->ui->glViewContainer->layout()->addWidget(cocosWidget);
    //    this->update();

}

void CMainWindow::receiveKernel(LM::CKernel *aKernel)
{
    this->m_pKernel = aKernel;

    LM::CEditorFindEntityTouchVisitor* dummyVisitor= this->m_pKernel->GetEditorVisitor();
    connect(dummyVisitor, SIGNAL(labelClicked(LM::CLabelNode*)), this, SLOT(receiveLabel(LM::CLabelNode*)));
    connect(dummyVisitor, SIGNAL(spriteClicked(LM::CSpriteNode*)), this, SLOT(receiveSprite(LM::CSpriteNode*)));
    connect(m_pKernel, SIGNAL(addingSceneFinished()), this, SLOT(addingSceneFinished()));
    connect(m_pKernel, SIGNAL(deletingSceneFinished()), this, SLOT(deletingSceneFinished()));
    connect(m_pKernel, SIGNAL(sendScene(LM::CSceneNode*, bool)), this, SLOT(receiveScene(LM::CSceneNode*, bool)));
    this->ProcessTree();


    /*
    CFrameWidget *pFrameWidget = new CFrameWidget();
    ui->glViewContainer->layout()->addWidget(pFrameWidget);
    connect(pFrameWidget, SIGNAL(mousePressed(QMouseEvent*)), m_pKernel, SLOT(onMousePressed(QMouseEvent*)));
    connect(pFrameWidget, SIGNAL(mouseReleased(QMouseEvent*)), m_pKernel, SLOT(onMouseReleased(QMouseEvent*)));
    connect(pFrameWidget, SIGNAL(mouseMoved(QMouseEvent*)), m_pKernel, SLOT(onMouseMoved(QMouseEvent*)));
    pFrameWidget->start();*/
}

void CMainWindow::receiveLabel(LM::CLabelNode* a_pLabel)
{
    //qDebug("Reception d'un Label");
    this->InspectLabel(a_pLabel);
}

void CMainWindow::receiveSprite(LM::CSpriteNode* a_pSprite)
{
    this->InspectSprite(a_pSprite);
}

void CMainWindow::receiveMenu(LM::CMenuNode* a_pMenuNode)
{
    qDebug("received CMenuNode");
    this->InspectMenuNode(a_pMenuNode);
}

void CMainWindow::receiveScene(LM::CSceneNode *a_pScene, bool a_bIsNav)
{
    this->InspectScene(a_pScene);
    /*    if(a_bIsNav)
    {
        qDebug("navigation transition -> changing active thumbnail");
        this->activeThumbnail(QString(a_pScene->GetSceneID().c_str()), m_iActivePlayer);
    }*/
}

void CMainWindow::clearInspectorContainer()
{
    QLayout* inspectorContainerLayout = this->ui->inspectorContainer->layout();
    QLayoutItem *child;
    while ((child = inspectorContainerLayout->takeAt(0)) != 0) {
        child->widget()->close();
        delete child->widget();
        delete child;
    }
    this->setInspectorName("");
}
/*
void CMainWindow::goToSceneID(CThumbnailWidget* a_pClickedWidget)
{
    QString sSceneID = a_pClickedWidget->GetSceneID();
    int iPlayerID = a_pClickedWidget->GetPlayerID();

    qDebug()<<"Deplacement vers la scene d'id : "<< sSceneID;

    if(iPlayerID == PLAYER_2)
    {
        if(m_pCurrentThumbnailWidget2 != Q_NULLPTR)
        {
            m_pCurrentThumbnailWidget2->Unselect();
        }
        m_pCurrentThumbnailWidget2 = a_pClickedWidget;
        // Change color for other player timeline
        if(m_pCurrentThumbnailWidget1 != Q_NULLPTR)
        {
            m_pCurrentThumbnailWidget1->LastActive();
        }
        m_iActivePlayer = PLAYER_2;
        m_pCurrentThumbnailWidget2->Select();
    }
    else // By default take player one
    {
        if(m_pCurrentThumbnailWidget1 != Q_NULLPTR)
        {
            m_pCurrentThumbnailWidget1->Unselect();
        }
        m_pCurrentThumbnailWidget1 = a_pClickedWidget;
        // Change color for other player timeline
        if(m_pCurrentThumbnailWidget2 != Q_NULLPTR)
        {
            m_pCurrentThumbnailWidget2->LastActive();
        }
        m_iActivePlayer = PLAYER_1;
        m_pCurrentThumbnailWidget1->Select();
    }

    LM::SEvent dummyEvent(LM::SEvent::NONE, nullptr, sSceneID.toStdString(), true, iPlayerID);
    //    ON_CC_THREAD(LM::CKernel::GotoScreenID, this->m_pKernel, dummyEvent, nullptr);
    m_pKernel->GotoScreenID(dummyEvent, nullptr);
    //void GotoScreenID(SEvent a_rEvent, CEntityNode* a_pTarget);
    this->clearInspectorContainer();
}

void CMainWindow::goToNextScene()
{
    CThumbnailWidget *pNextWidget;
    if (m_iActivePlayer == PLAYER_2)
    {
        pNextWidget = FindThumbnailWidget(m_pCurrentThumbnailWidget2->GetSceneID(), m_iActivePlayer, +1);
    }
    else
    {
        pNextWidget = FindThumbnailWidget(m_pCurrentThumbnailWidget1->GetSceneID(), m_iActivePlayer, +1);
    }
    if (pNextWidget != nullptr)
    {
        this->goToSceneID(pNextWidget);
//            ON_CC_THREAD(LM::CKernel::NavNext, this->m_pKernel, nullptr, nullptr);
        this->clearInspectorContainer();
        this->setInspectorName("");
    }
}

void CMainWindow::goToPreviousScene()
{
    CThumbnailWidget *pPrevWidget;
    if (m_iActivePlayer == PLAYER_2)
    {
        pPrevWidget = FindThumbnailWidget(m_pCurrentThumbnailWidget2->GetSceneID(), m_iActivePlayer, -1);
    }
    else
    {
        pPrevWidget = FindThumbnailWidget(m_pCurrentThumbnailWidget1->GetSceneID(), m_iActivePlayer, -1);
    }
    if (pPrevWidget != nullptr)
    {
        this->goToSceneID(pPrevWidget);
        //    ON_CC_THREAD(LM::CKernel::NavPrevious, this->m_pKernel, nullptr, nullptr);
        this->clearInspectorContainer();
        this->setInspectorName("");
    }
}
*/

void CMainWindow::goToSceneID(CThumbnailWidget* a_pClickedWidget)
{
    SaveThumbnail();
    DeactivateThumbnails();
    m_iActivePlayer = a_pClickedWidget->GetPlayerID();
    SetCurrentThumbnailIndex(m_iActivePlayer, GetThumbnailList(m_iActivePlayer)->indexOf(a_pClickedWidget));
    ActivateThumbnails();
    ShowCurrentScene();
}

void CMainWindow::goToNextScene()
{
    SaveThumbnail();
    DeactivateThumbnails();
    SetCurrentThumbnailIndex(m_iActivePlayer, GetCurrentThumbnailIndex(m_iActivePlayer)+1);
    ActivateThumbnails();
    ShowCurrentScene();
}

void CMainWindow::goToPreviousScene()
{
    SaveThumbnail();
    DeactivateThumbnails();
    SetCurrentThumbnailIndex(m_iActivePlayer, GetCurrentThumbnailIndex(m_iActivePlayer)-1);
    ActivateThumbnails();
    ShowCurrentScene();
}

void CMainWindow::ShowCurrentScene()
{
    if (GetThumbnailList(m_iActivePlayer)->count() > GetCurrentThumbnailIndex(m_iActivePlayer) &&
            GetCurrentThumbnailIndex(m_iActivePlayer) >= 0)
    {
        CThumbnailWidget* pCurrentThumbnail = GetThumbnailList(m_iActivePlayer)
                ->at(GetCurrentThumbnailIndex(m_iActivePlayer));

        QString sID = pCurrentThumbnail->GetSceneID();
        int iPlayerID = pCurrentThumbnail->GetPlayerID();
        LM::SEvent dummyEvent(LM::SEvent::NONE, nullptr, sID.toStdString(), true, iPlayerID);
        //        ON_CC_THREAD(LM::CKernel::GotoScreenID, this->m_pKernel, dummyEvent, nullptr);
        //        ON_CC_THREAD(LM::CKernel::GotoScreenID, this->m_pKernel, sID.toStdString(), iPlayerID);
        //                m_pKernel->GotoScreenID(sID.toStdString(), iPlayerID);
        m_pKernel->GotoScreenID(dummyEvent, nullptr);
        this->clearInspectorContainer();
        this->setInspectorName("");
    }
}

void CMainWindow::addOneScene(const QString &a_sPreviousID, const QString &a_sNewID, int a_iPlayerID, CTemplate* a_pTemplate)
{
    m_iActivePlayer = a_iPlayerID;
    //    ON_CC_THREAD(LM::CKernel::AddNewScene, m_pKernel, a_pTemplate->GetPath().toStdString(),
    //                 a_sPreviousID.toStdString(), a_sNewID.toStdString(), a_iPlayerID, 0, "");
    m_pKernel->AddNewScene( a_pTemplate->GetPath().toStdString(),
                            a_sPreviousID.toStdString(), a_sNewID.toStdString(), a_iPlayerID, 0, "");
}

void CMainWindow::addTwoScene(const QString &a_sPreviousIDP1, const QString &a_sNewIDP1,
                              const QString &a_sPreviousIDP2, const QString &a_sNewIDP2,
                              CTemplate* a_pTemplate)
{
    //    ON_CC_THREAD(LM::CKernel::AddNewScene, m_pKernel, a_pTemplate->GetPath().toStdString(),
    //                 a_sPreviousIDP1.toStdString(), a_sNewIDP1.toStdString(), PLAYER_1, 0, "");
    //    ON_CC_THREAD(LM::CKernel::AddNewScene, m_pKernel, a_pTemplate->GetPath().toStdString(),
    //                 a_sPreviousIDP2.toStdString(), a_sNewIDP2.toStdString(), PLAYER_2, 0, "");
    m_pKernel->AddNewScene(a_pTemplate->GetPath().toStdString(),
                           a_sPreviousIDP1.toStdString(), a_sNewIDP1.toStdString(), PLAYER_1, 0, "");
    m_pKernel->AddNewScene(a_pTemplate->GetPath().toStdString(),
                           a_sPreviousIDP2.toStdString(), a_sNewIDP2.toStdString(), PLAYER_2, 0, "");
}

void CMainWindow::addGameScene(const QString &a_sPreviousIDP1, const QString &a_sNewIDP1,
                               const QString &a_sPreviousIDP2, const QString &a_sNewIDP2,
                               CTemplate* a_pTemplate, int a_iTemplateNumberP1, int a_iTemplateNumberP2)
{
    //    ON_CC_THREAD(LM::CKernel::AddNewScene, m_pKernel, a_pTemplate->GetPath().toStdString(),
    //                 a_sPreviousIDP1.toStdString(), a_sNewIDP1.toStdString(), PLAYER_1, a_iTemplateNumberP1, a_sNewIDP2.toStdString());
    //    ON_CC_THREAD(LM::CKernel::AddNewScene, m_pKernel, a_pTemplate->GetPath().toStdString(),
    //                 a_sPreviousIDP2.toStdString(), a_sNewIDP2.toStdString(), PLAYER_2, a_iTemplateNumberP2, a_sNewIDP1.toStdString());
    //    ON_CC_THREAD(LM::CKernel::AddSyncID, m_pKernel, a_sNewIDP1.toStdString(), a_sNewIDP2.toStdString());
    m_pKernel->AddNewScene(a_pTemplate->GetPath().toStdString(),
                           a_sPreviousIDP1.toStdString(), a_sNewIDP1.toStdString(), PLAYER_1, a_iTemplateNumberP1, a_sNewIDP2.toStdString());
    m_pKernel->AddNewScene(a_pTemplate->GetPath().toStdString(),
                           a_sPreviousIDP2.toStdString(), a_sNewIDP2.toStdString(), PLAYER_2, a_iTemplateNumberP2, a_sNewIDP1.toStdString());
    m_pKernel->AddSyncID(a_sNewIDP1.toStdString(), a_sNewIDP2.toStdString());
}

void CMainWindow::deleteScene(QString a_sSceneID, bool a_bIsSync)
{
    if(!a_bIsSync)
    {
        //        ON_CC_THREAD(LM::CKernel::DeleteScene, this->m_pKernel, a_sSceneID.toStdString());
        m_pKernel->DeleteScene(a_sSceneID.toStdString());
    }
    else
    {
        //        ON_CC_THREAD(LM::CKernel::DeleteSyncScenes, this->m_pKernel, a_sSceneID.toStdString());
        m_pKernel->DeleteSyncScenes(a_sSceneID.toStdString());
    }
}

void CMainWindow::launchEmulator()
{
    QString execPath = CProjectManager::Instance()->QGetInstallPath() + "/LudoMuse.exe";
    QString jsonPath = CProjectManager::Instance()->QGetProjectPath()+"temp.json";
    produceJson(jsonPath);
    //    QString cmd = execPath + " server " + CProjectManager::Instance()->QGetProjectJsonFile();
    QString cmd = execPath + " server " + jsonPath;
    m_oProcessServer.start(cmd);
    QThread::sleep(2);
    //    cmd = execPath + " client " + CProjectManager::Instance()->QGetProjectJsonFile();
    cmd = execPath + " client " + jsonPath;
    m_oProcessClient.start(cmd);
}

void CMainWindow::saveAs()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save File",
                                                    CProjectManager::Instance()->QGetProjectPath(),
                                                    "Scénarios Ludomuse (*.json)");

    if (!filePath.isNull())
    {
        // Covering empty result
        QFileInfo fileInfo(filePath);
        if(fileInfo.fileName() == ".json" || fileInfo.fileName().startsWith("."))
        {
            filePath = fileInfo.absolutePath() + "/Default_save_name.json";
        }
        // Forcing extension
        if(!filePath.endsWith(".json"))
        {
            filePath = filePath + ".json";
        }
        this->m_sSaveName = filePath;
        this->ui->save->setEnabled(true);
        this->produceJson(filePath);
    }
}

void CMainWindow::save()
{
    this->produceJson(this->m_sSaveName);
}

void CMainWindow::exportProject(const QString& a_rDestination)
{
    if (!a_rDestination.isEmpty())
    {
        std::string projectFolder = CProjectManager::Instance()->GetProjectPath();
        m_oArchiver.CompressFolder(projectFolder.substr(0, projectFolder.length()-1), a_rDestination.toStdString());
    }
}

void CMainWindow::produceJson(const QString& a_rFileName){
    QString jsonResult;
    QFile file( a_rFileName );
    if ( file.open(QIODevice::ReadWrite | QFile::Truncate) )
    {
        QTextStream stream( &file );
        stream.setCodec("UTF-8");
        jsonResult = QString::fromUtf8(this->m_pKernel->ToJson().c_str());
        stream << jsonResult;
        stream.flush();
        file.close();
    }
    //this->ui->jsonDisplayer->setText("Parsing finished");
}

void CMainWindow::launchAddSceneWizard()
{
    CAddSceneWizard* pSceneWizard;
    //    if(m_pCurrentThumbnailWidget1 != Q_NULLPTR)
    if (m_pThumbnailList1->count()>m_iCurrentThumbnailIndex1 && m_iCurrentThumbnailIndex1>= 0)
    {
        //        if(m_pCurrentThumbnailWidget2 != Q_NULLPTR)                           // selected screen for both timeline
        if (m_pThumbnailList2->count()>m_iCurrentThumbnailIndex2 &&m_iCurrentThumbnailIndex2>= 0)
        {
            pSceneWizard = new CAddSceneWizard(m_iActivePlayer,
                                               m_pKernel->GetSceneIDPlayer(PLAYER_1),
                                               m_pKernel->GetSceneIDPlayer(PLAYER_2),
                                               this,
                                               //                                               m_pCurrentThumbnailWidget1->GetSceneID(),
                                               //                                               m_pCurrentThumbnailWidget2->GetSceneID());
                                               m_pThumbnailList1->at(m_iCurrentThumbnailIndex1)->GetSceneID(),
                                               m_pThumbnailList2->at(m_iCurrentThumbnailIndex2)->GetSceneID());
        }
        else                                                                 // Only selected on P1 time line
        {
            pSceneWizard = new CAddSceneWizard(m_iActivePlayer,
                                               m_pKernel->GetSceneIDPlayer(PLAYER_1),
                                               m_pKernel->GetSceneIDPlayer(PLAYER_2),
                                               this,
                                               //                                               m_pCurrentThumbnailWidget1->GetSceneID());
                                               m_pThumbnailList1->at(m_iCurrentThumbnailIndex1)->GetSceneID());
        }
    }
    //    else if(m_pCurrentThumbnailWidget2 != Q_NULLPTR)
    else if (m_pThumbnailList2->count()>m_iCurrentThumbnailIndex2 && m_iCurrentThumbnailIndex2>= 0)
    {
        pSceneWizard = new CAddSceneWizard(m_iActivePlayer,
                                           m_pKernel->GetSceneIDPlayer(PLAYER_1),
                                           m_pKernel->GetSceneIDPlayer(PLAYER_2),
                                           this,
                                           Q_NULLPTR,
                                           //                                           m_pCurrentThumbnailWidget2->GetSceneID());
                                           m_pThumbnailList2->at(m_iCurrentThumbnailIndex2)->GetSceneID());
    }
    else    // No screen selected on both timeline
    {
        pSceneWizard = new CAddSceneWizard(m_iActivePlayer,
                                           m_pKernel->GetSceneIDPlayer(PLAYER_1),
                                           m_pKernel->GetSceneIDPlayer(PLAYER_2),
                                           this);
    }
    connect(pSceneWizard, SIGNAL(addOneScene(QString,QString,int,CTemplate*)), this, SLOT(addOneScene(QString,QString,int,CTemplate*)));
    connect(pSceneWizard, SIGNAL(addTwoScene(QString,QString,QString,QString,CTemplate*)),
            this, SLOT(addTwoScene(QString,QString,QString,QString,CTemplate*)));
    connect(pSceneWizard, SIGNAL(addGameScene(QString,QString,QString,QString,CTemplate*,int,int)),
            this, SLOT(addGameScene(QString,QString,QString,QString,CTemplate*,int,int)));
    pSceneWizard->setModal(true);
    pSceneWizard->show();
}

void CMainWindow::addingSceneFinished(std::string a_sSceneID, int a_iPlayerID)
{
    qDebug() << QString("addingfinished: ") << QString::fromStdString(a_sSceneID);
    m_pLoader->Reset();
    // Clear thumbnails widget
    QLayoutItem *child;
    QLayout* trameContainerLayout = this->ui->trameContainerP1->layout();
    if(trameContainerLayout != Q_NULLPTR)
    {
        while ((child = trameContainerLayout->takeAt(0)) != 0) {
            delete child->widget();
            delete child;
        }
    }
    trameContainerLayout = this->ui->trameContainerP2->layout();
    if(trameContainerLayout != Q_NULLPTR)
    {
        while ((child = trameContainerLayout->takeAt(0)) != 0) {
            delete child->widget();
            delete child;
        }
    }
    m_pThumbnailList1->clear();
    m_pThumbnailList2->clear();
    this->ProcessTree();
    //    this->InitiateThumbnails();
    SetCurrentThumbnailIndex(a_iPlayerID,
                             FindThumbnailIndexByID(QString::fromStdString(a_sSceneID), a_iPlayerID));
    ActivateThumbnails();
    ShowCurrentScene();
}

void CMainWindow::addingSceneFinished(const QString a_sPrevSceneID, const QString a_sSceneID, int a_iPlayerID)
{
    DeactivateThumbnails();
    int iPrevIndex = FindThumbnailIndexByID(a_sPrevSceneID, a_iPlayerID);
    CThumbnailWidget* pNewThumbnail = new CThumbnailWidget(a_sSceneID, a_iPlayerID);
    connect(pNewThumbnail,
            SIGNAL(onClick(CThumbnailWidget*)),
            this,
            SLOT(goToSceneID(CThumbnailWidget*)));
    GetThumbnailList(a_iPlayerID)->insert(iPrevIndex+1, pNewThumbnail);
    SetCurrentThumbnailIndex(a_iPlayerID, iPrevIndex + 1);
    ActivateThumbnails();
    ShowCurrentScene();
    UpdateThumbnailView(a_iPlayerID);
}

void CMainWindow::deletingSceneFinished()
{
    m_pLoader->Reset();
    // Clear thumbnails widget
    QLayoutItem *child;
    QLayout* trameContainerLayout = this->ui->trameContainerP1->layout();
    if(trameContainerLayout != Q_NULLPTR)
    {
        while ((child = trameContainerLayout->takeAt(0)) != 0) {
            delete child->widget();
            delete child;
        }
    }
    trameContainerLayout = this->ui->trameContainerP2->layout();
    if(trameContainerLayout != Q_NULLPTR)
    {
        while ((child = trameContainerLayout->takeAt(0)) != 0) {
            delete child->widget();
            delete child;
        }
    }
    m_pThumbnailList1->clear();
    m_pThumbnailList2->clear();
    this->ProcessTree();
    if (m_pThumbnailList1->count() <= m_iCurrentThumbnailIndex1)
    {
        m_iCurrentThumbnailIndex1--;
    }
    if (m_pThumbnailList2->count() <= m_iCurrentThumbnailIndex2)
    {
        m_iCurrentThumbnailIndex2--;
    }
    ActivateThumbnails();
    ShowCurrentScene();
}

void CMainWindow::deletingSceneFinished(const QString a_sSceneID, int a_iPlayerID)
{
    DeactivateThumbnails();
    if (a_iPlayerID == BOTH_PLAYER)
    {
        int iIndex1 = FindThumbnailIndexByID(a_sSceneID, PLAYER_1);
        int iIndex2 = FindThumbnailIndexByID(a_sSceneID, PLAYER_2);
        CThumbnailWidget* pDeletedThumbnail1 = m_pThumbnailList1->at(iIndex1);
        CThumbnailWidget* pDeletedThumbnail2 = m_pThumbnailList2->at(iIndex2);
        m_pLoader->RemoveThumbnail(pDeletedThumbnail1);
        m_pLoader->RemoveThumbnail(pDeletedThumbnail2);
        m_pThumbnailList1->removeAt(iIndex1);
        m_pThumbnailList2->removeAt(iIndex2);
        pDeletedThumbnail1->deleteLater();
        pDeletedThumbnail2->deleteLater();
        if (iIndex1 < m_iCurrentThumbnailIndex1)
        {
            m_iCurrentThumbnailIndex1--;
        }
        if (iIndex2 < m_iCurrentThumbnailIndex2)
        {
            m_iCurrentThumbnailIndex2--;
        }
    }
    else
    {
        int iIndex = FindThumbnailIndexByID(a_sSceneID, a_iPlayerID);
        CThumbnailWidget* pDeletedThumbnail = GetThumbnailList(a_iPlayerID)->at(iIndex);
        m_pLoader->RemoveThumbnail(pDeletedThumbnail);
        GetThumbnailList(a_iPlayerID)->removeAt(iIndex);
        pDeletedThumbnail->deleteLater();
        if (iIndex < GetCurrentThumbnailIndex(a_iPlayerID))
        {
            SetCurrentThumbnailIndex(a_iPlayerID, GetCurrentThumbnailIndex(a_iPlayerID) - 1);
        }
    }
    ActivateThumbnails();
    ShowCurrentScene();
    UpdateThumbnailView(a_iPlayerID);
}

void CMainWindow::ProcessTree()
{
    LM::CNode *mainNode = this->m_pKernel->GetBehaviorTree();
    for(LM::CNode* currentNode : *mainNode)
    {
        LM::CSceneNode* currentSceneNode = (dynamic_cast<LM::CSceneNode*>(currentNode));
        if(currentSceneNode)
        {
            QString sceneId(currentSceneNode->GetSceneID().c_str());
            int id = this->ScreenIDToPlayerID(sceneId);
            if(id == BOTH_PLAYER)
            {
                connect(this->addSceneToTimeLine(sceneId, PLAYER_1),
                        SIGNAL(onClick(CThumbnailWidget*)),
                        this,
                        SLOT(goToSceneID(CThumbnailWidget*)));
                connect(this->addSceneToTimeLine(sceneId, PLAYER_2),
                        SIGNAL(onClick(CThumbnailWidget*)),
                        this,
                        SLOT(goToSceneID(CThumbnailWidget*)));
            }
            else if(id == PLAYER_1)
            {
                connect(this->addSceneToTimeLine(sceneId, PLAYER_1),
                        SIGNAL(onClick(CThumbnailWidget*)),
                        this,
                        SLOT(goToSceneID(CThumbnailWidget*)));
            }
            else
            {
                connect(this->addSceneToTimeLine(sceneId, PLAYER_2),
                        SIGNAL(onClick(CThumbnailWidget*)),
                        this,
                        SLOT(goToSceneID(CThumbnailWidget*)));
            }
        }

    }
    m_pLoader->AddThumbnailList(m_pThumbnailList1);
    m_pLoader->AddThumbnailList(m_pThumbnailList2);
}

int CMainWindow::ScreenIDToPlayerID(const QString &a_id)
{
    if (m_pKernel->PlayerHasScene(a_id.toStdString(), PLAYER_1))
    {
        if(m_pKernel->PlayerHasScene(a_id.toStdString(), PLAYER_2))
        {
            return BOTH_PLAYER;
        }
        return PLAYER_1;
    }
    return PLAYER_2;
}

CThumbnailWidget* CMainWindow::addSceneToTimeLine(const QString &a_id, int a_playerID)
{

    if(a_playerID == PLAYER_1)
    {
        CThumbnailWidget* nodeWidget = new CThumbnailWidget(a_id, a_playerID, ui->trameContainerP1);
        this->ui->trameContainerP1->layout()->addWidget(nodeWidget);
        this->m_pThumbnailList1->append(nodeWidget);
        //                m_pLoader->AddThumbnail(nodeWidget);
        return nodeWidget;
    }
    else if(a_playerID == PLAYER_2)
    {
        CThumbnailWidget* nodeWidget = new CThumbnailWidget(a_id, a_playerID, ui->trameContainerP2);
        this->ui->trameContainerP2->layout()->addWidget(nodeWidget);
        this->m_pThumbnailList2->append(nodeWidget);
        //                m_pLoader->AddThumbnail(nodeWidget);
        return nodeWidget;
    }

    return new CThumbnailWidget(a_id, a_playerID);
}

void CMainWindow::InspectLabel(LM::CLabelNode* a_pLabel)
{
    // Clear inspector tool bar
    this->setInspectorName("Éditeur de texte");

    // Clear inspector layout from older inspection
    QLayoutItem *child;
    QLayout* inspectorContainerLayout = this->ui->inspectorContainer->layout();
    if(inspectorContainerLayout != Q_NULLPTR)
    {
        while ((child = inspectorContainerLayout->takeAt(0)) != 0) {
            delete child->widget();
            delete child;
        }
    }

    CLabelInspector* inspector = new CLabelInspector(a_pLabel);
    inspectorContainerLayout->addWidget(inspector);
    connect(inspector, SIGNAL(closeInspector()), this, SLOT(clearInspectorContainer()));
    connect(inspector, SIGNAL(modifyLabel(LM::CEntityNode*)), this, SLOT(nodeModified(LM::CEntityNode*)));
}

void CMainWindow::InspectSprite(LM::CSpriteNode* a_pSprite)
{
    // Clear inspector tool bar
    this->setInspectorName("Éditeur d'image");

    // Clear inspector loayout from older inspection
    QLayoutItem *child;
    QLayout* inspectorContainerLayout = this->ui->inspectorContainer->layout();
    if(inspectorContainerLayout != Q_NULLPTR)
    {
        while ((child = inspectorContainerLayout->takeAt(0)) != 0) {
            delete child->widget();
            delete child;
        }

    }
    CSpriteInspector* inspector = new CSpriteInspector(a_pSprite);
    inspectorContainerLayout->addWidget(inspector);
    connect(inspector, SIGNAL(closeInspector()), this, SLOT(clearInspectorContainer()));
    connect(inspector, SIGNAL(modifySprite(LM::CEntityNode*)), this, SLOT(nodeModified(LM::CEntityNode*)));
}

void CMainWindow::nodeModified(LM::CEntityNode* a_pNode)
{
    //    LM::CSceneNode* pScene = a_pSprite->GetParentSceneNode();
    //    LM::CSceneNode* pSyncedScene = m_pKernel->GetSyncedScene(pScene);
    //    if (pSyncedScene != nullptr)
    //    {
    //        LM::CSpriteNode* pSyncedSprite = dynamic_cast<LM::CSpriteNode*>
    //                (pSyncedScene->FindChildByID(a_pSprite->GetID(), true));
    //        if (pSyncedSprite != nullptr && !pSyncedSprite->hasID(""))
    //        {
    //            pSyncedSprite->Copy(a_pSprite);
    //        }
    //    }
    LM::CSceneNode* pScene = a_pNode->GetParentSceneNode();
    LM::CSceneNode* pSyncedScene = m_pKernel->GetSyncedScene(pScene);
    if (pSyncedScene != nullptr)
    {
        std::string sSyncedID = a_pNode->GetHierarchyID();
        if (sSyncedID != "")
        {
            LM::CEntityNode* pBaseNode = dynamic_cast<LM::CEntityNode*>
                    (pScene->FindChildByID(sSyncedID, true));
            LM::CEntityNode* pSyncedNode = dynamic_cast<LM::CEntityNode*>
                    (pSyncedScene->FindChildByID(sSyncedID, true));
            if (pSyncedNode)
            {
                pSyncedNode->Copy(pBaseNode);
            }
        }
    }
}


void CMainWindow::InspectScene(LM::CSceneNode* a_pScene)
{
    // Clear inspector loayout from older inspection
    QLayoutItem *child;
    QLayout* inspectorContainerLayout = this->ui->sceneInspectorContainer->layout();
    if(inspectorContainerLayout != Q_NULLPTR)
    {
        while ((child = inspectorContainerLayout->takeAt(0)) != 0) {
            delete child->widget();
            delete child;
        }
    }

    // Searching which player have the scene
    QString sceneId(a_pScene->GetSceneID().c_str());
    CSceneInspector* sceneInspector = Q_NULLPTR;
    if(m_pKernel->PlayerHasScene(sceneId.toStdString(), PLAYER_1)
            && m_pKernel->PlayerHasScene(sceneId.toStdString(), PLAYER_2)) //  P1 and P2
    {
        sceneInspector = new  CSceneInspector(a_pScene, BOTH_PLAYER, ui->sceneInspectorContainer);
    }
    else if (m_pKernel->PlayerHasScene(sceneId.toStdString(), PLAYER_1)) // P1
    {
        sceneInspector = new  CSceneInspector(a_pScene, PLAYER_1, ui->sceneInspectorContainer);
    }
    else if(m_pKernel->PlayerHasScene(sceneId.toStdString(), PLAYER_2)) // P2
    {
        sceneInspector = new  CSceneInspector(a_pScene, PLAYER_2, ui->sceneInspectorContainer);
    }

    if(sceneInspector)
    {
        inspectorContainerLayout->addWidget(sceneInspector);
        connect(sceneInspector, SIGNAL(addScene()), this, SLOT(launchAddSceneWizard()));
        connect(sceneInspector, SIGNAL(deleteScene(QString, bool)), this, SLOT(deleteScene(QString, bool)));
    }
    else
    {
        qDebug("Scene id is nowhere to be found, not in p1 neither in P2");
    }
}

void CMainWindow::InspectMenuNode(LM::CMenuNode* a_pMenuNode)
{
    // Clear inspector tool bar
    this->setInspectorName("Éditeur de bouton de Navigation");

    // Clear inspector loayout from older inspection
    QLayoutItem *child;
    QLayout* inspectorContainerLayout = this->ui->inspectorContainer->layout();
    if(inspectorContainerLayout != Q_NULLPTR)
    {
        while ((child = inspectorContainerLayout->takeAt(0)) != 0) {
            delete child->widget();
            delete child;
        }

    }
    CMenuNodeInspector* inspector = new CMenuNodeInspector(a_pMenuNode);
    inspectorContainerLayout->addWidget(inspector);
    connect(inspector, SIGNAL(closeInspector()), this, SLOT(clearInspectorContainer()));
}

void CMainWindow::setInspectorName(const QString &a_rName)
{
    QLayout* inspectorToolbarLayout = this->ui->toolBarInspector->layout();
    QLayoutItem *child;
    if(inspectorToolbarLayout != Q_NULLPTR)
    {
        while ((child = inspectorToolbarLayout->takeAt(0)) != 0) {
            delete child->widget();
            delete child;
        }
    }
    else
    {
        inspectorToolbarLayout = new QHBoxLayout();
        this->ui->toolBarInspector->setLayout(inspectorToolbarLayout);
    }
    QLabel* inspectorTitle = new QLabel(a_rName);
    //    inspectorTitle->setAlignment(Qt::AlignHCenter);
    inspectorTitle->setAlignment(Qt::AlignCenter);
    inspectorTitle->setStyleSheet("QLabel{color : white;}");
    this->ui->toolBarInspector->layout()->addWidget(inspectorTitle);
}
/*
void CMainWindow::activeThumbnail(const QString &a_sSceneId, int a_iPlayerId)
{
    int index = 0;
    if(a_iPlayerId == PLAYER_2)
    {
        QLayout* timelineLayout = this->ui->trameContainerP2->layout();
        QLayoutItem *child;
        if(timelineLayout != Q_NULLPTR)
        {
            while ((child = timelineLayout->itemAt(index++)) != 0) {
                CThumbnailWidget* aThumbnail = dynamic_cast<CThumbnailWidget*>(child->widget());
                if(aThumbnail)
                {
                    if(aThumbnail->GetSceneID() == a_sSceneId)
                    {
                        // Desactive previous selection
                        if(m_pCurrentThumbnailWidget2 != Q_NULLPTR)
                        {
                            m_pCurrentThumbnailWidget2->Unselect();
                        }
                        m_pCurrentThumbnailWidget2 = aThumbnail;
                        aThumbnail->Select();

                        // take care of p1 timeline
                        if(m_pCurrentThumbnailWidget1 != Q_NULLPTR)
                        {
                            m_pCurrentThumbnailWidget1->LastActive();
                        }
                        break; // Stop loop cause thumbnail found
                    }
                }
            }
        }
    }
    else
    {
        QLayout* timelineLayout = this->ui->trameContainerP1->layout();
        QLayoutItem *child;
        if(timelineLayout != Q_NULLPTR)
        {
            while ((child = timelineLayout->itemAt(index++)) != 0) {
                CThumbnailWidget* aThumbnail = dynamic_cast<CThumbnailWidget*>(child->widget());
                if(aThumbnail)
                {
                    if(aThumbnail->GetSceneID() == a_sSceneId)
                    {
                        // Desactive previous selection
                        if(m_pCurrentThumbnailWidget1 != Q_NULLPTR)
                        {
                            m_pCurrentThumbnailWidget1->Unselect();
                        }
                        m_pCurrentThumbnailWidget1 = aThumbnail;
                        aThumbnail->Select();

                        // take care of p1 timeline
                        if(m_pCurrentThumbnailWidget2 != Q_NULLPTR)
                        {
                            m_pCurrentThumbnailWidget2->LastActive();
                        }
                        break; // Stop loop cause thumbnail found
                    }
                }
            }
        }
    }
}*/

void CMainWindow::ActivateThumbnails()
{
    QList<CThumbnailWidget*>* pActiveList = GetThumbnailList(m_iActivePlayer);
    QList<CThumbnailWidget*>* pOtherList = GetThumbnailList(GetOtherPlayer(m_iActivePlayer));
    int iActiveIndex = GetCurrentThumbnailIndex(m_iActivePlayer);
    int iOtherIndex = GetCurrentThumbnailIndex(GetOtherPlayer(m_iActivePlayer));


    if (iActiveIndex >= 0 &&
            pActiveList->count() > iActiveIndex)
    {
        pActiveList->at(iActiveIndex)->Select();
        if (iOtherIndex >= 0 &&
                pOtherList->count() > iOtherIndex)
        {
            pOtherList->at(iOtherIndex)->LastActive();
        }
    }
    else if (iOtherIndex >= 0 &&
             pOtherList->count() > iOtherIndex)
    {
        pOtherList->at(iOtherIndex)->Select();
    }
}

void CMainWindow::DeactivateThumbnails()
{
    QList<CThumbnailWidget*>* pActiveList = GetThumbnailList(m_iActivePlayer);
    QList<CThumbnailWidget*>* pOtherList = GetThumbnailList(GetOtherPlayer(m_iActivePlayer));
    int iActiveIndex = GetCurrentThumbnailIndex(m_iActivePlayer);
    int iOtherIndex = GetCurrentThumbnailIndex(GetOtherPlayer(m_iActivePlayer));


    if (iActiveIndex >= 0 &&
            pActiveList->count() > iActiveIndex)
    {
        pActiveList->at(iActiveIndex)->Unselect();
    }
    if (iOtherIndex >= 0 &&
            pOtherList->count() > iOtherIndex)
    {
        pOtherList->at(iOtherIndex)->Unselect();
    }
}

int CMainWindow::FindThumbnailIndexByID(const QString& a_sSceneId, int a_iPlayerId)
{
    QList<CThumbnailWidget*>* pTimeline = GetThumbnailList(a_iPlayerId);
    int iIndex;
    for (iIndex = 0; iIndex < pTimeline->count(); iIndex++)
    {
        if (pTimeline->at(iIndex)->GetSceneID() == a_sSceneId)
        {
            return iIndex;
        }
    }
    return -1;
}

void CMainWindow::on_fileBrowser_clicked(const QModelIndex &index)
{
    QString path = m_pDirModel->fileInfo(index).absoluteFilePath();
    ui->fileListView->setRootIndex(m_pFileModel->setRootPath(path));
    ui->fileListView->SetCurrentPath(path);
}

void CMainWindow::SetCurrentThumbnailIndex(int a_iPlayerID, int a_iNewThumbnailIndex)
{
    if (a_iPlayerID == PLAYER_2)
    {
        m_iCurrentThumbnailIndex2 = a_iNewThumbnailIndex;
    }
    else
    {
        m_iCurrentThumbnailIndex1 = a_iNewThumbnailIndex;
    }
}

int CMainWindow::GetCurrentThumbnailIndex(int a_iPlayerID)
{
    if (a_iPlayerID == PLAYER_2)
    {
        return m_iCurrentThumbnailIndex2;
    }
    else
    {
        return m_iCurrentThumbnailIndex1;
    }
}

QList<CThumbnailWidget*>* CMainWindow::GetThumbnailList(int a_iPlayerID)
{
    if (a_iPlayerID == PLAYER_2)
    {
        return m_pThumbnailList2;
    }
    else
    {
        return m_pThumbnailList1;
    }
}

int CMainWindow::GetOtherPlayer(int a_iPlayerID)
{
    if (a_iPlayerID == PLAYER_2) {
        return PLAYER_1;
    }
    else
    {
        return PLAYER_2;
    }
}

void CMainWindow::SaveThumbnail()
{
    //    ON_CC_THREAD(LM::CKernel::CaptureScreen, m_pKernel, CProjectManager::Instance()->GetProjectPath()+"thumbnails/");
    m_pKernel->CaptureScreen();
}

void CMainWindow::loadCapture(QString a_sSceneID)
{
    int iIndex = FindThumbnailIndexByID(a_sSceneID, PLAYER_1);
    if (iIndex != -1) {
        m_pLoader->RemoveThumbnail(GetThumbnailList(PLAYER_1)->at(iIndex));
        m_pLoader->AddThumbnail(GetThumbnailList(PLAYER_1)->at(iIndex));
    }
    iIndex = FindThumbnailIndexByID(a_sSceneID, PLAYER_2);
    if (iIndex != -1) {
        m_pLoader->RemoveThumbnail(GetThumbnailList(PLAYER_2)->at(iIndex));
        m_pLoader->AddThumbnail(GetThumbnailList(PLAYER_2)->at(iIndex));
    }
}

void CMainWindow::UpdateThumbnailView(int a_iPlayerID)
{
    QLayoutItem *child;
    QWidget* pParentWidget;
    if (a_iPlayerID == PLAYER_2)
    {
        pParentWidget = this->ui->trameContainerP2;
    }
    else
    {
        pParentWidget = this->ui->trameContainerP1;
    }
    if(pParentWidget->layout() != Q_NULLPTR)
    {
        while ((child = pParentWidget->layout()->takeAt(0)) != 0) {
            //            delete child->widget();
            //            delete child;
        }
    }
    QList<CThumbnailWidget*>* pList = GetThumbnailList(a_iPlayerID);
    for (int i = 0; i < pList->length(); i++)
    {
        //        qDebug() << (pList->at(i) != Q_NULLPTR);
        pList->at(i)->setParent(pParentWidget);
        pParentWidget->layout()->addWidget(pList->at(i));
    }
}


void CMainWindow::on_archiveButton_clicked()
{
    QFileDialog* fileDialog = new QFileDialog();
    fileDialog->setDirectory(QDir::home());
    connect(fileDialog, SIGNAL(fileSelected(QString)), this, SLOT(exportProject(QString)));

    fileDialog->show();
}
