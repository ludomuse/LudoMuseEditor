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
#include <QMessageBox>
#include <QInputDialog>

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
#include "ui_CMainWindow.h"
#include "CEditorKernel.h"
#include "CThreadCocos.h"
#include "CThumbnailWidget.h"
#include "CLabelInspector.h"
#include "CSpriteInspector.h"
#include "CTeamNodeInspector.h"
#include "CSoundInspector.h"
#include "CSceneInspector.h"
#include "CDashboardInspector.h"
#include "CMenuNodeInspector.h"
#include "CAddSceneWizard.h"
#include "CLoaderWidget.h"
#include "CProjectManager.h"
#include "CThumbnailsLoaderThread.h"
#include "CTimelineWidget.h"
#include "CExplorerView.h"
#include "CMacroWidget.h"


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

#include <iostream>

#define PERFORM_IN_COCOS_THREAD(FUN) cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]()FUN)



CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CMainWindow),

    m_pKernel(NULL),
    m_sSaveName(""),

    m_oArchiver(this)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(CProjectManager::Instance()->QGetInstallPath().append("/resources/icon.png")));
    setWindowTitle("LudoMuseEditor");

    // Init with loader widget
    ui->mmBotView->setVisible(false);
    ui->toolBarCocos->setVisible(false);
    ui->sceneInspectorContainer->setVisible(false);
    ui->toolBarInspector->setVisible(false);
    ui->scrollInspector->setVisible(false);
    CLoaderWidget* loaderWidget = new CLoaderWidget();
    ui->glViewContainer->layout()->addWidget(loaderWidget);
    connect(loaderWidget, SIGNAL(closeEditor()), this, SLOT(close()));
    connect(loaderWidget, SIGNAL(loadProject(const QString&)), this, SLOT(loadExistingProject(const QString&)));

    //    ui->sceneInspectorContainer->setStyleSheet("#sceneInspectorContainer{background-color : rgb(50, 50, 50);border-right : 1px solid white                                           }");

    // disable save button
    if(m_sSaveName.isEmpty())
    {
        ui->actionEnregistrer->setEnabled(false);
    }

    // Icon creation
    ui->emulateButton->setIcon(QIcon(CProjectManager::Instance()->QGetInstallPath().append("/resources/play_arrow.png")));
    ui->archiveButton->setIcon(QIcon(CProjectManager::Instance()->QGetInstallPath().append("/resources/ic_action_export.png")));

    // Create Template Manager
    this->m_pTemplatesManager = CTemplateManager::Instance();

    // Connect Tool bar
    connect(ui->prevScreenButton, SIGNAL(clicked(bool)), this, SLOT(goToPreviousScene()));
    connect(ui->nextScreenButton, SIGNAL(clicked(bool)), this, SLOT(goToNextScene()));
    connect(ui->dashboardButton, SIGNAL(clicked(bool)), this, SLOT(goToDashBoard()));
    connect(ui->waitingScreenButton, SIGNAL(clicked(bool)), this, SLOT(goToWaitingScreen()));

    connect(ui->emulateButton, SIGNAL(clicked(bool)), this, SLOT(launchEmulator()));
    connect(ui->actionEnregistrer_Sous, SIGNAL(triggered(bool)), this, SLOT(saveAs()));
    connect(ui->actionEnregistrer, SIGNAL(triggered(bool)), this, SLOT(save()));
    connect(ui->actionExporter_une_archive, SIGNAL(triggered(bool)), this, SLOT(on_archiveButton_clicked()));
    //connect(ui->lmwTestButton, SIGNAL(clicked(bool)), this, SLOT(launchAddSceneWizard()));
    //connect(ui->archiveButton, SIGNAL(clicked(bool)), this, SLOT(exportProject));

    // Connect kernel signal
    connect(CEditorKernel::Instance(), SIGNAL(sendMenuNodeSignal(LM::CMenuNode*)), this, SLOT(receiveMenu(LM::CMenuNode*)));

    /*CHAPTERSPROTOTYPE************************************************************************************************************************/
    connect(ui->mmBotView->tabBar(),SIGNAL(tabMoved(int,int)),this,SLOT(reorganizeChapters(int,int)));
    /******************************************************************************************************************************************/

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

    ui->actionEnregistrer->setEnabled(true);
    // Clear loader widget!
    QLayout* glViewContainerLayout = ui->glViewContainer->layout();
    QLayoutItem *child;
    while ((child = glViewContainerLayout->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }

    ui->mmBotView->setVisible(true);
    ui->scrollInspector->setVisible(true);
    ui->sceneInspectorContainer->setVisible(true);
    //    ui->sceneInspectorContainer->setStyleSheet("#sceneInspectorContainer{background-color : rgb(50, 50, 50);border-right :none}");
    ui->toolBarCocos->setVisible(true);
    ui->toolBarInspector->setVisible(true);
    //    ui->toolBarCocos->setStyleSheet("#toolBarCocos{border-bottom: 1px solid black;border-right : 2px solid rgba(255,255,255,255);}");
    ui->inspectorContainer->layout()->setAlignment(Qt::AlignTop);
    QString projectPath = QFileInfo(a_sProjectFile).absolutePath();

    setWindowTitle(QString("LudoMuseEditor -- ") + a_sProjectFile);

//    m_pDirModel = new QFileSystemModel();
//    m_pDirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
//    m_pDirModel->setRootPath(projectPath);
//    ui->fileBrowser->setModel(m_pDirModel);
//    ui->fileBrowser->setRootIndex(m_pDirModel->index(projectPath));
//    ui->fileBrowser->setColumnHidden( 1, true );
//    ui->fileBrowser->setColumnHidden( 2, true );
//    ui->fileBrowser->setColumnHidden( 3, true );
//    m_pFileModel = new QFileSystemModel(this);
//    m_pFileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
//    ui->fileListView->setModel(m_pFileModel);
//    ui->fileListView->setRootIndex(m_pFileModel->setRootPath(projectPath));
//    ui->fileListView->SetCurrentPath(projectPath);
    ui->explorer->SetProjectPath(projectPath);

    AppDelegate* app = (AppDelegate*)(cocos2d::CCQApplication::getInstance());
    app->setPath(m_sSaveName.toStdString());

    QFileInfo tempFile(projectPath + "/" + CProjectManager::Instance()->QGetProjectName() + ".tmp");
    if (tempFile.exists())
    {
        int i = QMessageBox::critical(this, tr("Oups !"),
                                      tr("Il semble que LudoMuseEditor n'a pas été correctement fermé à la dernière édition de ce scénario. Voulez vous le restaurer ?\nSi vous choisissez non, les modifications seront définitvement perdues."),
                                      QMessageBox::No | QMessageBox::Escape, QMessageBox::Yes | QMessageBox::Default);
        if (i == QMessageBox::Yes)
        {
            app->setPath(tempFile.absoluteFilePath().toStdString());
        }
        else
        {
            QFile removeTempFile(tempFile.absoluteFilePath());
            removeTempFile.remove();
        }
    }
    app->setParentWidget(ui->glViewContainer);
    cocos2d::CCQApplication::getInstance()->run();
    m_pKernel = app->getKernel();
    LM::CEditorFindEntityTouchVisitor* dummyVisitor= this->m_pKernel->GetEditorVisitor();
    connect(dummyVisitor, SIGNAL(labelClicked(LM::CLabelNode*)), this, SLOT(receiveLabel(LM::CLabelNode*)));
    connect(dummyVisitor, SIGNAL(spriteClicked(LM::CSpriteNode*)), this, SLOT(receiveSprite(LM::CSpriteNode*)));
    connect(dummyVisitor, SIGNAL(teamNodeClicked(LM::CTeamNode*)), this, SLOT(receiveTeamNode(LM::CTeamNode*)));
    //    connect(m_pKernel, SIGNAL(addingSceneFinished(std::string, int)), this, SLOT(addingSceneFinished(std::string, int)));
    //    connect(m_pKernel, SIGNAL(deletingSceneFinished()), this, SLOT(deletingSceneFinished()));
    connect(m_pKernel, SIGNAL(addingSceneFinished(const QString, const QString, int)),
            this, SLOT(addingSceneFinished(const QString, const QString, int)));
    connect(m_pKernel, SIGNAL(addingSharedSceneFinished(QString,QString,QString)),
            this, SLOT(addingSharedSceneFinished(QString,QString,QString)));
    connect(m_pKernel, SIGNAL(deletingSceneFinished(const QString)),
            this, SLOT(deletingSceneFinished(const QString)));
    connect(m_pKernel, SIGNAL(sendScene(LM::CSceneNode*, bool)),
            this, SLOT(receiveScene(LM::CSceneNode*, bool)));
    connect(m_pKernel, SIGNAL(captureFinished(QString)),
            this, SLOT(loadCapture(QString)));

    /*CHAPTERSPROTOTYPE************************************************************************************************************************/
    //Need to create a timelinewidget by chapter.
    m_pTimelines.append(new CTimelineWidget(ui->timelineContainer));
    ui->timelineContainer->layout()->addWidget(m_pTimelines[0]);
    connect(m_pTimelines[0], SIGNAL(thumbnailSelected()),
            this, SLOT(changeScene()));
    connect(m_pTimelines[0], SIGNAL(saveThumbnail()),
            this, SLOT(saveCapture()));

    std::cout << "1 - MMBOTVIEW SIZE : " << ui->mmBotView->count() << std::endl;
    for (int i = 0; i < m_pKernel->GetChapterNumber();++i){
        CTabPage *tabPage = new CTabPage();
        QString tabName;
        std::cout << "2 - I VALUE : " << i << std::endl;
        tabName = QString::fromStdString(m_pKernel->GetChapterName(i));
        if (i == 0){
            ui->mmBotView->setTabText(i,tabName);
        } else {
            m_pTimelines.append(new CTimelineWidget(tabPage->GetTimeline()));
            tabPage->GetTimeline()->layout()->addWidget(m_pTimelines[i]);
            connect(m_pTimelines[i], SIGNAL(thumbnailSelected()),
                    this, SLOT(changeScene()));
            connect(m_pTimelines[i], SIGNAL(saveThumbnail()),
                    this, SLOT(saveCapture()));
            ui->mmBotView->insertTab(i,tabPage,tabName);
            std::cout << "3 - MMBOTVIEW SIZE : " << ui->mmBotView->count() << " - INDEX : " << i << std::endl;
        }
    }
    std::cout << "4 - MMBOTVIEW SIZE : " << ui->mmBotView->count() << std::endl;
    ui->mmBotView->setMovable(true);
    /******************************************************************************************************************************************/
    std::cout << "5.1 - BEFORE TREE" << std::endl;
    this->ProcessTree();
    std::cout << "5.2 - AFTER TREE" << std::endl;

    InspectScene(m_pKernel->m_pCurrentScene);
    m_pTimelines[0]->SetCurrentPlayer(m_pKernel->GetActivePlayer());
    m_pTimelines[0]->SelectThumbnail(QString::fromStdString(m_pKernel->m_pCurrentScene->GetSceneID()));
    std::cout << "6.1 - TIMELINE LOADED" << std::endl;
    for (int j = 0; j < m_pTimelines.size(); ++j){
        m_pTimelines[j]->UpdateTimeline();
        m_pTimelines[j]->LoadPreviews();
    }
    std::cout << "6.2 - TIMELINES LOADED" << std::endl;

//    ui->macros->Init();
    ui->macros->SetKernel(m_pKernel);
    ui->macros->Init();
    connect(ui->macros, SIGNAL(macroModified()), this, SLOT(reloadScene()));
    connect(ui->macros, SIGNAL(deleteClicked()), this, SLOT(clearInspectorContainer()));
//    CExplorerView *cev = new CExplorerView(projectPath);


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

void CMainWindow::receiveTeamNode(LM::CTeamNode* a_pTeamNode)
{
    this->InspectTeamNode(a_pTeamNode);
}

void CMainWindow::receiveMenu(LM::CMenuNode* a_pMenuNode)
{
    qDebug("received CMenuNode");
    this->InspectMenuNode(a_pMenuNode);
}

void CMainWindow::receiveScene(LM::CSceneNode *a_pScene, bool a_bIsNav)
{
    this->clearSceneInspector();
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
        //        child->widget()->deleteLater();
        //        delete child;
    }
    this->setInspectorName("");
}

