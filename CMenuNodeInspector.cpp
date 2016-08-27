#include "CMenuNodeInspector.h"
#include "ui_CMenuNodeInspector.h"

#include <Classes/Engine/Include/CKernel.h>

#include <QButtonGroup>

CMenuNodeInspector::CMenuNodeInspector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CMenuNodeInspector)
{
    ui->setupUi(this);
    ui->nextRButton->setAutoExclusive(true);
    ui->prevRButton->setAutoExclusive(true);
}

CMenuNodeInspector::CMenuNodeInspector(LM::CMenuNode *a_pMenuNode, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CMenuNodeInspector),
    m_pMenuNode(a_pMenuNode)
{
    ui->setupUi(this);

    QButtonGroup* radioButtons = new QButtonGroup(this);
    ui->nextRButton->setAutoExclusive(true);
    ui->prevRButton->setAutoExclusive(true);
    QString action = QString(m_pMenuNode->GetAction().c_str());
    if(action == "next")
    {
        ui->prevRButton->setChecked(false);
        ui->nextRButton->setChecked(true);
    }
    else
    {
        ui->prevRButton->setChecked(true);
        ui->nextRButton->setChecked(false);
    }
    radioButtons->addButton(ui->nextRButton);
    radioButtons->addButton(ui->prevRButton);
    ui->TextLineEdit->setText(QString(m_pMenuNode->GetText().c_str()));

    connect(ui->backButton, SIGNAL(clicked(bool)), this, SLOT(closeInspectorSlot()));
    connect(ui->prevRButton, SIGNAL(toggled(bool)), this, SLOT(checkPrev(bool)));
    connect(ui->nextRButton, SIGNAL(toggled(bool)), this, SLOT(checkNext(bool)));
    connect(ui->TextLineEdit, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));
}

CMenuNodeInspector::~CMenuNodeInspector()
{
    delete ui;
}

void CMenuNodeInspector::checkPrev(bool a_rState)
{
    if(a_rState)
    {
        m_pMenuNode->SetNavAction(false);
    }
}

void CMenuNodeInspector::checkNext(bool a_rState)
{
    if(a_rState)
    {
        m_pMenuNode->SetNavAction(true);
    }
}

void CMenuNodeInspector::textChanged(QString a_sText)
{
    ON_CC_THREAD(LM::CMenuNode::SetText, m_pMenuNode, a_sText.toStdString());
}

void CMenuNodeInspector::closeInspectorSlot()
{
    emit closeInspector();
}

