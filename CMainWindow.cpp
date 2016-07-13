#include <GL/glew.h>
#include <WinSock2.h>
#include <vector>

// Include custom classes
#include "CMainWindow.h"
#include "ui_cmainwindow.h"
#include "CThreadCocos.h"
#include "CThumbnailWidget.h"
#include "CLabelInspector.h"
#include "CSpriteInspector.h"
#include "CAddSceneWizard.h"

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
#include "Classes/Engine/Include/CCallback.h"
#include "Classes/Engine/Include/CEditorFindEntityTouchVisitor.h"
#include "Classes/Modules/Util/Include/Util.h"



CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CMainWindow),
    m_pCurrentThumbnailWidget1(Q_NULLPTR),
    m_pCurrentThumbnailWidget2(Q_NULLPTR),
    m_iActivePlayer(0),
    m_pKernel(NULL)
{
/*   atm useless code */
    QThread* thread = new QThread;
    CThreadCocos* worker = new CThreadCocos();
    worker->moveToThread(thread);
    //connect(worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(worker, SIGNAL(sendHWND(int)), this, SLOT(receiveHWND(int)));
    connect(worker, SIGNAL(sendKernel(LM::CKernel*)), this, SLOT(receiveKernel(LM::CKernel*)));
    connect(thread, SIGNAL(started()), worker, SLOT(process()));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();

    ui->setupUi(this);

    // File browser All path will be changed TODO
    QString temporaryPath("D:\\IHMTEK\\LudoMuseEditorCocos\\build-LudoMuseEditor-Clone_de_Desktop_Qt_5_6_0_MSVC2015_32bit-Debug\\debug");
    m_pDirModel = new QFileSystemModel();
    m_pDirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    m_pDirModel->setRootPath(temporaryPath);
    ui->fileBrowser->setModel(m_pDirModel);
    ui->fileBrowser->setRootIndex(m_pDirModel->index(temporaryPath));
    ui->fileBrowser->setColumnHidden( 1, true );
    ui->fileBrowser->setColumnHidden( 2, true );
    ui->fileBrowser->setColumnHidden( 3, true );

    m_pFileModel = new QFileSystemModel(this);
    m_pFileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    ui->fileListView->setModel(m_pFileModel);
    ui->fileListView->setRootIndex(m_pFileModel->setRootPath(temporaryPath));
    ui->fileListView->SetCurrentPath(temporaryPath);


    // Icon creation
    ui->playlistButton->setIcon(QIcon("resources/add_playlist_w.png"));
    ui->playButton->setIcon(QIcon("resources/play_arrow.png"));
    ui->emulateButton->setIcon(QIcon("resources/emulate.png"));

    // Connect Tool bar
    connect(ui->prevScreenButton, SIGNAL(clicked(bool)), this, SLOT(goToPreviousScene()));
    connect(ui->nextScreenButton, SIGNAL(clicked(bool)), this, SLOT(goToNextScene()));
    connect(ui->emulateButton, SIGNAL(clicked(bool)), this, SLOT(launchEmulator()));
    connect(ui->JsonGo, SIGNAL(clicked(bool)), this, SLOT(produceJson()));
    connect(ui->lmwTestButton, SIGNAL(clicked(bool)), this, SLOT(launchAddSceneWizard(bool)));

    this->showMaximized();
}

CMainWindow::~CMainWindow()
{
    delete ui;
}


void CMainWindow::machin()
{

}

void CMainWindow::on_bugButton_clicked()
{
    qInfo( "Click on bug Button!" );
}

void CMainWindow::on_playlistButton_clicked()
{
    qInfo( "Click on playList button!" );
    //ui->glView_1->test();
}

void CMainWindow::on_playButton_clicked()
{
    qInfo("Click on play Button! - process behaviourTree");
    //this->ProcessTree();
}

void CMainWindow::on_lmwTestButton_clicked()
{

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

    this->ProcessTree();
}

void CMainWindow::receiveLabel(LM::CLabelNode* a_pLabel)
{
    //qDebug("Reception d'un Label");
    this->inspectLabel(a_pLabel);
}