void CMainWindow::clearSceneInspector()
{
    QLayout* sceneInspectorLayout = this->ui->sceneInspectorContainer->layout();
    QLayoutItem *child;
    while ((child = sceneInspectorLayout->takeAt(0)) != 0) {
        child->widget()->close();
    }
}

void CMainWindow::ShowCurrentScene()
{
    clearInspectorContainer();
    LM::SEvent dummyEvent(LM::SEvent::NONE, nullptr, m_pTimelines[ui->mmBotView->currentIndex()]->GetCurrentSceneID().toStdString(), true, m_pTimelines[ui->mmBotView->currentIndex()]->GetCurrentPlayer());
    ON_CC_THREAD(LM::CKernel::GotoScreenID, this->m_pKernel, dummyEvent, nullptr);
}

void CMainWindow::changeScene()
{
    //saveCapture();
    ShowCurrentScene();
}

void CMainWindow::goToNextScene()
{
    saveCapture();
    m_pTimelines[ui->mmBotView->currentIndex()]->SelectNextThumbnail();
    ShowCurrentScene();
}

void CMainWindow::goToPreviousScene()
{
    saveCapture();
    m_pTimelines[ui->mmBotView->currentIndex()]->SelectPrevThumbnail();
    ShowCurrentScene();
}

void CMainWindow::goToDashBoard()
{
    clearInspectorContainer();
    m_pTimelines[ui->mmBotView->currentIndex()]->UnselectThumbnails();
    ON_CC_THREAD(LM::CKernel::GotoDashboard, this->m_pKernel);
}

