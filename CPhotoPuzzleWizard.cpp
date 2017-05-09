#include "CPhotoPuzzleWizard.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QDrag>
#include <QMouseEvent>
#include <QMimeData>
#include <QMessageBox>


#include "CMainWindow.h"
#include "CProjectManager.h"
#include "LudoMuse_src/Classes/Engine/Include/CSceneNode.h"
#include "LudoMuse_src/Classes/Engine/Include/CSpriteNode.h"
#include "LudoMuse_src/Classes/Engine/Include/CGridNode.h"
#include "LudoMuse_src/Classes/Engine/Include/CAnimationNode.h"
#include "LudoMuse_src/Classes/Engine/Include/CLabelNode.h"
#include "LudoMuse_src/Classes/Engine/Include/CMenuNode.h"
#include "LudoMuse_src/Classes/Engine/Include/CValidator.h"


CPhotoPuzzleWizard::CPhotoPuzzleWizard(const SNewGameInfo& a_rNewGame, LM::CKernel* a_pKernel, QWidget* parent):
    m_oNewGameInfo(a_rNewGame),
    m_pKernel(a_pKernel),
    QDialog(parent),
    ui(new Ui_Dialog)
{
    ui->setupUi(this);
    setModal(true);
    setAcceptDrops(true);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    connect(ui->okButton, SIGNAL(clicked(bool)), this, SLOT(clickOnValidate(bool)));
    connect(ui->cancelButton, SIGNAL(clicked(bool)), this, SLOT(clickOnCancel(bool)));

    connect(ui->ToFillColSpinner, SIGNAL(valueChanged(int)), this, SLOT(GenerateGrids()));
    connect(ui->ToFillRowsSpinner, SIGNAL(valueChanged(int)), this, SLOT(GenerateGrids()));
    connect(ui->PiecesColSpinner, SIGNAL(valueChanged(int)), this, SLOT(GenerateGrids()));
    connect(ui->PiecesRowsSpinner, SIGNAL(valueChanged(int)), this, SLOT(GenerateGrids()));

    GenerateGrids();
}


void CPhotoPuzzleWizard::ClearLayout(QLayout* layout, bool deleteWidgets)
{
    while (QLayoutItem* item = layout->takeAt(0))
    {
        if (deleteWidgets)
        {
            if (QWidget* widget = item->widget())
                delete widget;
        }
        if (QLayout* childLayout = item->layout())
            ClearLayout(childLayout, deleteWidgets);
        delete item;
    }
}


void CPhotoPuzzleWizard::GenerateGrids()
{
    ClearLayout(ui->ToFillGridLayout, true);
    ClearLayout(ui->PiecesGridLayout, true);
    m_vPiecesGridItems.clear();
    m_vToFillGridItems.clear();

    ui->PiecesColSpinner->setMinimum(1);
    ui->PiecesRowsSpinner->setMinimum(1);
    ui->PiecesColSpinner->setMaximum(ui->ToFillColSpinner->value());
    ui->PiecesRowsSpinner->setMaximum(ui->ToFillRowsSpinner->value());

    ui->ToFillColSpinner->setMinimum(1);
    ui->ToFillRowsSpinner->setMinimum(1);
    ui->ToFillColSpinner->setMaximum(5);
    ui->ToFillRowsSpinner->setMaximum(5);

    for (int row = 0; row < ui->ToFillRowsSpinner->value(); ++row)
    {
        for (int col = 0; col < ui->ToFillColSpinner->value(); ++col)
        {
            QLabel* label = new QLabel(this);
            label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            label->setMinimumHeight(20);
            label->setMinimumWidth(20);
            label->setText(QString::number(row * ui->ToFillColSpinner->value() + col + 1));

            if (row >= m_vToFillGridItems.size())
            {
                m_vToFillGridItems.append(QVector<QLabel*>());
            }

            m_vToFillGridItems[row].append(label);

            ui->ToFillGridLayout->addWidget(label, row, col);
        }
    }

    for (int row = 0; row < ui->PiecesRowsSpinner->value(); ++row)
    {
        for (int col = 0; col < ui->PiecesColSpinner->value(); ++col)
        {
            QComboBox* combo = new QComboBox(this);
            combo->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

            combo->setAcceptDrops(true);

            for (int i = 0; i < m_vToFillGridItems.size() * m_vToFillGridItems.at(0).size(); ++i)
            {
                combo->addItem(QString::number(i + 1));
            }

            if (row >= m_vPiecesGridItems.size())
            {
                m_vPiecesGridItems.append(QVector<QComboBox*>());
            }

            m_vPiecesGridItems[row].append(combo);

            ui->PiecesGridLayout->addWidget(combo, row, col);
        }
    }
}


