#include "CSceneInspector.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QSpacerItem>
#include <QTabWidget>
#include <QButtonGroup>
#include <QFrame>

#include "CLineEdit.h"

#include <QDebug>

CSceneInspector::CSceneInspector(LM::CSceneNode* a_pScene, int a_iPlayerID, QWidget *parent) :
    QWidget(parent),
    m_pScene(a_pScene)
{
//    QWidget* optionColWidget = new QWidget(this);
//    optionColWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

//    QWidget* firstRow = new QWidget(optionColWidget);
//    firstRow->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
////    firstRow->setMinimumHeight(50);
//    QWidget* secondRow = new QWidget(optionColWidget);
//    secondRow->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
////    secondRow->setMinimumHeight(50);

//    QHBoxLayout* hFirstRowLayout = new QHBoxLayout(this);
//    QHBoxLayout* hSecondRowLayout = new QHBoxLayout(this);

//    firstRow->setLayout(hFirstRowLayout);
//    secondRow->setLayout(hSecondRowLayout);

//    // Fill firstRow with id and player id
//    QString sID = "id : ";
//    sID.append(m_pScene->GetSceneID().c_str());
//    QLabel* id = new QLabel(sID);
//    id->setStyleSheet("QLabel{ color : white;}");
//    hFirstRowLayout->addWidget(id, Qt::AlignLeft);
////    QWidget* playerCheckBox = new QWidget(this);
////    QHBoxLayout* hPlayerLayout = new QHBoxLayout(this);
////    QCheckBox* player1CheckBox = new QCheckBox("Joueur 1");
////    player1CheckBox->setStyleSheet("QCheckBox{ color : white;}");
////    QCheckBox* player2CheckBox = new QCheckBox("Joueur 2");
////    player2CheckBox->setStyleSheet("QCheckBox{ color : white;}");
////    hPlayerLayout->addWidget(player1CheckBox, Qt::AlignRight);
////    hPlayerLayout->addWidget(player2CheckBox, Qt::AlignRight);
////    // initialise check box
////    if(a_iPlayerID == 1)
////    {
////        player2CheckBox->setChecked(true);
////        player1CheckBox->setChecked(false);
////    }
////    else if(a_iPlayerID == 0)
////    {
////        player1CheckBox->setChecked(true);
////        player2CheckBox->setChecked(false);
////    }
////    else
////    {
////        player1CheckBox->setChecked(true);
////        player2CheckBox->setChecked(true);
////    }

////    playerCheckBox->setLayout(hPlayerLayout);
////    hFirstRowLayout->addWidget(playerCheckBox, Qt::AlignLeft);
//    QLabel* soundLabel = new QLabel("Ajouter un son :", firstRow);
//    soundLabel->setStyleSheet("QLabel{ color : white;}");
//    hFirstRowLayout->addWidget(soundLabel, Qt::AlignLeft);

//    QWidget* soundWidget = new QWidget(this);
//    soundWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
//    QGridLayout* soundLayout = new QGridLayout(soundWidget);
//    soundWidget->setLayout(soundLayout);
//    QLabel* initLabel = new QLabel("Initialisation", soundWidget);
//    initLabel->setStyleSheet("QLabel{ color : white;}");
//    initLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
//    QLabel* validLabel = new QLabel("Validation", soundWidget);
//    validLabel->setStyleSheet("QLabel{ color : white;}");
//    validLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

//    soundLayout->addWidget(initLabel, 0, 0, 1, 1, Qt::AlignLeft);
//    soundLayout->addWidget(validLabel, 0, 1, 1, 1, Qt::AlignLeft);

//    hFirstRowLayout->addWidget(soundWidget, Qt::AlignRight);

//    // Fill second row with synchro and dash
//    QCheckBox* isDashCB = new QCheckBox("Déclencher le dashboard", secondRow);
//    isDashCB->setStyleSheet("QCheckBox{color : white}");
//    isDashCB->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);

//    if(m_pScene->m_bDashboardTrigger)
//    {
//        isDashCB->setChecked(true);
//    }

//    QCheckBox* enableReward = new QCheckBox("Débloquer une récompense", secondRow);
//    enableReward->setStyleSheet("QCheckBox{color : white}");
//    enableReward->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);

//    if(m_pScene->GetRewardID() != "")
//    {
//        enableReward->setChecked(true);
//    }

//    QLineEdit* rewardID = new QLineEdit(QString::fromStdString(m_pScene->GetRewardID()), secondRow);
//    rewardID->setPlaceholderText(tr("ID récompense"));
//    rewardID->setEnabled(enableReward->isChecked());
//    enableReward->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

//    hSecondRowLayout->addWidget(isDashCB);
//    hSecondRowLayout->addWidget(enableReward);
//    hSecondRowLayout->addWidget(rewardID);
//    hSecondRowLayout->setAlignment(isDashCB, Qt::AlignLeft);
//    hSecondRowLayout->setAlignment(enableReward, Qt::AlignRight);
//    hSecondRowLayout->setAlignment(rewardID, Qt::AlignRight);

//    connect(isDashCB, SIGNAL(clicked(bool)), this, SLOT(switchDash(bool)));
//    connect(enableReward, SIGNAL(toggled(bool)), rewardID, SLOT(setEnabled(bool)));
//    connect(enableReward, SIGNAL(toggled(bool)), rewardID, SLOT(clear()));
//    connect(rewardID, SIGNAL(textChanged(QString)), this, SLOT(changeRewardID(QString)));

//    QVBoxLayout* vOptionLayout = new QVBoxLayout(this);
//    vOptionLayout->addWidget(firstRow, Qt::AlignLeft);
//    vOptionLayout->addWidget(secondRow, Qt::AlignLeft);
//    optionColWidget->setLayout(vOptionLayout);

    QString sID = "Scène : ";
    sID.append(m_pScene->GetSceneID().c_str());
    QLabel* id = new QLabel(sID, this);
    id->setStyleSheet("QLabel{ color : white; padding : 6px;}");
    id->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
    QFrame* separator = new QFrame(this);
    separator->setFrameShape(QFrame::VLine);
    separator->setStyleSheet("QFrame{ color : white; width : 2px;}");

    QWidget* soundWidget = new QWidget(this);
    soundWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    QGridLayout* gSoundLayout = new QGridLayout(soundWidget);
    QCheckBox* initCB = new QCheckBox("Initialisation :", soundWidget);
    initCB->setStyleSheet("QCheckBox{color : white}");
    initCB->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
    if(m_pScene->GetInitSound() != "")
    {
        initCB->setChecked(true);
    }
    QCheckBox* validCB = new QCheckBox("Validation :", soundWidget);
    validCB->setStyleSheet("QCheckBox{color : white}");
    validCB->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
    if(m_pScene->GetValidSound() != "")
    {
        validCB->setChecked(true);
    }
    CLineEdit* initPath = new CLineEdit(ETypes::Sound, soundWidget);
    initPath->setText(QString::fromStdString(m_pScene->GetInitSound()));
    initPath->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    initPath->setEnabled(initCB->isChecked());
    CLineEdit* validPath = new CLineEdit(ETypes::Sound, soundWidget);
    validPath->setText(QString::fromStdString(m_pScene->GetValidSound()));
    validPath->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    validPath->setEnabled(validCB->isChecked());
    gSoundLayout->addWidget(initCB, 0, 0);
    gSoundLayout->addWidget(initPath, 0, 1);
    gSoundLayout->addWidget(validCB, 1, 0);
    gSoundLayout->addWidget(validPath, 1, 1);
    soundWidget->setLayout(gSoundLayout);
    connect(initCB, SIGNAL(toggled(bool)), initPath, SLOT(setEnabled(bool)));
    connect(initCB, SIGNAL(toggled(bool)), initPath, SLOT(clear()));
    connect(initPath, SIGNAL(textChanged(QString)), this, SLOT(changeInitSound(QString)));
    connect(validCB, SIGNAL(toggled(bool)), validPath, SLOT(setEnabled(bool)));
    connect(validCB, SIGNAL(toggled(bool)), validPath, SLOT(clear()));
    connect(validPath, SIGNAL(textChanged(QString)), this, SLOT(changeValidSound(QString)));

    soundWidget->setVisible(false);

    QWidget* dashWidget = new QWidget(this);
    dashWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    QGridLayout* gDashLayout = new QGridLayout(dashWidget);
    QCheckBox* isDashCB = new QCheckBox("Déclencher l'écran de récompenses", dashWidget);
    isDashCB->setStyleSheet("QCheckBox{color : white}");
    isDashCB->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
    QCheckBox* enableReward = new QCheckBox("Débloquer une récompense", dashWidget);
    enableReward->setStyleSheet("QCheckBox{color : white}");
    enableReward->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
    if(m_pScene->GetRewardID() != "")
    {
        enableReward->setChecked(true);
    }
    QLineEdit* rewardID = new QLineEdit(QString::fromStdString(m_pScene->GetRewardID()), dashWidget);
    rewardID->setPlaceholderText(tr("ID récompense"));
    rewardID->setEnabled(enableReward->isChecked());
    enableReward->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    gDashLayout->addWidget(isDashCB, 0, 0);
    gDashLayout->addWidget(enableReward, 1, 0);
    gDashLayout->addWidget(rewardID, 1, 1);
    dashWidget->setLayout(gDashLayout);
    connect(isDashCB, SIGNAL(clicked(bool)), this, SLOT(switchDash(bool)));
    connect(enableReward, SIGNAL(toggled(bool)), rewardID, SLOT(setEnabled(bool)));
    connect(enableReward, SIGNAL(toggled(bool)), rewardID, SLOT(clear()));
    connect(rewardID, SIGNAL(textChanged(QString)), this, SLOT(changeRewardID(QString)));
    dashWidget->setVisible(false);

    // Create button layout
    QWidget* buttonColWidget = new QWidget(this);
    buttonColWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
//    QVBoxLayout* vButtonLayout = new QVBoxLayout(buttonColWidget);
    QGridLayout* gButtonLayout = new QGridLayout(buttonColWidget);

    QPushButton* soundButton = new QPushButton("Ajouter des sons", buttonColWidget);
    soundButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    soundButton->setStyleSheet("QPushButton{ border : 1px solid white; padding : 6px; color : white;}"
                               "QPushButton:checked{ border : 2px solid white; padding : 5px; color : white;}");
    soundButton->setFlat(true);
    soundButton->setCheckable(true);
    QPushButton* dashButton = new QPushButton("Intéragir avec les récompenses", buttonColWidget);
    dashButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    dashButton->setStyleSheet("QPushButton{ border : 1px solid white; padding : 6px; color : white;}"
                              "QPushButton:checked{ border : 2px solid white; padding : 5px; color : white;}");
    dashButton->setFlat(true);
    dashButton->setCheckable(true);
    QButtonGroup* modeSelector = new QButtonGroup(buttonColWidget);
    modeSelector->addButton(soundButton);
    modeSelector->addButton(dashButton);
    modeSelector->setExclusive(true);
    QPushButton* deleteSceneButton = new QPushButton("Supprimer scène", buttonColWidget);
    deleteSceneButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    deleteSceneButton->setStyleSheet("QPushButton{ border : 1px solid white; padding : 6px; color : white;}");
    deleteSceneButton->setFlat(true);
    QPushButton* addSceneButton = new QPushButton("Ajouter scène", buttonColWidget);
    addSceneButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    addSceneButton->setStyleSheet("QPushButton{ border : 1px solid white; padding : 6px; color : white;}");
    addSceneButton->setFlat(true);
//    vButtonLayout->addWidget(deleteSceneButton);
//    vButtonLayout->addWidget(addSceneButton);
    gButtonLayout->addWidget(soundButton, 0, 0);
    gButtonLayout->addWidget(dashButton, 1, 0);
    gButtonLayout->addWidget(deleteSceneButton, 0, 1);
    gButtonLayout->addWidget(addSceneButton, 1, 1);
//    buttonColWidget->setLayout(vButtonLayout);
    buttonColWidget->setLayout(gButtonLayout);
    buttonColWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);

    QHBoxLayout* hSceneInspectorLayout = new QHBoxLayout(this);