void CMainWindow::goToWaitingScreen()
{
    clearInspectorContainer();
    m_pTimelines[ui->mmBotView->currentIndex()]->UnselectThumbnails();
    ON_CC_THREAD(LM::CKernel::GotoWaitingScene, this->m_pKernel);
}

void CMainWindow::addOneScene(const QString &a_sPreviousID, const QString &a_sNewID, int a_iPlayerID, CTemplate* a_pTemplate)
{
    //    m_iActivePlayer = a_iPlayerID;
//    ON_CC_THREAD(LM::CKernel::AddNewScene, m_pKernel, a_pTemplate->GetPath().toStdString(),
//                 a_sPreviousID.toStdString(), a_sNewID.toStdString(), a_iPlayerID, 0, "");
    PERFORM_IN_COCOS_THREAD({
        this->m_pKernel->AddNewScene(a_pTemplate->GetPath().toStdString(),
                               a_sPreviousID.toStdString(),
                               a_sNewID.toStdString(),
                               a_iPlayerID,
                               ui->mmBotView->currentIndex());
    });

    //    m_pKernel->AddNewScene( a_pTemplate->GetPath().toStdString(),
    //                            a_sPreviousID.toStdString(), a_sNewID.toStdString(), a_iPlayerID, 0, "");
}

