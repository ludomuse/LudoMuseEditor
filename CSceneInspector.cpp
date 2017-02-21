#include "CSceneInspector.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

#include <QDebug>

CSceneInspector::CSceneInspector(LM::CSceneNode* a_pScene, int a_iPlayerID, QWidget *parent) :
    QWidget(parent),
    m_pScene(a_pScene)
{
    QWidget* optionColWidget = new QWidget(this);
    QWidget* buttonColWidget = new QWidget(this);

    QWidget* firstRow = new QWidget(this);
    firstRow->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    firstRow->setMinimumHeight(50);
    QWidget* secondRow = new QWidget(this);
    secondRow->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    secondRow->setMinimumHeight(50);

    QHBoxLayout* hFirstRowLayout = new QHBoxLayout(this);
    QHBoxLayout* hSecondRowLayout = new QHBoxLayout(this);

    firstRow->setLayout(hFirstRowLayout);
    secondRow->setLayout(hSecondRowLayout);

    // Fill firstRow with id and player id
    QString sID = "id : ";
    sID.append(m_pScene->GetSceneID().c_str());
    QLabel* id = new QLabel(sID);
    id->setStyleSheet("QLabel{ color : white;}");
    hFirstRowLayout->addWidget(id, Qt::AlignLeft);
    QWidget* playerCheckBox = new QWidget(this);
    QHBoxLayout* hPlayerLayout = new QHBoxLayout(this);
    QCheckBox* player1CheckBox = new QCheckBox("Joueur 1");
    player1CheckBox->setStyleSheet("QCheckBox{ color : white;}");
    QCheckBox* player2CheckBox = new QCheckBox("Joueur 2");
    player2CheckBox->setStyleSheet("QCheckBox{ color : white;}");
    hPlayerLayout->addWidget(player1CheckBox, Qt::AlignRight);
    hPlayerLayout->addWidget(player2CheckBox, Qt::AlignRight);
    // initialise check box
    if(a_iPlayerID == 1)
    {
        player2CheckBox->setChecked(true);
        player1CheckBox->setChecked(false);
    }
    else if(a_iPlayerID == 0)
    {
        player1CheckBox->setChecked(true);
        player2CheckBox->setChecked(false);
    }
    else
    {
        player1CheckBox->setChecked(true);
        player2CheckBox->setChecked(true);
    }

    playerCheckBox->setLayout(hPlayerLayout);
    hFirstRowLayout->addWidget(playerCheckBox, Qt::AlignLeft);

    // Fill second row with synchro and dash
    QCheckBox* isDashCB = new QCheckBox("déclenche le dashboard", this);
    isDashCB->setStyleSheet("QCheckBox{color : white}");

    if(m_pScene->m_bDashboardTrigger)
    {
        isDashCB->setChecked(true);
    }
    hSecondRowLayout->addWidget(isDashCB);

    QCheckBox* enableReward = new QCheckBox("active une récompense", this);
    enableReward->setStyleSheet("QCheckBox{color : white}");

    if(m_pScene->GetRewardID() != "")
    {
        enableReward->setChecked(true);
    }

    QLineEdit* rewardID = new QLineEdit(QString::fromStdString(m_pScene->GetRewardID()), this);
    rewardID->setPlaceholderText(tr("ID récompense"));
    rewardID->setVisible(enableReward->isChecked());

    hSecondRowLayout->addWidget(isDashCB, Qt::AlignLeft);
    hSecondRowLayout->addWidget(enableReward, Qt::AlignLeft);
    hSecondRowLayout->addWidget(rewardID, Qt::AlignLeft);
    connect(enableReward, SIGNAL(toggled(bool)), rewardID, SLOT(setVisible(bool)));
    connect(enableReward, SIGNAL(toggled(bool)), rewardID, SLOT(clear()));

    QVBoxLayout* vOptionLayout = new QVBoxLayout(this);
    vOptionLayout->addWidget(firstRow, Qt::AlignLeft);
    vOptionLayout->addWidget(secondRow, Qt::AlignLeft);
    optionColWidget->setLayout(vOptionLayout);

    // Create button layout
    QVBoxLayout* vButtonLayout = new QVBoxLayout();

    QPushButton* deleteSceneButton = new QPushButton("Supprimer scène");
    QPushButton* addSceneButton = new QPushButton("Ajouter scène");
    QWidget* deleteSceneButtonWidget = new QWidget();
    deleteSceneButtonWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    deleteSceneButtonWidget->setMaximumHeight(25);
    deleteSceneButtonWidget->setMaximumWidth(150);
    deleteSceneButton->setMaximumHeight(25);
    QWidget* addSceneButtonWidget = new QWidget();
    addSceneButtonWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    addSceneButtonWidget->setMaximumHeight(25);
    addSceneButtonWidget->setMaximumWidth(150);
    addSceneButton->setMaximumHeight(25);
    QVBoxLayout* dummyLayout = new QVBoxLayout();
    dummyLayout->setContentsMargins(0,0,0,0);
    dummyLayout->addWidget(deleteSceneButton);
    deleteSceneButtonWidget->setLayout(dummyLayout);
    dummyLayout = new QVBoxLayout();
    dummyLayout->setContentsMargins(0,0,0,0);
    dummyLayout->addWidget(addSceneButton);
    addSceneButtonWidget->setLayout(dummyLayout);
    deleteSceneButton->setFlat(true);
    addSceneButton->setFlat(true);
    deleteSceneButtonWidget->setObjectName("deleteWidget");
    deleteSceneButtonWidget->setStyleSheet("#deleteWidget{ border : 1px solid white;} QWidget{color : white;}");
    addSceneButtonWidget->setObjectName("addWidget");
    addSceneButtonWidget->setStyleSheet("#addWidget{ border : 1px solid white;} QWidget{color : white;}");
    vButtonLayout->addWidget(deleteSceneButtonWidget);
    vButtonLayout->addWidget(addSceneButtonWidget);
    buttonColWidget->setLayout(vButtonLayout);
    buttonColWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
    buttonColWidget->setMaximumWidth(150);
    QHBoxLayout* hSceneInspectorLayout = new QHBoxLayout(this);
    hSceneInspectorLayout->addWidget(optionColWidget, Qt::AlignLeft);
    hSceneInspectorLayout->addWidget(buttonColWidget, Qt::AlignRight);
    this->setLayout(hSceneInspectorLayout);



    // Connect all checkbox
    connect(isDashCB, SIGNAL(clicked(bool)), this, SLOT(switchDash(bool)));

    // Connect push button
    connect(addSceneButton, SIGNAL(clicked(bool)), this, SLOT(clickOnAdd()));
    connect(deleteSceneButton, SIGNAL(clicked(bool)), this, SLOT(clickOnDelete()));

    // temporary disable player check box
    player1CheckBox->setEnabled(false);
    player2CheckBox->setEnabled(false);
    connect(player1CheckBox, SIGNAL(clicked(bool)), this, SLOT(switchP1(bool)));
    connect(player2CheckBox, SIGNAL(clicked(bool)), this, SLOT(switchP2(bool)));

    connect(rewardID, SIGNAL(textChanged(QString)), this, SLOT(changeRewardID(QString)));
}



void CSceneInspector::switchDash(bool a_bState)
{
    this->m_pScene->m_bDashboardTrigger = a_bState;
}

void CSceneInspector::changeRewardID(const QString &a_sRewardID)
{
    this->m_pScene->SetRewardID(a_sRewardID.toStdString());
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
