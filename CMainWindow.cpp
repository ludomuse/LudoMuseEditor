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

// Include QT
#include <QtWidgets>
#include <QThread>
#include <QProcess>
#include <QBoxLayout>
#include <QLayoutItem>
#include <QStringList>

// Include cocos
#include "cocos2d.h"

// Include LudoMuse
#include "Classes/Engine/Include/CNode.h"
#include "Classes/Engine/Include/CSceneNode.h"
#include "Classes/Engine/Include/CSequenceNode.h"
#include "Classes/Engine/Include/CCallback.h"
#include "Classes/Engine/Include/CEditorFindEntityTouchVisitor.h"
#include "Classes/Modules/Util/Include/Util.h"

// Test json include
#include "rapidjson.h"
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
#include "prettywriter.h"


CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CMainWindow),
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

    // Icon creation
    ui->playlistButton->setIcon(QIcon("resources/add_playlist_w.png"));
    ui->playButton->setIcon(QIcon("resources/play_arrow.png"));
    ui->emulateButton->setIcon(QIcon("resources/emulate.png"));

    // Connect Tool bar
    connect(ui->prevScreenButton, SIGNAL(clicked(bool)), this, SLOT(goToPreviousScene()));
    connect(ui->nextScreenButton, SIGNAL(clicked(bool)), this, SLOT(goToNextScene()));
    connect(ui->emulateButton, SIGNAL(clicked(bool)), this, SLOT(launchEmulator()));
    connect(ui->JsonGo, SIGNAL(clicked(bool)), this, SLOT(produceJson()));

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
}

void CMainWindow::goToSceneID(const QString &a_id, int a_iPlayerID)
{
    qDebug()<<"Deplacement vers la scene d'id : "<<a_id;
    LM::SEvent dummyEvent(nullptr, a_id.toStdString(), true, a_iPlayerID);
    ON_CC_THREAD(LM::CKernel::GotoScreenID, this->m_pKernel, dummyEvent, nullptr);
    //void GotoScreenID(SEvent a_rEvent, CEntityNode* a_pTarget);
}

void CMainWindow::goToNextScene()
{
    ON_CC_THREAD(LM::CKernel::NavNext, this->m_pKernel, nullptr, nullptr);
}

void CMainWindow::goToPreviousScene()
{
    ON_CC_THREAD(LM::CKernel::NavPrevious, this->m_pKernel, nullptr, nullptr);
}

void CMainWindow::launchEmulator()
{
    m_oProcessServer.start("emulator\\LudoMuse.exe");
    QThread::sleep(2);
    m_oProcessClient.start("emulator\\LudoMuse.exe", QStringList()<<"client");
}

void CMainWindow::produceJson(){
    QString jsonResult;
    this->ui->jsonDisplayer->setText(this->m_pKernel->ToJson().c_str());
//    rapidjson::StringBuffer s;
//    rapidjson::Writer<rapidjson::StringBuffer> writer(s);
//    writer.StartObject();
//    writer.Key("hello");
//    writer.String("world");
//    writer.Key("t");
//    writer.Bool(true);
//    writer.Key("f");
//    writer.Bool(false);
//    writer.Key("n");
//    writer.Null();
//    writer.Key("i");
//    writer.Uint(123);
//    writer.Key("pi");
//    writer.Double(3.1416);
//    writer.Key("a");
//    writer.StartArray();
//    for (unsigned i = 0; i < 4; i++)
//    {
//        writer.Uint(i);
//    }
//    writer.EndArray();
//    writer.EndObject();
//    qDebug() << s.GetString() << endl;

//    this->ui->jsonDisplayer->setText(jsonResult);
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
                        SIGNAL(onClick(const QString&, int)),
                        this,
                        SLOT(goToSceneID(const QString&, int)));
                connect(this->addSceneToTimeLine(sceneId, PLAYER_2),
                        SIGNAL(onClick(const QString&, int)),
                        this,
                        SLOT(goToSceneID(const QString&, int)));
            }
            else if(id == PLAYER_1)
            {
                connect(this->addSceneToTimeLine(sceneId, PLAYER_1),
                        SIGNAL(onClick(const QString&, int)),
                        this,
                        SLOT(goToSceneID(const QString&, int)));
            }
            else
            {
                connect(this->addSceneToTimeLine(sceneId, PLAYER_2),
                        SIGNAL(onClick(const QString&, int)),
                        this,
                        SLOT(goToSceneID(const QString&, int)));
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
    QLabel* inspectorTitle = new QLabel("Editeur de label");
    inspectorTitle->setAlignment(Qt::AlignHCenter);
    inspectorTitle->setStyleSheet("QLabel{color : white;}");
    this->ui->toolBarInspector->layout()->addWidget(inspectorTitle);

    // Clear inspector loayout from older inspection
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
    QLabel* inspectorTitle = new QLabel("Editeur de sprite");
    inspectorTitle->setAlignment(Qt::AlignHCenter);
    inspectorTitle->setStyleSheet("QLabel{color : white;}");
    this->ui->toolBarInspector->layout()->addWidget(inspectorTitle);

    // Clear inspector loayout from older inspection
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