void CMainWindow::addTwoScene(const QString &a_sPreviousIDP1, const QString &a_sNewIDP1,
                              const QString &a_sPreviousIDP2, const QString &a_sNewIDP2,
                              CTemplate* a_pTemplate)
{
    PERFORM_IN_COCOS_THREAD({
        this->m_pKernel->AddNewScene(a_pTemplate->GetPath().toStdString(),
                                a_sPreviousIDP1.toStdString(),
                                a_sNewIDP1.toStdString(),
                                PLAYER_1,
                                ui->mmBotView->currentIndex());
    });
//    ON_CC_THREAD(LM::CKernel::AddNewScene, m_pKernel, a_pTemplate->GetPath().toStdString(),
//                 a_sPreviousIDP1.toStdString(), a_sNewIDP1.toStdString(), PLAYER_1, 0, "");

    PERFORM_IN_COCOS_THREAD({
        this->m_pKernel->AddNewScene(a_pTemplate->GetPath().toStdString(),
                                a_sPreviousIDP2.toStdString(), a_sNewIDP2.toStdString(),
                                PLAYER_2,
                                ui->mmBotView->currentIndex());
    });

//    ON_CC_THREAD(LM::CKernel::AddNewScene, m_pKernel, a_pTemplate->GetPath().toStdString(),
//                 a_sPreviousIDP2.toStdString(), a_sNewIDP2.toStdString(), PLAYER_2, 0, "");

    //    m_pKernel->AddNewScene(a_pTemplate->GetPath().toStdString(),
    //                           a_sPreviousIDP1.toStdString(), a_sNewIDP1.toStdString(), PLAYER_1, 0, "");
    //    m_pKernel->AddNewScene(a_pTemplate->GetPath().toStdString(),
    //                           a_sPreviousIDP2.toStdString(), a_sNewIDP2.toStdString(), PLAYER_2, 0, "");
}

void CMainWindow::addSharedScene(const QString &a_sPreviousIDP1, const QString &a_sPreviousIDP2,
                                 const QString &a_sNewIDP, CTemplate* a_pTemplate)
{
    PERFORM_IN_COCOS_THREAD({
        this->m_pKernel->AddNewSharedScene(a_pTemplate->GetPath().toStdString(),
                                a_sPreviousIDP1.toStdString(),
                                a_sPreviousIDP2.toStdString(),
                                a_sNewIDP.toStdString(),
                                0, "", ui->mmBotView->currentIndex());
    });

//    ON_CC_THREAD(LM::CKernel::AddNewSharedScene, m_pKernel, a_pTemplate->GetPath().toStdString(),
//                 a_sPreviousIDP1.toStdString(), a_sPreviousIDP2.toStdString(), a_sNewIDP.toStdString(), "", 0);
}

void CMainWindow::addGameScene(const QString &a_sPreviousIDP1, const QString &a_sNewIDP1,
                               const QString &a_sPreviousIDP2, const QString &a_sNewIDP2,
                               CTemplate* a_pTemplate, int a_iTemplateNumberP1, int a_iTemplateNumberP2)
{
    ON_CC_THREAD(LM::CKernel::AddSyncID, m_pKernel, a_sNewIDP1.toStdString(), a_sNewIDP2.toStdString());
    ON_CC_THREAD(LM::CKernel::AddNewScene, m_pKernel, a_pTemplate->GetPath().toStdString(),
                 a_sPreviousIDP1.toStdString(), a_sNewIDP1.toStdString(), PLAYER_1, 0, a_iTemplateNumberP1, a_sNewIDP2.toStdString());
    ON_CC_THREAD(LM::CKernel::AddNewScene, m_pKernel, a_pTemplate->GetPath().toStdString(),
                 a_sPreviousIDP2.toStdString(), a_sNewIDP2.toStdString(), PLAYER_2, 0, a_iTemplateNumberP2, a_sNewIDP1.toStdString());
//    m_pKernel->AddNewScene(a_pTemplate->GetPath().toStdString(),
//                           a_sPreviousIDP1.toStdString(), a_sNewIDP1.toStdString(), PLAYER_1, a_iTemplateNumberP1, a_sNewIDP2.toStdString());
//    m_pKernel->AddNewScene(a_pTemplate->GetPath().toStdString(),
//                           a_sPreviousIDP2.toStdString(), a_sNewIDP2.toStdString(), PLAYER_2, a_iTemplateNumberP2, a_sNewIDP1.toStdString());
//    m_pKernel->AddSyncID(a_sNewIDP1.toStdString(), a_sNewIDP2.toStdString());
}

