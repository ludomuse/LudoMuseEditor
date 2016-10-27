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
    m_pMenuNode(a_pMenuNode),

    m_sSavedText(a_pMenuNode->GetText())
{
    ui->setupUi(this);

    QButtonGroup* radioButtons = new QButtonGroup(this);
    ui->nextRButton->setAutoExclusive(true);
    ui->prevRButton->setAutoExclusive(true);
    QString action = QString(m_pMenuNode->GetAction().c_str());
    if(action == "next")
    {
        m_bSavedNav = true;
        ui->prevRButton->setChecked(false);
        ui->nextRButton->setChecked(true);
    }
    else
    {
        m_bSavedNav = false;
        ui->prevRButton->setChecked(true);
        ui->nextRButton->setChecked(false);
    }
    radioButtons->addButton(ui->nextRButton);
    radioButtons->addButton(ui->prevRButton);
    ui->TextLineEdit->setText(QString(m_pMenuNode->GetText().c_str()));

    connect(ui->backButton, SIGNAL(clicked(bool)), this, SLOT(discardChanges()));
    connect(ui->okButton, SIGNAL(clicked(bool)), this, SLOT(validateChanges()));
    connect(ui->prevRButton, SIGNAL(toggled(bool)), this, SLOT(checkPrev(bool)));
    connect(ui->nextRButton, SIGNAL(toggled(bool)), this, SLOT(checkNext(bool)));
    connect(ui->TextLineEdit, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));
}

CMenuNodeInspector::~CMenuNodeInspector()
{
    delete ui;
}

void CMenuNodeInspector::validateChanges()
{
    m_sSavedText = m_pMenuNode->GetText();
    if(m_pMenuNode->GetAction() == "next")
    {
        m_bSavedNav = true;
    }
    else
    {
        m_bSavedNav = false;
    }
    emit closeInspector();
}

void CMenuNodeInspector::discardChanges()
{
    emit closeInspector();
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

void CMenuNodeInspector::closeEvent (QCloseEvent *event)
{
    ON_CC_THREAD(LM::CMenuNode::SetText, m_pMenuNode, m_sSavedText);
    m_pMenuNode->SetNavAction(m_bSavedNav);
//    discardChanges();
    QWidget::closeEvent(event);
}

