#ifdef _WIN32
#include <WinSock2.h>
#endif

#include <GL/glew.h>
#include <vector>
#include <string>

#ifdef __linux__
    //linux code goes here
#elif _WIN32
    #include <Windows.h>
    // windows code goes here
#endif

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

// Include QT
#include <QtWidgets>
#include <QThread>
#include <QProcess>
#include <QBoxLayout>
#include <QLayoutItem>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QDirModel>

// Include cocos
#include "cocos2d.h"

// Include LudoMuse
#include "Classes/Engine/Include/CNode.h"
#include "Classes/Engine/Include/CSceneNode.h"
#include "Classes/Engine/Include/CSequenceNode.h"
#include "Classes/Engine/Include/CMenuNode.h"
#include "Classes/Engine/Include/CCallback.h"
#include "Classes/Engine/Include/CEditorFindEntityTouchVisitor.h"
#include "Classes/Modules/Util/Include/Util.h"



CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CMainWindow),
    m_pCurrentThumbnailWidget1(Q_NULLPTR),
    m_pCurrentThumbnailWidget2(Q_NULLPTR),
    m_iActivePlayer(BOTH_PLAYER),
    m_pKernel(NULL),
    m_sSaveName("")
{
    ui->setupUi(this);

    index = 1;

    // Init with loader widget
    ui->toolBarCocos->setVisible(false);
    CLoaderWidget* loaderWidget = new CLoaderWidget();
    ui->glViewContainer->layout()->addWidget(loaderWidget);
    connect(loaderWidget, SIGNAL(closeEditor()), this, SLOT(close()));
    connect(loaderWidget, SIGNAL(loadProject(const QString&)), this, SLOT(loadExistingProject(const QString&)));

    ui->sceneInspectorContainer->setStyleSheet("#sceneInspectorContainer{background-color : rgb(50, 50, 50);border-right :none}");

    // disable save button
    if(m_sSaveName.isEmpty())
    {
        ui->save->setEnabled(false);
    }

    // File browser (tree and file display)
    // All path will be changed TODO
    QString temporaryPath("D:\\IHMTEK\\LudoMuseEditorCocos\\build-LudoMuseEditor-Clone_de_Desktop_Qt_5_6_0_MSVC2015_32bit-Debug\\debug");


    // Icon creation
    ui->emulateButton->setIcon(QIcon("resources/emulate.png"));

    // Create Template Manager
    this->m_pTemplatesManager = CTemplateManager::Instance();

    // Connect Tool bar
    connect(ui->prevScreenButton, SIGNAL(clicked(bool)), this, SLOT(goToPreviousScene()));
    connect(ui->nextScreenButton, SIGNAL(clicked(bool)), this, SLOT(goToNextScene()));
    connect(ui->emulateButton, SIGNAL(clicked(bool)), this, SLOT(launchEmulator()));
    connect(ui->JsonGo, SIGNAL(clicked(bool)), this, SLOT(saveAs()));
    connect(ui->save, SIGNAL(clicked(bool)), this, SLOT(save()));
    //connect(ui->lmwTestButton, SIGNAL(clicked(bool)), this, SLOT(launchAddSceneWizard()));

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
    thread->start();

    ui->sceneInspectorContainer->setStyleSheet("#sceneInspectorContainer{background-color : rgb(50, 50, 50);border-right :none}");
    ui->toolBarCocos->setVisible(true);
    ui->toolBarCocos->setStyleSheet("#toolBarCocos{border-bottom: 1px solid black;border-right : 2px solid rgba(255,255,255,255);}");

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
}


void CMainWindow::receiveHWND(int number)
{
    WId player1ID = (WId) number;
    QWindow *container = QWindow::fromWinId(player1ID);
    QWidget *cocosWidget= QWidget::createWindowContainer(container);
    this->ui->glViewContainer->layout()->addWidget(cocosWidget);
    this->update();
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

    QString currentScene(m_pKernel->m_pCurrentScene->GetSceneID().c_str());
    m_iActivePlayer = m_pKernel->GetCurrentPlayer();
    this->activeThumbnail(currentScene, m_iActivePlayer);
    this->InspectScene(m_pKernel->m_pCurrentScene);
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
    if(a_bIsNav)
    {
        qDebug("navigation transition -> changing active thumbnail");
        this->activeThumbnail(QString(a_pScene->GetSceneID().c_str()), m_iActivePlayer);
    }
}