void CMainWindow::deleteScene(QString a_sSceneID, bool a_bIsSync)
{
    clearInspectorContainer();
    if(!a_bIsSync)
    {
        ON_CC_THREAD(LM::CKernel::DeleteScene, this->m_pKernel, a_sSceneID.toStdString());
        //        m_pKernel->DeleteScene(a_sSceneID.toStdString());
    }
    else
    {
        ON_CC_THREAD(LM::CKernel::DeleteSyncScenes, this->m_pKernel, a_sSceneID.toStdString());
        //        m_pKernel->DeleteSyncScenes(a_sSceneID.toStdString());
    }
}

void CMainWindow::launchEmulator()
{
    QString execPath = CProjectManager::Instance()->QGetInstallPath() + "/LudoMuse";
    QString jsonPath = CProjectManager::Instance()->QGetProjectPath() + CProjectManager::Instance()->QGetProjectName() + ".tmp";
    produceJson(jsonPath);
    //    QString cmd = execPath + " server " + CProjectManager::Instance()->QGetProjectJsonFile();
    //QString cmd = execPath + " server " + jsonPath;
    m_oProcessServer.setProgram(execPath);
    QStringList oArgsServer;
    oArgsServer << "server" << jsonPath;
    m_oProcessServer.setArguments(oArgsServer);

    m_oProcessClient.setProgram(execPath);
    QStringList oArgsClient;
    oArgsClient << "client" << jsonPath;
    m_oProcessClient.setArguments(oArgsClient);


    QString sLogServer = CProjectManager::Instance()->QGetAbsoluteWritablePath() + "/server.log";
    QString sLogClient = CProjectManager::Instance()->QGetAbsoluteWritablePath() + "/client.log";

    m_oProcessServer.setStandardOutputFile(sLogServer);
    m_oProcessServer.setStandardErrorFile(sLogServer);
    m_oProcessClient.setStandardOutputFile(sLogClient);
    m_oProcessClient.setStandardErrorFile(sLogClient);

    m_oProcessServer.start();
    QThread::sleep(2);
    //    cmd = execPath + " client " + CProjectManager::Instance()->QGetProjectJsonFile();
//    cmd = execPath + " client " + jsonPath;
    m_oProcessClient.start();
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
        this->ui->actionEnregistrer->setEnabled(true);
        save();

        CProjectManager::Instance()->SetProjectFile(filePath);
    }
}

void CMainWindow::save()
{
    this->produceJson(this->m_sSaveName);
    QFile tempFile(CProjectManager::Instance()->QGetProjectPath() + "/" +
                   CProjectManager::Instance()->QGetProjectName() + ".tmp");
    if (tempFile.exists())
    {
        tempFile.remove();
    }
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
    pSceneWizard = new CAddSceneWizard(m_pTimelines[ui->mmBotView->currentIndex()]->GetCurrentScenePlayer(),
                                       m_pKernel->GetSceneIDPlayer(PLAYER_1),
                                       m_pKernel->GetSceneIDPlayer(PLAYER_2),
                                       this,
                                       m_pTimelines[ui->mmBotView->currentIndex()]->GetPlayerSceneID(PLAYER_1),
                                       m_pTimelines[ui->mmBotView->currentIndex()]->GetPlayerSceneID(PLAYER_2),
                                       m_pKernel);

    connect(pSceneWizard, SIGNAL(addOneScene(QString,QString,int,CTemplate*)), this, SLOT(addOneScene(QString,QString,int,CTemplate*)));
//    connect(pSceneWizard, SIGNAL(addTwoScene(QString,QString,QString,QString,CTemplate*)),
//            this, SLOT(addTwoScene(QString,QString,QString,QString,CTemplate*)));
    connect(pSceneWizard, SIGNAL(addSharedScene(QString, QString, QString, CTemplate*)),
            this, SLOT(addSharedScene(QString,QString,QString,CTemplate*)));
    connect(pSceneWizard, SIGNAL(addGameScene(QString,QString,QString,QString,CTemplate*,int,int)),
            this, SLOT(addGameScene(QString,QString,QString,QString,CTemplate*,int,int)));
    pSceneWizard->setModal(true);
    pSceneWizard->show();
}

/*CHAPTERSPROTOTYPE************************************************************************************************************************/
void CMainWindow::addingChapter(){
    CTabPage *tabPage = new CTabPage();
    bool ok;
    QString tabName = QInputDialog::getText(this,"Nom du chapitre","Veuillez entrez un nom pour le chapitre :",QLineEdit::Normal,"Introduction",&ok);
    if (ok){
        ui->mmBotView->insertTab(ui->mmBotView->currentIndex()+1, tabPage, tabName);

        m_pKernel->AddChapter(tabName.toStdString(), ui->mmBotView->currentIndex()+1);

        m_pTimelines.insert(ui->mmBotView->currentIndex()+1,new CTimelineWidget(tabPage->GetTimeline()));
        tabPage->GetTimeline()->layout()->addWidget(m_pTimelines[ui->mmBotView->currentIndex()+1]);
        connect(m_pTimelines[ui->mmBotView->currentIndex()+1], SIGNAL(thumbnailSelected()),
                this, SLOT(changeScene()));
        connect(m_pTimelines[ui->mmBotView->currentIndex()+1], SIGNAL(saveThumbnail()),
                this, SLOT(saveCapture()));
    }
}

