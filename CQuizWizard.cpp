#include "CQuizWizard.h"
#include "ui_CQuizWizard.h"

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

CQuizWizard::CQuizWizard(const SNewGameInfo &a_rNewGame, LM::CKernel *a_pKernel, QWidget *parent):
    QDialog(parent),
    ui(new Ui::CQuizWizard),
    m_oNewGameInfo(a_rNewGame),
    m_pKernel(a_pKernel)
{
    ui->setupUi(this);
    setModal(true);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    connect(ui->okButton, SIGNAL(clicked(bool)), this, SLOT(clickOnValidate(bool)));
    connect(ui->cancelButton, SIGNAL(clicked(bool)), this, SLOT(clickOnCancel(bool)));

    connect(ui->Player1ColsSpinner, SIGNAL(valueChanged(int)), this, SLOT(GenerateGrids()));
    connect(ui->Player1RowsSpinner, SIGNAL(valueChanged(int)), this, SLOT(GenerateGrids()));
    connect(ui->Player2ColsSpinner, SIGNAL(valueChanged(int)), this, SLOT(GenerateGrids()));
    connect(ui->Player2RowsSpinner, SIGNAL(valueChanged(int)), this, SLOT(GenerateGrids()));

    ui->Player1ColsSpinner->setMinimum(1);
    ui->Player1RowsSpinner->setMinimum(1);

    ui->Player2ColsSpinner->setMinimum(1);
    ui->Player2ColsSpinner->setMinimum(1);

    GenerateGrids();
}

CQuizWizard::~CQuizWizard()
{
    delete ui;
}


void CQuizWizard::ClearLayout(QLayout* layout, bool deleteWidgets)
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


void CQuizWizard::GenerateGrids()
{
    ClearLayout(ui->Player1GridLayout, true);
    ClearLayout(ui->Player2GridLayout, true);
    m_vPlayer1GridItems.clear();
    m_vPlayer2GridItems.clear();


    for (int row = 0; row < ui->Player1RowsSpinner->value(); ++row)
    {
        for (int col = 0; col < ui->Player1ColsSpinner->value(); ++col)
        {
            QCheckBox* check = new QCheckBox(this);
            check->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            check->setMinimumHeight(20);
            check->setMinimumWidth(20);

            check->setText("Réponse");

            if (row >= m_vPlayer1GridItems.size())
            {
                m_vPlayer1GridItems.append(QVector<QCheckBox*>());
            }

            m_vPlayer1GridItems[row].append(check);

            ui->Player1GridLayout->addWidget(check, row, col);
        }
    }


    for (int row = 0; row < ui->Player2RowsSpinner->value(); ++row)
    {
        for (int col = 0; col < ui->Player2ColsSpinner->value(); ++col)
        {
            QCheckBox* check = new QCheckBox(this);
            check->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            check->setMinimumHeight(20);
            check->setMinimumWidth(20);

            check->setText("Réponse");

            if (row >= m_vPlayer2GridItems.size())
            {
                m_vPlayer2GridItems.append(QVector<QCheckBox*>());
            }

            m_vPlayer2GridItems[row].append(check);

            ui->Player2GridLayout->addWidget(check, row, col);
        }
    }

}



void CQuizWizard::clickOnCancel(bool)
{
    close();
}

void CQuizWizard::clickOnValidate(bool)
{
    bool isOkay = false;
    for (QVector<QCheckBox*> col1 : m_vPlayer1GridItems)
    {
        for (QCheckBox* check : col1)
        {
            isOkay = isOkay || check->isChecked();
        }
    }
    for (QVector<QCheckBox*> col2 : m_vPlayer2GridItems)
    {
        for (QCheckBox* check : col2)
        {
            isOkay = isOkay || check->isChecked();
        }
    }

    if (!isOkay)
    {
        QMessageBox::critical(this, "Erreur !",
                              "Impossible de créer le jeu, il vous faut au moins une bonne réponse.");
        return;
    }

    ON_CC_THREAD(CQuizWizard::GenerateScenes, this);
}