//    hSceneInspectorLayout->addWidget(optionColWidget, Qt::AlignLeft);
    this->setLayout(hSceneInspectorLayout);
//    hSceneInspectorLayout->addWidget(optionTabWidget, Qt::AlignLeft);
//    layout()->setAlignment(optionTabWidget, Qt::AlignLeft);
    hSceneInspectorLayout->addWidget(id, Qt::AlignLeft);
    hSceneInspectorLayout->addWidget(separator, Qt::AlignLeft);
    hSceneInspectorLayout->addWidget(soundWidget, Qt::AlignCenter);
    hSceneInspectorLayout->addWidget(dashWidget, Qt::AlignCenter);
    hSceneInspectorLayout->addWidget(buttonColWidget, Qt::AlignRight);

    layout()->setAlignment(id, Qt::AlignLeft);
    layout()->setAlignment(separator, Qt::AlignLeft);
//    layout()->setAlignment(soundWidget, Qt::AlignCenter);
//    layout()->setAlignment(dashWidget, Qt::AlignCenter);
    layout()->setAlignment(buttonColWidget, Qt::AlignRight);

    // Connect push button
    connect(addSceneButton, SIGNAL(clicked(bool)), this, SLOT(clickOnAdd()));
    connect(deleteSceneButton, SIGNAL(clicked(bool)), this, SLOT(clickOnDelete()));

    connect(soundButton, SIGNAL(toggled(bool)), soundWidget, SLOT(setVisible(bool)));
    connect(dashButton, SIGNAL(toggled(bool)), dashWidget, SLOT(setVisible(bool)));
    soundButton->toggle();
}



void CSceneInspector::switchDash(bool a_bState)
{
    this->m_pScene->m_bDashboardTrigger = a_bState;
}

void CSceneInspector::changeRewardID(const QString &a_sRewardID)
{
    this->m_pScene->SetRewardID(a_sRewardID.toStdString());
}

void CSceneInspector::changeInitSound(const QString &a_sInitSound)
{
    this->m_pScene->SetInitSound(a_sInitSound.toStdString());
}

void CSceneInspector::changeValidSound(const QString &a_sValidSound)
{
    this->m_pScene->SetValidSound(a_sValidSound.toStdString());
}

void CSceneInspector::switchP1(bool a_bState)
{
    // disable atm
}

void CSceneInspector::switchP2(bool a_bState)
{
    // disable atm
}

void CSceneInspector::clickOnAdd()
{
    emit addScene();
}


void CSceneInspector::clickOnDelete()
{
    // if sync scene, delete it's screenmate
    emit deleteScene(QString(this->m_pScene->GetSceneID().c_str()), this->m_pScene->IsSynced());
}