void CMainWindow::clearInspectorContainer()
{
    QLayout* inspectorContainerLayout = this->ui->inspectorContainer->layout();
    QLayoutItem *child;
    while ((child = inspectorContainerLayout->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }
    this->setInspectorName("");
}

void CMainWindow::goToSceneID(const QString &a_id, int a_iPlayerID, CThumbnailWidget* a_pClickedWidget)
{
    qDebug()<<"Deplacement vers la scene d'id : "<<a_id;
    if(a_iPlayerID == PLAYER_2)
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
    }
    LM::SEvent dummyEvent(LM::SEvent::NONE, nullptr, a_id.toStdString(), true, a_iPlayerID);
    ON_CC_THREAD(LM::CKernel::GotoScreenID, this->m_pKernel, dummyEvent, nullptr);
    //void GotoScreenID(SEvent a_rEvent, CEntityNode* a_pTarget);
    this->clearInspectorContainer();
}

void CMainWindow::goToNextScene()
{
    ON_CC_THREAD(LM::CKernel::NavNext, this->m_pKernel, nullptr, nullptr);
    this->clearInspectorContainer();
    this->setInspectorName("");
}

void CMainWindow::goToPreviousScene()
{
    ON_CC_THREAD(LM::CKernel::NavPrevious, this->m_pKernel, nullptr, nullptr);
    this->clearInspectorContainer();
    this->setInspectorName("");
}

void CMainWindow::addOneScene(const QString &a_sPreviousID, const QString &a_sNewID, int a_iPlayerID, CTemplate* a_pTemplate)
{
    ON_CC_THREAD(LM::CKernel::AddNewScene, m_pKernel, a_pTemplate->GetPath().toStdString(),
                 a_sPreviousID.toStdString(), a_sNewID.toStdString(), a_iPlayerID, 0, "");
}

void CMainWindow::addTwoScene(const QString &a_sPreviousIDP1, const QString &a_sNewIDP1,
                              const QString &a_sPreviousIDP2, const QString &a_sNewIDP2,
                              CTemplate* a_pTemplate)
{
    ON_CC_THREAD(LM::CKernel::AddNewScene, m_pKernel, a_pTemplate->GetPath().toStdString(),
                 a_sPreviousIDP1.toStdString(), a_sNewIDP1.toStdString(), PLAYER_1, 0, "");
    ON_CC_THREAD(LM::CKernel::AddNewScene, m_pKernel, a_pTemplate->GetPath().toStdString(),
                 a_sPreviousIDP2.toStdString(), a_sNewIDP2.toStdString(), PLAYER_2, 0, "");
}

void CMainWindow::addGameScene(const QString &a_sPreviousIDP1, const QString &a_sNewIDP1,
                               const QString &a_sPreviousIDP2, const QString &a_sNewIDP2,
                               CTemplate* a_pTemplate, int a_iTemplateNumberP1, int a_iTemplateNumberP2)
{
    ON_CC_THREAD(LM::CKernel::AddNewScene, m_pKernel, a_pTemplate->GetPath().toStdString(),
                 a_sPreviousIDP1.toStdString(), a_sNewIDP1.toStdString(), PLAYER_1, a_iTemplateNumberP1, a_sNewIDP2.toStdString());
    ON_CC_THREAD(LM::CKernel::AddNewScene, m_pKernel, a_pTemplate->GetPath().toStdString(),
                 a_sPreviousIDP2.toStdString(), a_sNewIDP2.toStdString(), PLAYER_2, a_iTemplateNumberP2, a_sNewIDP1.toStdString());
    ON_CC_THREAD(LM::CKernel::AddSyncID, m_pKernel, a_sNewIDP1.toStdString(), a_sNewIDP2.toStdString());
}

void CMainWindow::deleteScene(QString a_sSceneID, bool a_bIsSync)
{
    if(!a_bIsSync)
    {
        ON_CC_THREAD(LM::CKernel::DeleteScene, this->m_pKernel, a_sSceneID.toStdString());
    }
    else
    {
        ON_CC_THREAD(LM::CKernel::DeleteSyncScenes, this->m_pKernel, a_sSceneID.toStdString());
    }
}

void CMainWindow::launchEmulator()
{
    QString execPath = CProjectManager::Instance()->QGetInstallPath() + "/debug/emulator/LudoMuse.exe";
    QString cmd = execPath + " server " + CProjectManager::Instance()->QGetProjectJsonFile();
    m_oProcessServer.start(cmd);
    QThread::sleep(2);
    cmd = execPath + " client " + CProjectManager::Instance()->QGetProjectJsonFile();
    m_oProcessClient.start(cmd);
}

void CMainWindow::saveAs()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save File",
                               CProjectManager::Instance()->QGetProjectPath(),
                               "Scénarios Ludomuse (*.json)");
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