void CPhotoPuzzleWizard::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {

        QLabel* label = dynamic_cast<QLabel*>(childAt(event->pos()));
        if (label && label->text() != "_")
        {
            QDrag* drag = new QDrag(this);
            QMimeData* mimeData = new QMimeData;
            mimeData->setText(label->text());
            drag->setMimeData(mimeData);

            drag->exec();
        }
    }
}


void CPhotoPuzzleWizard::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain"))
        event->acceptProposedAction();
}

void CPhotoPuzzleWizard::dropEvent(QDropEvent *event)
{
    QComboBox* combo = dynamic_cast<QComboBox*>(childAt(event->pos()));
    if (combo)
    {
        combo->setCurrentIndex(event->mimeData()->text().toInt() - 1);

        event->acceptProposedAction();
    }
}


void CPhotoPuzzleWizard::clickOnCancel(bool)
{
    qDebug() << "cancel";
    close();
}

void CPhotoPuzzleWizard::clickOnValidate(bool)
{

    std::vector<int> values;
    for (int row = ui->PiecesRowsSpinner->value() - 1; row >= 0; --row)
    {
        for (int col = 0; col < ui->PiecesColSpinner->value(); ++col)
        {
            values.push_back(m_vPiecesGridItems[row][col]->currentIndex() + 1);
        }
    }
    for (int i = 0; i < values.size(); ++i)
    {
        for (int j = 0; j < values.size(); ++j)
        {
            if (i != j && values[i] == values[j])
            {
                // two identical values
                QMessageBox::critical(this, "Erreur !",
                                      "Impossible de créer le jeu vous avez plusieurs pièces référençant le même élement manquant");
                return;
            }
        }
    }


    ON_CC_THREAD(CPhotoPuzzleWizard::GenerateScenes, this, values);
}

