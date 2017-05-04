#include "CPhotoPuzzleWizard.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QDrag>
#include <QMouseEvent>
#include <QMimeData>


#include "LudoMuse_src/Classes/Engine/Include/CSceneNode.h"
#include "LudoMuse_src/Classes/Engine/Include/CSpriteNode.h"
#include "LudoMuse_src/Classes/Engine/Include/CGridNode.h"
#include "LudoMuse_src/Classes/Engine/Include/CAnimationNode.h"
#include "LudoMuse_src/Classes/Engine/Include/CLabelNode.h"
#include "LudoMuse_src/Classes/Engine/Include/CMenuNode.h"


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
    qDebug()<< "Wizard ok";

    LM::CSceneNode* toFillPlayerScene;
    LM::CSceneNode* piecesPlayerScene;
    // generate scenes.
    if (!m_oNewGameInfo.isSwaped)
    {
        toFillPlayerScene = new LM::CSceneNode(m_oNewGameInfo.newID1.toStdString());
        piecesPlayerScene = new LM::CSceneNode(m_oNewGameInfo.newID2.toStdString());
    }
    else
    {
        toFillPlayerScene = new LM::CSceneNode(m_oNewGameInfo.newID2.toStdString());
        piecesPlayerScene = new LM::CSceneNode(m_oNewGameInfo.newID1.toStdString());
    }




    // fill scene1 with entities
    toFillPlayerScene->SetSynced(true);
    // backgournd image
    LM::CSpriteNode* backgroundImage = new LM::CSpriteNode("cache/background_dark.png",
                                                           LM::CENTER,
                                                           100);
    piecesPlayerScene->AddChildNode(backgroundImage);

    // drop area
    LM::CSpriteNode* dropArea = new LM::CSpriteNode("cache/shared_zone.png",
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
                                                  0, 80, 0, 0);
    piecesPlayerScene->AddChildNode(piecesGrid);



    // add information items to scene1
    LM::CSpriteNode* infoBottom = new LM::CSpriteNode("ui/info-6.png",
                                                      LM::EAnchor::BOTTOM,
                                                      0, 13);
    piecesPlayerScene->AddChildNode(infoBottom);


    LM::CGroupNode* infoBottomGroup = new LM::CGroupNode(LM::EAnchor::CENTER, 60, 100);
    infoBottom->AddChildNode(infoBottomGroup);

    LM::CAnimationNode* bottomMoveAnimation = new LM::CAnimationNode("ui/animations/move.plist",
                                                                     "ui/animations/move.png",
                                                                     LM::EAnchor::LEFT,
                                                                     0, 80);
    infoBottomGroup->AddChildNode(bottomMoveAnimation);

    LM::CLabelNode* infoBottomText = new LM::CLabelNode("Envoie les détails de la photo à ton partenaire",
                                                        "#default-font", 21, "center", "0,0,0,255",
                                                        LM::EAnchor::CENTER, 95);
    infoBottomGroup->AddChildNode(infoBottomText);



    LM::CSpriteNode* infoTop = new LM::CSpriteNode("ui/info-2.png",
                                                      LM::EAnchor::TOP,
                                                      0, 13);
    piecesPlayerScene->AddChildNode(infoTop);

    LM::CLabelNode* infoTopText = new LM::CLabelNode("Instructions Scénario",
                                                     "#default-font", 20, "center", "0,0,0,255",
                                                     LM::EAnchor::CENTER, 80, 100);
    infoTop->AddChildNode(infoTopText);


    LM::CMenuNode* navRight1 = new LM::CMenuNode("ui/nav-5.png",
                                                 "ui/nav-5-active.png",
                                                 LM::CCallback<LM::CKernel, cocos2d::Ref*>("Nav",
                                                                                           m_pKernel,
                                                                                           &LM::CKernel::NavNext),
                                                 LM::EAnchor::BOTTOM_RIGHT,
                                                 0, 13);

    piecesPlayerScene->AddChildNode(navRight1);

    LM::CEventCallback showNav("Show", m_pKernel, &LM::CKernel::SetNodeVisible,
                               LM::SEvent(LM::SEvent::BOOLEAN, navRight1, "Validate", true));
    navRight1->AddListener("Validate", showNav);

    LM::CLabelNode* navLabel = new LM::CLabelNode("Suivant",
                                                  "#default-font",
                                                  24, "center", "",
                                                  LM::EAnchor::CENTER, 100, 100);

    // fill scene2 with entities


}