void CMainWindow::save()
{
    this->produceJson(this->m_sSaveName);
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
    if(m_pCurrentThumbnailWidget1 != Q_NULLPTR)
    {
        if(m_pCurrentThumbnailWidget2 != Q_NULLPTR)                           // selected screen for both timeline
        {
            pSceneWizard = new CAddSceneWizard(m_iActivePlayer,
                                               m_pKernel->GetSceneIDPlayer(PLAYER_1),
                                               m_pKernel->GetSceneIDPlayer(PLAYER_2),
                                               this,
                                               m_pCurrentThumbnailWidget1->GetSceneID(),
                                               m_pCurrentThumbnailWidget2->GetSceneID());
        }
        else                                                                 // Only selected on P1 time line
        {
            pSceneWizard = new CAddSceneWizard(m_iActivePlayer,
                                               m_pKernel->GetSceneIDPlayer(PLAYER_1),
                                               m_pKernel->GetSceneIDPlayer(PLAYER_2),
                                               this,
                                               m_pCurrentThumbnailWidget1->GetSceneID());
        }
    }
    else if(m_pCurrentThumbnailWidget2 != Q_NULLPTR)
    {
        pSceneWizard = new CAddSceneWizard(m_iActivePlayer,
                                           m_pKernel->GetSceneIDPlayer(PLAYER_1),
                                           m_pKernel->GetSceneIDPlayer(PLAYER_2),
                                           this,
                                           Q_NULLPTR,
                                           m_pCurrentThumbnailWidget2->GetSceneID());
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

void CMainWindow::addingSceneFinished()
{
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
    // Clear pointer on current thumbnail widget
    this->m_pCurrentThumbnailWidget1 = Q_NULLPTR;
    this->m_pCurrentThumbnailWidget2 = Q_NULLPTR;
    this->m_iActivePlayer = BOTH_PLAYER;
    this->ProcessTree();
}

void CMainWindow::deletingSceneFinished()
{
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
    // Clear pointer on current thumbnail widget
    this->m_pCurrentThumbnailWidget1 = Q_NULLPTR;
    this->m_pCurrentThumbnailWidget2 = Q_NULLPTR;
    this->m_iActivePlayer = BOTH_PLAYER;
    this->ProcessTree();
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
                        SIGNAL(onClick(const QString&, int, CThumbnailWidget*)),
                        this,
                        SLOT(goToSceneID(const QString&, int, CThumbnailWidget*)));
                connect(this->addSceneToTimeLine(sceneId, PLAYER_2),
                        SIGNAL(onClick(const QString&, int, CThumbnailWidget*)),
                        this,
                        SLOT(goToSceneID(const QString&, int, CThumbnailWidget*)));
            }
            else if(id == PLAYER_1)
            {
                connect(this->addSceneToTimeLine(sceneId, PLAYER_1),
                        SIGNAL(onClick(const QString&, int, CThumbnailWidget*)),
                        this,
                        SLOT(goToSceneID(const QString&, int, CThumbnailWidget*)));
            }
            else
            {
                connect(this->addSceneToTimeLine(sceneId, PLAYER_2),
                        SIGNAL(onClick(const QString&, int, CThumbnailWidget*)),
                        this,
                        SLOT(goToSceneID(const QString&, int, CThumbnailWidget*)));
            }
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

CThumbnailWidget* CMainWindow::addSceneToTimeLine(const QString &a_id, int a_playerID)
{

    if(a_playerID == PLAYER_1)
    {
        CThumbnailWidget* nodeWidget = new CThumbnailWidget(a_id, a_playerID, ui->trameContainerP1, index++);
        this->ui->trameContainerP1->layout()->addWidget(nodeWidget);
        return nodeWidget;
    }
    else if(a_playerID == PLAYER_2)
    {
        CThumbnailWidget* nodeWidget = new CThumbnailWidget(a_id, a_playerID, ui->trameContainerP2, 0);
        this->ui->trameContainerP2->layout()->addWidget(nodeWidget);
        return nodeWidget;
    }

    return new CThumbnailWidget(a_id, a_playerID);
}

void CMainWindow::InspectLabel(LM::CLabelNode* a_pLabel)
{
    // Clear inspector tool bar
    this->setInspectorName("Éditeur de texte");

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

    CLabelInspector* inspector = new CLabelInspector(a_pLabel);
    inspectorContainerLayout->addWidget(inspector);
    connect(inspector, SIGNAL(closeInspector()), this, SLOT(clearInspectorContainer()));
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
       && m_pKernel->PlayerHasScene(sceneId.toStdString(), 1)) //  P1 and P2
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
    inspectorTitle->setAlignment(Qt::AlignHCenter);
    inspectorTitle->setStyleSheet("QLabel{color : white;}");
    this->ui->toolBarInspector->layout()->addWidget(inspectorTitle);
}

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
}

void CMainWindow::on_fileBrowser_clicked(const QModelIndex &index)
{
    QString path = m_pDirModel->fileInfo(index).absoluteFilePath();
    ui->fileListView->setRootIndex(m_pFileModel->setRootPath(path));
    ui->fileListView->SetCurrentPath(path);
}