void CMainWindow::receiveSprite(LM::CSpriteNode* a_pSprite)
{
    //qDebug("Reception d'un Sprite");
    this->inspectSprite(a_pSprite);
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
        if(m_pCurrentThumbnailWidget2 == Q_NULLPTR) // First click
        {
            m_pCurrentThumbnailWidget2 = a_pClickedWidget;
        }
        else
        {
            m_pCurrentThumbnailWidget2->unselect();
            m_pCurrentThumbnailWidget2 = a_pClickedWidget;
        }
        // Change color pfor other player timeline
        if(m_pCurrentThumbnailWidget1 != Q_NULLPTR)
        {
            m_pCurrentThumbnailWidget1->LastActive();
        }
        m_iActivePlayer = PLAYER_2;
    }
    else // By default take player one
    {
        if(m_pCurrentThumbnailWidget1 == Q_NULLPTR) // First click
        {
            m_pCurrentThumbnailWidget1 = a_pClickedWidget;
        }
        else
        {
            m_pCurrentThumbnailWidget1->unselect();
            m_pCurrentThumbnailWidget1 = a_pClickedWidget;
        }
        // Change color pfor other player timeline
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


void CMainWindow::addSceneTemplate(const QString& a_sPreviousID,const QString& a_sNewID, int a_iPlayerID, int a_iTemplateNumber)
{
    qDebug()<<"insertion du template : "<< a_iTemplateNumber<< " avec l'id : "<< a_sNewID << "Aprés l'écran : " << a_sPreviousID << "Pour le joueur : " << a_iPlayerID;
    //ON_CC_THREAD(LM::CKernel::AddNewScene, this->m_pKernel,"test.json","test", a_sNewID, a_iPlayerID);
    ON_CC_THREAD(LM::CKernel::AddNewScene, m_pKernel, "test.json", a_sPreviousID.toStdString(), a_sNewID.toStdString(), a_iPlayerID)
}

void CMainWindow::launchEmulator()
{
    m_oProcessServer.start("emulator\\LudoMuse.exe");
    QThread::sleep(2);
    m_oProcessClient.start("emulator\\LudoMuse.exe", QStringList()<<"client");
}

void CMainWindow::produceJson(){
    QString jsonResult;
    QString filename="result.json";
    QFile file( filename );
    if ( file.open(QIODevice::ReadWrite) )
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

void CMainWindow::launchAddSceneWizard(bool)
{
    CAddSceneWizard* pSceneWizard;
    if(m_pCurrentThumbnailWidget1 != Q_NULLPTR)
    {
        if(m_pCurrentThumbnailWidget2 != Q_NULLPTR)                           // selected screen for both timeline
        {
            pSceneWizard = new CAddSceneWizard(m_iActivePlayer,
                                               m_pKernel->GetSceneIDPlayer(0),
                                               m_pKernel->GetSceneIDPlayer(1),
                                               this,
                                               m_pCurrentThumbnailWidget1->getSceneID(),
                                               m_pCurrentThumbnailWidget2->getSceneID());
        }
        else                                                                 // Only selected on P1 time line
        {
            pSceneWizard = new CAddSceneWizard(m_iActivePlayer,
                                               m_pKernel->GetSceneIDPlayer(0),
                                               m_pKernel->GetSceneIDPlayer(1),
                                               this,
                                               m_pCurrentThumbnailWidget1->getSceneID());
        }
    }
    else if(m_pCurrentThumbnailWidget2 != Q_NULLPTR)
    {
        pSceneWizard = new CAddSceneWizard(m_iActivePlayer,
                                           m_pKernel->GetSceneIDPlayer(0),
                                           m_pKernel->GetSceneIDPlayer(1),
                                           this,
                                           Q_NULLPTR,
                                           m_pCurrentThumbnailWidget2->getSceneID());
    }
    else                                                                    // No screen selected on both timeline
    {
        pSceneWizard = new CAddSceneWizard(m_iActivePlayer,
                                           m_pKernel->GetSceneIDPlayer(0),
                                           m_pKernel->GetSceneIDPlayer(1),
                                           this);
    }
    connect(pSceneWizard, SIGNAL(validate(QString,QString,int,int)), this, SLOT(addSceneTemplate(QString,QString,int,int)));
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
    this->m_iActivePlayer = 0;
    this->ProcessTree();
}



void CMainWindow::ProcessTree()
{
    LM::CNode *mainNode = this->m_pKernel->GetBehaviorTree();
    std::vector<LM::CNode*> allScenes = mainNode->GetChildren();
    int i = 0;
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
    if (m_pKernel->PlayerHasScene(a_id.toStdString(), 0))
    {
        if(m_pKernel->PlayerHasScene(a_id.toStdString(), 1))
        {
            return BOTH_PLAYER;
        }
        return PLAYER_1;
    }
    return PLAYER_2;
}

CThumbnailWidget* CMainWindow::addSceneToTimeLine(const QString &a_id, int a_playerID)
{
    CThumbnailWidget *nodeWidget = new CThumbnailWidget(a_id, a_playerID);

    if(a_playerID == PLAYER_1)
    {
        this->ui->trameContainerP1->layout()->addWidget(nodeWidget);
    }
    else if(a_playerID == PLAYER_2)
    {
        this->ui->trameContainerP2->layout()->addWidget(nodeWidget);
    }
    return nodeWidget;
}

void CMainWindow::inspectLabel(LM::CLabelNode* a_pLabel)
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

void CMainWindow::inspectSprite(LM::CSpriteNode* a_pSprite)
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

void CMainWindow::on_fileBrowser_clicked(const QModelIndex &index)
{
    QString path = m_pDirModel->fileInfo(index).absoluteFilePath();
    ui->fileListView->setRootIndex(m_pFileModel->setRootPath(path));
    ui->fileListView->SetCurrentPath(path);
}