void CMainWindow::deletingChapter(){
    ui->mmBotView->removeTab(ui->mmBotView->currentIndex());
    QString tabName = "Chapitre ";
}

void CMainWindow::reorganizeChapters(int from, int to){
    m_pKernel->reorganizeChapters(from,to);
    CTimelineWidget *saveTimeline = m_pTimelines[from];
    m_pTimelines.erase(m_pTimelines.begin()+from);
    m_pTimelines.insert(to,saveTimeline);
}

/******************************************************************************************************************************************/

void CMainWindow::addingSceneFinished(const QString& a_sPrevSceneID, const QString& a_sSceneID, int a_iPlayerID)
{
    saveCapture();
    qDebug() << "Scene : " << a_sSceneID << " - Synced : " << m_pKernel->GetSyncedScene(a_sSceneID);
    m_pTimelines[ui->mmBotView->currentIndex()]->InsertScene(a_sPrevSceneID, a_sSceneID, a_iPlayerID, m_pKernel->GetSyncedScene(a_sSceneID));
    m_pTimelines[ui->mmBotView->currentIndex()]->UpdateTimeline();
    ShowCurrentScene();
}

void CMainWindow::addingSharedSceneFinished(const QString& a_sPrevSceneID1, const QString& a_sPrevSceneID2, const QString& a_sSceneID)
{
    saveCapture();
    m_pTimelines[ui->mmBotView->currentIndex()]->InsertSharedScene(a_sPrevSceneID1, a_sPrevSceneID2, a_sSceneID);
    m_pTimelines[ui->mmBotView->currentIndex()]->UpdateTimeline();
    ShowCurrentScene();
}

void CMainWindow::deletingSceneFinished(const QString a_sSceneID)
{
    m_pTimelines[ui->mmBotView->currentIndex()]->RemoveScene(a_sSceneID);
    m_pTimelines[ui->mmBotView->currentIndex()]->UpdateTimeline();
    ShowCurrentScene();
}

