#include "CSceneInspector.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include <QDebug>

CSceneInspector::CSceneInspector(LM::CSceneNode* a_pScene, int a_iPlayerID, QWidget *parent) :
    QWidget(parent),
    m_pScene(a_pScene)
{
    QWidget* firstRow = new QWidget(this);
    QWidget* secondRow = new QWidget(this);

    QHBoxLayout* hFirstRowLayout = new QHBoxLayout(this);
    QHBoxLayout* hSecondRowLayout = new QHBoxLayout(this);

    // Fill firstRow with id and player id
    QString sID = "id : ";
    sID.append(m_pScene->GetSceneID().c_str());
    QLabel* id = new QLabel(sID);
    id->setStyleSheet("QLabel{ color : white;}");
    hFirstRowLayout->addWidget(id);
    QWidget* playerCheckBox = new QWidget(this);
    QHBoxLayout* hPlayerLayout = new QHBoxLayout(this);
    QCheckBox* player1CheckBox = new QCheckBox("Joueur 1");
    player1CheckBox->setStyleSheet("QCheckBox{ color : white;}");
    QCheckBox* player2CheckBox = new QCheckBox("Joueur 2");
    player2CheckBox->setStyleSheet("QCheckBox{ color : white;}");
    hPlayerLayout->addWidget(player1CheckBox);
    hPlayerLayout->addWidget(player2CheckBox);
    // initialise check box
    if(a_iPlayerID == 2)
    {
        player2CheckBox->setChecked(true);
        player1CheckBox->setChecked(false);
    }
    else if(a_iPlayerID == 1)
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
    hFirstRowLayout->addWidget(playerCheckBox);

    // Fill second row with synchro and dash
    QCheckBox* isSynchCB = new QCheckBox("est synchronisée", this);
    isSynchCB->setStyleSheet("QCheckBox{color : white;}");
    QCheckBox* isDashCB = new QCheckBox("déclenche le dashboard", this);
    isDashCB->setStyleSheet("QCheckBox{color : white}");
    if(m_pScene->IsSynced())
    {
        isSynchCB->setChecked(true);
    }
    if(m_pScene->m_bDashboardTrigger)
    {
        isDashCB->setChecked(true);
    }
    hSecondRowLayout->addWidget(isSynchCB);
    hSecondRowLayout->addWidget(isDashCB);

    firstRow->setLayout(hFirstRowLayout);
    secondRow->setLayout(hSecondRowLayout);


    QVBoxLayout* vWidgetLayout = new QVBoxLayout(this);
    vWidgetLayout->addWidget(firstRow);
    vWidgetLayout->addWidget(secondRow);
    this->setLayout(vWidgetLayout);

    // Connect all checkbox
    connect(isSynchCB, SIGNAL(clicked(bool)), this, SLOT(switchSynchro(bool)));
    connect(isDashCB, SIGNAL(clicked(bool)), this, SLOT(switchDash(bool)));

    // temporary disable player check box
    player1CheckBox->setEnabled(false);
    player2CheckBox->setEnabled(false);
    connect(player1CheckBox, SIGNAL(clicked(bool)), this, SLOT(switchP1(bool)));
    connect(player2CheckBox, SIGNAL(clicked(bool)), this, SLOT(switchP2(bool)));
}


void CSceneInspector::switchSynchro(bool a_bState)
{
    this->m_pScene->SetSynced(a_bState);
}

void CSceneInspector::switchDash(bool a_bState)
{
    this->m_pScene->m_bDashboardTrigger = a_bState;
}

void CSceneInspector::switchP1(bool a_bState)
{
    if(a_bState)
    {

    }
    else
    {

    }
}

void CSceneInspector::switchP2(bool a_bState)
{
    if(a_bState)
    {

    }
    else
    {

    }
}