void CPhotoPuzzleWizard::GenerateScenes(const std::vector<int>& values)
    {


    std::string toFillSceneID;
    std::string piecesSceneID;



    LM::CSceneNode *sceneP1, *sceneP2;
    if (!m_oNewGameInfo.isSwaped)
    {
        toFillSceneID = m_oNewGameInfo.newID1.toStdString();
        piecesSceneID = m_oNewGameInfo.newID2.toStdString();
    }
    else
    {
        toFillSceneID = m_oNewGameInfo.newID2.toStdString();
        piecesSceneID = m_oNewGameInfo.newID1.toStdString();
    }

    LM::CSceneNode* toFillPlayerScene = new LM::CSceneNode(toFillSceneID);
    LM::CSceneNode* piecesPlayerScene = new LM::CSceneNode(piecesSceneID);

    if (!m_oNewGameInfo.isSwaped)
    {
        sceneP1 = toFillPlayerScene;
        sceneP2 = piecesPlayerScene;
    }
    else
    {
        sceneP1 = piecesPlayerScene;
        sceneP2 = toFillPlayerScene;
    }


    std::string basePath = CProjectManager::Instance()->GetProjectPath();

    // fill scene1 with entities
    piecesPlayerScene->SetSynced(true);
    // backgournd image
    LM::CSpriteNode* backgroundImage = new LM::CSpriteNode(basePath + "cache/background_dark.png",
                                                           LM::CENTER,
                                                           100);
    piecesPlayerScene->AddChildNode(backgroundImage);

    // drop area
    LM::CSpriteNode* dropArea = new LM::CSpriteNode(basePath + "cache/shared_zone.png",
                                                    LM::LEFT, 25);
    std::string dropArea1ID = "dropArea1";
    dropArea->SetID(dropArea1ID);

    LM::CEventCallback dropOnArea("Drop", m_pKernel, nullptr);
    dropArea->AddListener("Drop", dropOnArea);
    LM::CEventCallback dropEnableBack("DropEnableBack",
                                      m_pKernel,
                                      &LM::CKernel::EnableEvent,
                                      LM::SEvent(LM::SEvent::STRING, dropArea, dropArea1ID + ":Drop"));
    dropArea->AddListener("DropEnableBack", dropEnableBack);

    piecesPlayerScene->AddChildNode(dropArea);


    LM::CGridNode* piecesGrid = new LM::CGridNode(ui->PiecesRowsSpinner->value(),
                                                  ui->PiecesColSpinner->value(),
                                                  LM::EAnchor::RIGHT,
                                                  50, 50, 0, 0);
    piecesPlayerScene->AddChildNode(piecesGrid);
    // fill this grid in the dynamic filling loop at the end



    // add information items to scene1
    LM::CSpriteNode* infoBottom = new LM::CSpriteNode(basePath + "ui/info-6.png",
                                                      LM::EAnchor::BOTTOM,
                                                      0, 13);
    piecesPlayerScene->AddChildNode(infoBottom);


    LM::CGroupNode* infoBottomGroup = new LM::CGroupNode(LM::EAnchor::CENTER, 60, 100);
    infoBottom->AddChildNode(infoBottomGroup);

    LM::CAnimationNode* bottomMoveAnimation = new LM::CAnimationNode(basePath + "ui/animations/move.png",
                                                                     basePath + "ui/animations/move.plist",
                                                                     LM::EAnchor::LEFT,
                                                                     0, 80);
    infoBottomGroup->AddChildNode(bottomMoveAnimation);

    LM::CLabelNode* infoBottomText = new LM::CLabelNode("Envoie les détails de la photo à ton partenaire",
                                                        "#default-font", 21, "center", "0,0,0,255",
                                                        LM::EAnchor::CENTER, 95);
    infoBottomGroup->AddChildNode(infoBottomText);



    LM::CSpriteNode* infoTop = new LM::CSpriteNode(basePath + "ui/info-2.png",
                                                      LM::EAnchor::TOP,
                                                      0, 13);
    piecesPlayerScene->AddChildNode(infoTop);

    LM::CLabelNode* infoTopText = new LM::CLabelNode("Instructions Scénario",
                                                     "#default-font", 20, "center", "0,0,0,255",
                                                     LM::EAnchor::CENTER, 80, 100);
    infoTop->AddChildNode(infoTopText);


    LM::CMenuNode* navRight1 = new LM::CMenuNode(basePath + "ui/nav-5.png",
                                                 basePath + "ui/nav-5-active.png",
                                                 LM::CCallback<LM::CKernel, cocos2d::Ref*>("Nav",
                                                                                           m_pKernel,
                                                                                           &LM::CKernel::NavNext),
                                                 LM::EAnchor::BOTTOM_RIGHT,
                                                 0, 13, 0, 0, "next");

    piecesPlayerScene->AddChildNode(navRight1);

    LM::CEventCallback showNav("Show", m_pKernel, &LM::CKernel::SetNodeVisible,
                               LM::SEvent(LM::SEvent::BOOLEAN, navRight1, "Validate", true));
    navRight1->AddListener("Validate", showNav);

    LM::CLabelNode* navLabel = new LM::CLabelNode("Suivant",
                                                  "#default-font",
                                                  24, "center", "",
                                                  LM::EAnchor::CENTER, 100, 100);
    navRight1->AddChildNode(navLabel);



    // fill scene2 with entities
    toFillPlayerScene->SetSynced(true);
    LM::CValidator* validator = new LM::CValidator(m_pKernel);
    for (int value : values)
    {
        validator->AddID(std::to_string(value));
    }

    toFillPlayerScene->AddChildNode(validator);

    LM::CSpriteNode* backgroundToFill = new LM::CSpriteNode(basePath + "cache/background_dark.png",
                                                            LM::EAnchor::CENTER, 100, 100);
    toFillPlayerScene->AddChildNode(backgroundToFill);

    LM::CSpriteNode* photoPuzzle = new LM::CSpriteNode(basePath + "cache/background.png",
                                                       LM::EAnchor::LEFT, 70);
    toFillPlayerScene->AddChildNode(photoPuzzle);

    LM::CGridNode* toFillGrid = new LM::CGridNode(ui->ToFillRowsSpinner->value(),
                                                  ui->ToFillColSpinner->value(),
                                                  LM::EAnchor::CENTER,
                                                  100, 100, 0, 0);
    photoPuzzle->AddChildNode(toFillGrid);
    // fill this grid in the dynamic filling loop at the end


    LM::CSpriteNode* dropAreaToFill = new LM::CSpriteNode(basePath + "cache/shared_zone.png",
                                                          LM::EAnchor::RIGHT,
                                                          25, 100);
    toFillPlayerScene->AddChildNode(dropAreaToFill);
    // fill this area in the dynamic filling loop at the end


    LM::CSpriteNode* infoBottomToFill = new LM::CSpriteNode(basePath + "ui/info-6.png",
                                                            LM::EAnchor::BOTTOM,
                                                            0, 13);
    toFillPlayerScene->AddChildNode(infoBottomToFill);

    LM::CGroupNode* infoBottomToFillGroup = new LM::CGroupNode(LM::EAnchor::CENTER,
                                                               60, 100);
    infoBottomToFill->AddChildNode(infoBottomToFillGroup);

    LM::CAnimationNode* animationToFill = new LM::CAnimationNode(basePath + "ui/animations/move.png",
                                                                 basePath + "ui/animations/move.plist",
                                                                 LM::EAnchor::LEFT,
                                                                 0, 80);
    infoBottomToFillGroup->AddChildNode(animationToFill);

    LM::CLabelNode* infoBottomToFillText = new LM::CLabelNode("Recompose la photo à partir des détails envoyés.",
                                                              "#default-font",
                                                              21, "center", "0,0,0,255",
                                                              LM::EAnchor::RIGHT, 95);
    infoBottomToFillGroup->AddChildNode(infoBottomToFillText);

    LM::CSpriteNode* infoTopToFill = new LM::CSpriteNode(basePath + "ui/info-2.png",
                                                   LM::EAnchor::TOP,
                                                   0, 13);
    toFillPlayerScene->AddChildNode(infoTopToFill);

    LM::CLabelNode* infoTopToFillText = new LM::CLabelNode("Instructions scénario",
                                                           "#default-font",
                                                           20, "center", "0,0,0,255",
                                                           LM::EAnchor::CENTER,
                                                           80, 100);
    infoTopToFill->AddChildNode(infoTopToFillText);


    LM::CMenuNode* navRight2 = new LM::CMenuNode(basePath + "ui/nav-5.png",
                                                 basePath + "ui/nav-5-active.png",
                                                 LM::CCallback<LM::CKernel, cocos2d::Ref*>("Nav",
                                                                                           m_pKernel,
                                                                                           &LM::CKernel::NavNext),
                                                 LM::EAnchor::BOTTOM_RIGHT,
                                                 0, 13, 0, 0, "next");

    toFillPlayerScene->AddChildNode(navRight2);

    LM::CEventCallback showNav2("Show", m_pKernel, &LM::CKernel::SetNodeVisible,
                               LM::SEvent(LM::SEvent::BOOLEAN, navRight2, "Validate", true));
    navRight2->AddListener("Validate", showNav2);

    LM::CLabelNode* navLabel2 = new LM::CLabelNode("Suivant",
                                                  "#default-font",
                                                  24, "center", "",
                                                  LM::EAnchor::CENTER, 100, 100);
    navRight2->AddChildNode(navLabel2);



    // link grid elements to each other and populate them
    for (int toFillRow = ui->ToFillRowsSpinner->value() - 1; toFillRow >= 0; --toFillRow)
    {
        for (int toFillCol = 0; toFillCol < ui->ToFillColSpinner->value(); ++toFillCol)
        {
            LM::CGroupNode* toFillCell = new LM::CGroupNode(LM::EAnchor::CENTER,
                                                            100, 100);

            LM::CEventCallback sendOndropOnCell("SendMessage", m_pKernel, &LM::CKernel::SendNetworkMessage,
                                          LM::SEvent(LM::SEvent::STRING, toFillCell,
                                                     piecesSceneID + ":DropEnableBack:" + dropArea1ID));
            toFillCell->AddListener("Drop", sendOndropOnCell);

            LM::CEventCallback anchorOnDropOnCell("AnchorEntity", m_pKernel, &LM::CKernel::AnchorEntityCallback,
                                                  LM::SEvent(LM::SEvent::STRING, toFillCell,
                                                             m_vToFillGridItems[toFillRow][toFillCol]->text().toStdString()));
            toFillCell->AddListener("Drop", anchorOnDropOnCell);

            toFillGrid->AddChildNode(toFillCell);
        }
    }

    for (int value : values)
    {

        // pieces in pieces grid
        LM::CSpriteNode* piece = new LM::CSpriteNode(basePath + "cache/puzzle.png",
                                                     LM::EAnchor::CENTER,
                                                     0, 80);

        LM::CEventCallback fadeOnDroped("Fade", m_pKernel, &LM::CKernel::FadeEntity,
                                      LM::SEvent(LM::SEvent::NONE, piece));
        piece->AddListener("Droped", fadeOnDroped);

        LM::CEventCallback disableAreaOnDroped("DisableEvent", m_pKernel,
                                               &LM::CKernel::DisableEvent,
                                               LM::SEvent(LM::SEvent::STRING, piece, dropArea1ID + ":drop"));
        piece->AddListener("Droped", disableAreaOnDroped);

        LM::CEventCallback sendMessageOnDroped("SendMessage", m_pKernel,
                                               &LM::CKernel::SendNetworkMessage,
                                               LM::SEvent(LM::SEvent::STRING, piece, toFillSceneID + std::string(":Show:") + std::to_string(value)));
        piece->AddListener("Droped", sendMessageOnDroped);

        LM::CEventCallback dummy("Move", m_pKernel, nullptr);
        piece->AddListener("Move", dummy);

        LM::CEventCallback showBackOnReturn("ShowBack", m_pKernel,
                                            &LM::CKernel::SetNodeVisible,
                                            LM::SEvent(LM::SEvent::BOOLEAN, piece, "ShowBack", true));
        piece->AddListener("ReturnElement", showBackOnReturn);

        piece->SetID(std::to_string(value));
        piecesGrid->AddChildNode(piece);


        // pieces in sending area
        LM::CSpriteNode* fill = new LM::CSpriteNode(basePath + "cache/puzzle.png",
                                                    LM::EAnchor::CENTER,
                                                    40);
        LM::CEventCallback validateOnAnchored("Validate", m_pKernel,
                                              &LM::CKernel::Validate,
                                              LM::SEvent(LM::SEvent::STRING, fill, std::to_string(value)));
        fill->AddListener("Anchored", validateOnAnchored);

        LM::CEventCallback fadeOnAnchoredFailed("Fade", m_pKernel,
                                                &LM::CKernel::FadeEntity,
                                                LM::SEvent(LM::SEvent::NONE, fill));
        fill->AddListener("AnchoredFailed", fadeOnAnchoredFailed);


        LM::CEventCallback sendMessageOnFailed("SendMessage", m_pKernel,
                                               &LM::CKernel::SendNetworkMessage,
                                               LM::SEvent(LM::SEvent::STRING, fill, piecesSceneID + std::string(":ReturnElement:") + std::to_string(value)));
        fill->AddListener("AnchoredFailed", sendMessageOnFailed);

        LM::CEventCallback localMessageOnFailed("LocalMessage", m_pKernel,
                                                &LM::CKernel::LocalMessage,
                                                LM::SEvent(LM::SEvent::STRING, fill, toFillSceneID + std::string(":Beep:") + std::to_string(value)));
        fill->AddListener("AnchoredFailed", localMessageOnFailed);

        LM::CEventCallback dummy2("Move", m_pKernel, nullptr);
        fill->AddListener("Move", dummy2);

        LM::CEventCallback show("Show", m_pKernel,
                                &LM::CKernel::SetNodeVisible,
                                LM::SEvent(LM::SEvent::BOOLEAN, fill, "Show", true));
        fill->AddListener("Show", show);

        LM::CEventCallback beep("PlaySound", m_pKernel,
                                &LM::CKernel::PlaySoundCallback,
                                LM::SEvent(LM::SEvent::STRING, fill, basePath + "ui/audio/beep.wav"));
        fill->AddListener("Beep", beep);

        fill->SetVisible(false);

        fill->SetID(std::to_string(value));
        dropAreaToFill->AddChildNode(fill);
    }


    m_pKernel->AddSyncID(m_oNewGameInfo.newID1.toStdString(), m_oNewGameInfo.newID2.toStdString());
    m_pKernel->AddScene(sceneP1,
                        m_oNewGameInfo.previousID1.toStdString(),
                        m_oNewGameInfo.newID1.toStdString(),
                        CMainWindow::PLAYER_1);

    m_pKernel->AddScene(sceneP2,
                        m_oNewGameInfo.previousID2.toStdString(),
                        m_oNewGameInfo.newID2.toStdString(),
                        CMainWindow::PLAYER_2);

    close();
}