void CQuizWizard::GenerateScenes()
{
    std::string player1SceneID;
    std::string player2SceneID;


    if (!m_oNewGameInfo.isSwaped)
    {
        player1SceneID = m_oNewGameInfo.newID1.toStdString();
        player2SceneID = m_oNewGameInfo.newID2.toStdString();
    }
    else
    {
        player1SceneID = m_oNewGameInfo.newID2.toStdString();
        player2SceneID = m_oNewGameInfo.newID1.toStdString();
    }

    LM::CSceneNode* player1Scene = new LM::CSceneNode(player1SceneID);
    LM::CSceneNode* player2Scene = new LM::CSceneNode(player2SceneID);

    LM::CSceneNode *sceneP1, *sceneP2;
    if (!m_oNewGameInfo.isSwaped)
    {
        sceneP1 = player1Scene;
        sceneP2 = player2Scene;
    }
    else
    {
        sceneP1 = player2Scene;
        sceneP2 = player1Scene;
    }

    std::string basePath = CProjectManager::Instance()->GetProjectPath();

    player1Scene->SetSynced(true);
    LM::CSpriteNode* backgroundImage1 = new LM::CSpriteNode("#default-background",
                                                           LM::CENTER,
                                                           100);
    player1Scene->AddChildNode(backgroundImage1);

    LM::CGridNode* player1Grid = new LM::CGridNode(ui->Player1RowsSpinner->value(),
                                                   ui->Player1ColsSpinner->value(),
                                                   LM::EAnchor::CENTER,
                                                   50, 50, 0, 0);
    player1Scene->AddChildNode(player1Grid);

    LM::CSpriteNode* infoBottom1 = new LM::CSpriteNode(basePath + "ui/info-6.png",
                                                      LM::EAnchor::BOTTOM,
                                                      0, 13);
    player1Scene->AddChildNode(infoBottom1);

    LM::CGroupNode* infoBottom1Group = new LM::CGroupNode(LM::EAnchor::CENTER, 60, 100);
    infoBottom1->AddChildNode(infoBottom1Group);

    LM::CAnimationNode* bottom1TouchAnimation = new LM::CAnimationNode(basePath + "ui/animations/touch.png",
                                                                       basePath + "ui/animations/touch.plist",
                                                                       LM::EAnchor::LEFT,
                                                                       0, 80);
    infoBottom1Group->AddChildNode(bottom1TouchAnimation);

    LM::CLabelNode* infoBottom1Text = new LM::CLabelNode("Touche la bonne réponse",
                                                         "#default-font", 21,
                                                         "center", "0,0,0,255",
                                                         LM::EAnchor::CENTER, 95);
    infoBottom1Group->AddChildNode(infoBottom1Text);

    LM::CSpriteNode* infoTop1 = new LM::CSpriteNode(basePath + "ui/info-2.png",
                                                   LM::EAnchor::TOP,
                                                   0, 13);
    player1Scene->AddChildNode(infoTop1);

    LM::CLabelNode* infoTop1Text = new LM::CLabelNode("Instructions scénario",
                                                      "#default-font", 20, "center",
                                                      "0,0,0,255",
                                                      LM::EAnchor::CENTER,
                                                      80, 100);
    infoTop1->AddChildNode(infoTop1Text);

    LM::CMenuNode* navRight1 = new LM::CMenuNode(basePath + "ui/nav-5.png",
                                                 basePath + "ui/nav-5-active.png",
                                                 LM::CCallback<LM::CKernel, cocos2d::Ref*>(
                                                     "Nav",
                                                     m_pKernel,
                                                     &LM::CKernel::NavNext),
                                                 LM::EAnchor::BOTTOM_RIGHT,
                                                 0, 13, 0, 0, "next");
    player1Scene->AddChildNode(navRight1);

    LM::CEventCallback showNav1("Show", m_pKernel, &LM::CKernel::SetNodeVisible,
                                LM::SEvent(LM::SEvent::BOOLEAN, navRight1, "Validate", true));
    navRight1->AddListener("Validate", showNav1);

    LM::CLabelNode* navLabel1 = new LM::CLabelNode("Suivant",
                                                   "#default-font",
                                                   24, "center", "",
                                                   LM::EAnchor::CENTER, 100, 100);
    navRight1->AddChildNode(navLabel1);



    // fill player2 scene
    player2Scene->SetSynced(true);
    LM::CValidator* validator = new LM::CValidator(m_pKernel);
    player2Scene->AddChildNode(validator);

    LM::CSpriteNode* backgroundImage2 = new LM::CSpriteNode("#default-background",
                                                            LM::EAnchor::CENTER,
                                                            100, 100);
    player2Scene->AddChildNode(backgroundImage2);

    LM::CGridNode* player2Grid = new LM::CGridNode(ui->Player2RowsSpinner->value(),
                                                   ui->Player2ColsSpinner->value(),
                                                   LM::EAnchor::CENTER,
                                                   50, 50, 0, 0);
    player2Scene->AddChildNode(player2Grid);


    LM::CSpriteNode* infoBottom2 = new LM::CSpriteNode(basePath + "ui/info-6.png",
                                                       LM::EAnchor::BOTTOM,
                                                       0, 13);
    player2Scene->AddChildNode(infoBottom2);

    LM::CGroupNode* infoBottom2Group = new LM::CGroupNode(LM::EAnchor::CENTER,
                                                          60, 100);
    infoBottom2->AddChildNode(infoBottom2Group);

    LM::CAnimationNode* bottom2TouchAnimation = new LM::CAnimationNode(
                basePath + "ui/animations/touch.png",
                basePath + "ui/animations/touch.plist",
                LM::EAnchor::LEFT, 0, 80);
    infoBottom2Group->AddChildNode(bottom2TouchAnimation);

    LM::CLabelNode* infoBottom2Text = new LM::CLabelNode("Touche la bonne réponse",
                                                         "#default-font",
                                                         21, "center", "0,0,0,255",
                                                         LM::EAnchor::CENTER, 95);
    infoBottom2Group->AddChildNode(infoBottom2Text);

    LM::CSpriteNode* infoTop2 = new LM::CSpriteNode(basePath + "ui/info-2.png",
                                                    LM::EAnchor::TOP,
                                                    0, 13);
    player2Scene->AddChildNode(infoTop2);

    LM::CLabelNode* infoTop2Text = new LM::CLabelNode("Instructions Scénario",
                                                      "#default-font",
                                                      20, "center", "0,0,0,255",
                                                      LM::EAnchor::CENTER,
                                                      80, 100);
    infoTop2->AddChildNode(infoTop2Text);

    LM::CMenuNode* navRight2 = new LM::CMenuNode(basePath + "ui/nav-5.png",
                                                 basePath + "ui/nav-5-active.png",
                                                 LM::CCallback<LM::CKernel, cocos2d::Ref*>(
                                                     "Nav",
                                                     m_pKernel,
                                                     &LM::CKernel::NavNext),
                                                 LM::EAnchor::BOTTOM_RIGHT,
                                                 0, 13, 0, 0, "next");
    player2Scene->AddChildNode(navRight2);

    LM::CEventCallback showNav2("Show", m_pKernel, &LM::CKernel::SetNodeVisible,
                                LM::SEvent(LM::SEvent::BOOLEAN, navRight1, "Validate", true));
    navRight2->AddListener("Validate", showNav2);

    LM::CLabelNode* navLabel2 = new LM::CLabelNode("Suivant",
                                                   "#default-font",
                                                   24, "center", "",
                                                   LM::EAnchor::CENTER, 100, 100);
    navRight2->AddChildNode(navLabel2);



    // build dynamic grids
    for (int player1Row = ui->Player1RowsSpinner->value() - 1; player1Row >= 0; --player1Row)
    {
        for (int player1Col = 0; player1Col < ui->Player1ColsSpinner->value(); ++player1Col)
        {
            std::string cellID = std::string("Player1-") + std::to_string(player1Row) + std::to_string(player1Col);

            LM::CSpriteNode* cell = new LM::CSpriteNode(basePath + "ui/cache-noir-70.png",
                                                        LM::EAnchor::CENTER,
                                                        80);
            player1Grid->AddChildNode(cell);

            LM::CLabelNode* cellLabel = new LM::CLabelNode("Réponse",
                                                           "#default-font",
                                                           24, "center", "255,255,255,255",
                                                           LM::EAnchor::CENTER,
                                                           100, 100);
            cell->AddChildNode(cellLabel);



            LM::CSpriteNode* check;
            if (m_vPlayer1GridItems[player1Row][player1Col]->isChecked())
            {
                cellLabel->SetText("Bonne réponse");

                check = new LM::CSpriteNode(basePath + "ui/check.png",
                                            LM::EAnchor::CENTER,
                                            0, 80);


                LM::CEventCallback validateCell("SendMessage", m_pKernel,
                                             &LM::CKernel::SendNetworkMessage,
                                             LM::SEvent(LM::SEvent::STRING, cell, std::string("kernel:ValidateID:") + cellID));
                cell->AddListener("Touch", validateCell);
                validator->AddID(cellID);


            }
            else
            {
                cellLabel->SetText("Mauvaise réponse");

                check = new LM::CSpriteNode(basePath + "ui/cross.png",
                                            LM::EAnchor::CENTER,
                                            0, 80);
            }
            check->SetID(cellID + "Check");
            check->SetVisible(false);
            cell->AddChildNode(check);

            LM::CEventCallback showCheck("Show", m_pKernel,
                                         &LM::CKernel::SetNodeVisible,
                                         LM::SEvent(LM::SEvent::BOOLEAN, cell, "", true));
            check->AddListener("Show", showCheck);

            LM::CEventCallback showCheckOnTouch("LocalMessage", m_pKernel,
                                         &LM::CKernel::LocalMessage,
                                         LM::SEvent(LM::SEvent::STRING, cell, player1SceneID + ":Show:" + cellID + "Check"));
            cell->AddListener("Touch", showCheckOnTouch);
        }
    }


    for (int player2Row = ui->Player2RowsSpinner->value() - 1; player2Row >= 0; --player2Row)
    {
        for (int player2Col = 0; player2Col < ui->Player2ColsSpinner->value(); ++player2Col)
        {
            std::string cellID = std::string("Player1-") + std::to_string(player2Row) + std::to_string(player2Col);

            LM::CSpriteNode* cell = new LM::CSpriteNode(basePath + "ui/cache-noir-70.png",
                                                        LM::EAnchor::CENTER,
                                                        80);
            player2Grid->AddChildNode(cell);

            LM::CLabelNode* cellLabel = new LM::CLabelNode("Réponse",
                                                           "#default-font",
                                                           24, "center", "255,255,255,255",
                                                           LM::EAnchor::CENTER,
                                                           100, 100);
            cell->AddChildNode(cellLabel);



            LM::CSpriteNode* check;
            if (m_vPlayer2GridItems[player2Row][player2Col]->isChecked())
            {
                cellLabel->SetText("Bonne réponse");

                check = new LM::CSpriteNode(basePath + "ui/check.png",
                                            LM::EAnchor::CENTER,
                                            0, 80);


                LM::CEventCallback validateCell("Validate", m_pKernel,
                                             &LM::CKernel::Validate,
                                             LM::SEvent(LM::SEvent::STRING, cell, cellID));
                cell->AddListener("Touch", validateCell);
                validator->AddID(cellID);
            }
            else
            {
                cellLabel->SetText("Mauvaise réponse");
                check = new LM::CSpriteNode(basePath + "ui/cross.png",
                                            LM::EAnchor::CENTER,
                                            0, 80);
            }
            check->SetID(cellID + "Check");
            check->SetVisible(false);
            cell->AddChildNode(check);

            LM::CEventCallback showCheck("Show", m_pKernel,
                                         &LM::CKernel::SetNodeVisible,
                                         LM::SEvent(LM::SEvent::BOOLEAN, cell, "", true));
            check->AddListener("Show", showCheck);

            LM::CEventCallback showCheckOnTouch("LocalMessage", m_pKernel,
                                         &LM::CKernel::LocalMessage,
                                         LM::SEvent(LM::SEvent::STRING, cell, player2SceneID + ":Show:" + cellID + "Check"));
            cell->AddListener("Touch", showCheckOnTouch);

        }
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