void CMainWindow::ProcessTree()
{
    int sceneIndex = 0;
    int chapterIndex = 0;
    int change = m_pKernel->GetSceneNumberCalculated(chapterIndex);

    LM::CNode *mainNode = this->m_pKernel->GetBehaviorTree();

    for(LM::CNode* currentNode : *mainNode)
    {
        LM::CSceneNode* currentSceneNode = (dynamic_cast<LM::CSceneNode*>(currentNode));
        if(currentSceneNode)
        {
            QString sceneId(QString::fromStdString(currentSceneNode->GetSceneID()));
            if (sceneIndex == change){
                sceneIndex = 0;
                chapterIndex++;
                change = m_pKernel->GetSceneNumberCalculated(chapterIndex);
            }
            /*std::cout << "CHAPTERHASSCENE : SCENE " << sceneId.toStdString() << " CHAPTERINDEX " << chapterIndex << " RESULT " << m_pKernel->ChapterHasScene(chapterIndex,sceneId.toStdString()) << "\n";
            std::cout.flush();
            if (!m_pKernel->ChapterHasScene(chapterIndex,sceneId.toStdString())){
                chapterIndex++;
            }*/
            m_pTimelines[chapterIndex]->PushScene(sceneId, ScreenIDToPlayerID(sceneId), m_pKernel->GetSyncedScene(sceneId));
            sceneIndex++;
        }
    }
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

void CMainWindow::InspectLabel(LM::CLabelNode* a_pLabel)
{
    clearInspectorContainer();
    // Clear inspector tool bar
    this->setInspectorName("Éditeur de texte");

    // Clear inspector layout from older inspection
    QLayoutItem *child;
    QLayout* inspectorContainerLayout = this->ui->inspectorContainer->layout();
    //    if(inspectorContainerLayout != Q_NULLPTR)
    //    {
    //        while ((child = inspectorContainerLayout->takeAt(0)) != 0) {
    //            delete child->widget();
    //            delete child;
    //        }
    //    }

    CLabelInspector* inspector = new CLabelInspector(a_pLabel);
    inspector->setAttribute( Qt::WA_DeleteOnClose );
    inspectorContainerLayout->addWidget(inspector);
    connect(inspector, SIGNAL(closeInspector()), this, SLOT(clearInspectorContainer()));
    connect(inspector, SIGNAL(modifyLabel(LM::CEntityNode*)), this, SLOT(nodeModified(LM::CEntityNode*)));
}

void CMainWindow::InspectSprite(LM::CSpriteNode* a_pSprite)
{
    clearInspectorContainer();
    // Clear inspector tool bar
    this->setInspectorName("Éditeur d'image");

    // Clear inspector loayout from older inspection
    QLayoutItem *child;
    QLayout* inspectorContainerLayout = this->ui->inspectorContainer->layout();
    //    if(inspectorContainerLayout != Q_NULLPTR)
    //    {
    //        while ((child = inspectorContainerLayout->takeAt(0)) != 0) {
    //            delete child->widget();
    //            delete child;
    //        }

    //    }
    CSpriteInspector* inspector = new CSpriteInspector(a_pSprite, this->ui->inspectorContainer);
    CSoundInspector* soundInspector = new CSoundInspector(a_pSprite, this->ui->inspectorContainer);
    inspector->setAttribute( Qt::WA_DeleteOnClose );
    soundInspector->setAttribute( Qt::WA_DeleteOnClose );
    soundInspector->hide();

    inspectorContainerLayout->addWidget(inspector);
    inspectorContainerLayout->addWidget(soundInspector);

    connect(inspector, SIGNAL(callSoundInspector()), inspector, SLOT(hide()));
    connect(inspector, SIGNAL(callSoundInspector()), soundInspector, SLOT(show()));
    connect(soundInspector, SIGNAL(closeInspector()), inspector, SLOT(show()));
    connect(soundInspector, SIGNAL(closeInspector()), soundInspector, SLOT(hide()));

    connect(inspector, SIGNAL(closeInspector()), this, SLOT(clearInspectorContainer()));
    connect(inspector, SIGNAL(modifySprite(LM::CEntityNode*)), this, SLOT(nodeModified(LM::CEntityNode*)));

    connect(soundInspector, SIGNAL(modifySound(LM::CEntityNode*,QString,QString)), this, SLOT(nodeSoundModified(LM::CEntityNode*,QString,QString)));
    connect(soundInspector, SIGNAL(removeSound(LM::CEntityNode*)), this, SLOT(nodeSoundRemoved(LM::CEntityNode*)));
}

void CMainWindow::InspectTeamNode(LM::CTeamNode* a_pTeamNode)
{
    clearInspectorContainer();
    // Clear inspector tool bar
    this->setInspectorName("Éditeur de jeu en équipe");

    // Clear inspector loayout from older inspection
    QLayoutItem *child;
    QLayout* inspectorContainerLayout = this->ui->inspectorContainer->layout();
    //    if(inspectorContainerLayout != Q_NULLPTR)
    //    {
    //        while ((child = inspectorContainerLayout->takeAt(0)) != 0) {
    //            delete child->widget();
    //            delete child;
    //        }

    //    }
    CTeamNodeInspector* inspector = new CTeamNodeInspector(a_pTeamNode, this->ui->inspectorContainer);
    inspector->setAttribute( Qt::WA_DeleteOnClose );

    inspectorContainerLayout->addWidget(inspector);

    connect(inspector, SIGNAL(closeInspector()), this, SLOT(clearInspectorContainer()));
    connect(inspector, SIGNAL(modifyTeamNode(LM::CEntityNode*)), this, SLOT(nodeModified(LM::CEntityNode*)));

}

void CMainWindow::nodeModified(LM::CEntityNode* a_pNode)
{
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
                int originalWidth = pSyncedNode->GetWidth();
                int originalHeight = pSyncedNode->GetHeight();
                pSyncedNode->Copy(pBaseNode);
                pSyncedNode->SetWidth(originalWidth, false);
                pSyncedNode->SetHeight(originalHeight, false);
            }
        }
    }
    produceJson(CProjectManager::Instance()->QGetProjectPath() + "/" +
                CProjectManager::Instance()->QGetProjectName() + ".tmp");
}

void CMainWindow::nodeSoundModified(LM::CEntityNode* a_pNode, const QString& a_sEvent, const QString& a_sArgument)
{
    LM::CEventCallback oCallback("PlaySound", m_pKernel, &LM::CKernel::PlaySoundCallback,
                                 LM::SEvent(LM::SEvent::STRING, a_pNode, a_sArgument.toStdString()));
    a_pNode->RemoveCallbacks("PlaySound");
    a_pNode->AddListener(a_sEvent.toStdString(), oCallback);

    LM::CSceneNode* pScene = a_pNode->GetParentSceneNode();
    LM::CSceneNode* pSyncedScene = m_pKernel->GetSyncedScene(pScene);
    if (pSyncedScene != nullptr)
    {
        std::string sSyncedID = a_pNode->GetID();
        if (sSyncedID != "")
        {
            LM::CEntityNode* pSyncedNode = dynamic_cast<LM::CEntityNode*>
                    (pSyncedScene->FindChildByID(sSyncedID, true));
            if (pSyncedNode)
            {
                pSyncedNode->RemoveCallbacks("PlaySound");
                pSyncedNode->AddListener(a_sEvent.toStdString(), oCallback);
            }
        }
    }
}

void CMainWindow::nodeSoundRemoved(LM::CEntityNode* a_pNode)
{
    a_pNode->RemoveCallbacks("PlaySound");

    LM::CSceneNode* pScene = a_pNode->GetParentSceneNode();
    LM::CSceneNode* pSyncedScene = m_pKernel->GetSyncedScene(pScene);
    if (pSyncedScene != nullptr)
    {
        std::string sSyncedID = a_pNode->GetID();
        if (sSyncedID != "")
        {
            LM::CEntityNode* pSyncedNode = dynamic_cast<LM::CEntityNode*>
                    (pSyncedScene->FindChildByID(sSyncedID, true));
            if (pSyncedNode)
            {
                pSyncedNode->RemoveCallbacks("PlaySound");
            }
        }
    }
}

void CMainWindow::InspectScene(LM::CSceneNode* a_pScene)
{
    // Clear inspector loayout from older inspection
    //    QLayoutItem *child;
    //    QLayout* inspectorContainerLayout = this->ui->sceneInspectorContainer->layout();
    //    if(inspectorContainerLayout != Q_NULLPTR)
    //    {
    //        while ((child = inspectorContainerLayout->takeAt(0)) != 0) {
    //            delete child->widget();
    //            delete child;
    //        }
    //    }

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
        sceneInspector->setAttribute(Qt::WA_DeleteOnClose);
        ui->sceneInspectorContainer->layout()->addWidget(sceneInspector);
        connect(sceneInspector, SIGNAL(addScene()), this, SLOT(launchAddSceneWizard()));
        connect(sceneInspector, SIGNAL(deleteScene(QString, bool)), this, SLOT(deleteScene(QString, bool)));
        /*CHAPTERSPROTOTYPE************************************************************************************************************************/
        connect(sceneInspector,SIGNAL(addChapter()),this,SLOT(addingChapter()));
        connect(sceneInspector,SIGNAL(deleteChapter()),this,SLOT(deletingChapter()));
        /******************************************************************************************************************************************/
    }
    else
    {
        qDebug("Scene id is nowhere to be found, not in p1 neither in P2");
        if (sceneId == "Dashboard")
        {
            CDashboardInspector* dashboardInspector = new CDashboardInspector(a_pScene, ui->sceneInspectorContainer);
            dashboardInspector->setAttribute(Qt::WA_DeleteOnClose);
            ui->sceneInspectorContainer->layout()->addWidget(dashboardInspector);
        }
    }
}

void CMainWindow::InspectMenuNode(LM::CMenuNode* a_pMenuNode)
{
    clearInspectorContainer();
    // Clear inspector tool bar
    this->setInspectorName("Éditeur de bouton de Navigation");

    // Clear inspector loayout from older inspection
    QLayoutItem *child;
    QLayout* inspectorContainerLayout = this->ui->inspectorContainer->layout();
    //    if(inspectorContainerLayout != Q_NULLPTR)
    //    {
    //        while ((child = inspectorContainerLayout->takeAt(0)) != 0) {
    //            delete child->widget();
    //            delete child;
    //        }

    //    }
    CMenuNodeInspector* inspector = new CMenuNodeInspector(a_pMenuNode);
    inspector->setAttribute(Qt::WA_DeleteOnClose);
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

//void CMainWindow::on_fileBrowser_clicked(const QModelIndex &index)
//{
//    QString path = m_pDirModel->fileInfo(index).absoluteFilePath();
//    ui->fileListView->setRootIndex(m_pFileModel->setRootPath(path));
//    ui->fileListView->SetCurrentPath(path);
//}

void CMainWindow::saveCapture()
{
    //    ON_CC_THREAD(LM::CKernel::CaptureScreen, m_pKernel, CProjectManager::Instance()->GetProjectPath()+"thumbnails/");
    ON_CC_THREAD(LM::CKernel::CaptureScreen, m_pKernel);
    //    m_pKernel->CaptureScreen();
}

void CMainWindow::loadCapture(QString a_sSceneID)
{
    m_pTimelines[ui->mmBotView->currentIndex()]->LoadPreview(a_sSceneID);
}

void CMainWindow::on_archiveButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choisir l'emplacement de l'archive"), QDir::home().absolutePath(), QFileDialog::ShowDirsOnly);

    exportProject(dir);
}

void CMainWindow::reloadScene()
{
    m_pKernel->ReloadScreen();
}


void CMainWindow::closeEvent(QCloseEvent *a_oEvent)
{
    QFile tempFile(CProjectManager::Instance()->QGetProjectPath() + "/" +
                   CProjectManager::Instance()->QGetProjectName() + ".tmp");

    if (tempFile.exists())
    {
        int answer = QMessageBox::warning( this, tr("Modifications en cours"),
                                            tr("Vous avez des modifications non sauvegardées,\nêtes vous sûr de vouloir quitter ?\n(les modifications seront perdues)"),
                                            QMessageBox::Cancel | QMessageBox::Escape | QMessageBox::Default, QMessageBox::Yes);

        if (answer == QMessageBox::Cancel)
        {
            a_oEvent->ignore();
            return;
        }
    }

    tempFile.remove();
    a_oEvent->accept();
}
