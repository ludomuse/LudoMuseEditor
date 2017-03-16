#include "CTeamNodeInspector.h"

// Include Kernel to reach ON_CC_THREAD macro
#include "Classes/Engine/Include/CKernel.h"

// Include Util.cpp to reach SplitString function
#include "Classes/Modules/Util/Include/Util.h"

// Include QT
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QButtonGroup>

// Personnal include
#include "CLineEdit.h"
#include "CPathWidget.h"
#include "CProjectManager.h"
#include "CAnchorWidget.h"
#include "ETypes.h"

CTeamNodeInspector::CTeamNodeInspector(QWidget *parent):
    QWidget(parent)
{
    // Default constructor
}


CTeamNodeInspector::CTeamNodeInspector(LM::CTeamNode* a_pTeamNode, QWidget *parent):
    QWidget(parent),
    m_pTeamNode(a_pTeamNode),
    m_oSavedTasks(m_pTeamNode->GetTasks())
{
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    // Construction de tous les champs

    QVBoxLayout* vLayout = new QVBoxLayout();

    QHBoxLayout* hLayout = new QHBoxLayout();
    QLabel* pTasks = new QLabel("Tâches");
    pTasks->setStyleSheet("QLabel{color : white;}");
    pTasks->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    hLayout->addWidget(pTasks);
    QLabel* pActions = new QLabel("Actions");
    pActions->setStyleSheet("QLabel{color : white;}");
    pActions->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    hLayout->addWidget(pActions);

    QWidget* titles = new QWidget(this);
    titles->setLayout(hLayout);

    vLayout->addWidget(titles);


    for (int i = 0; i < M_NB_TASK; ++i)
    {
        QHBoxLayout* editsLayout = new QHBoxLayout();
        QLineEdit* task = new QLineEdit(this);
        task->setPlaceholderText("tâche à définir");
        task->setText(QString::fromStdString(m_oSavedTasks[i][0]));
        task->setAlignment(Qt::AlignLeft);
        editsLayout->addWidget(task);

        m_oTasks.push_back(task);

        connect(task, SIGNAL(textChanged(QString)), this, SLOT(changeTask(QString)));

        QLineEdit* action = new QLineEdit(this);
        action->setPlaceholderText("action à définir");
        action->setText(QString::fromStdString(m_oSavedTasks[i][1]));
        action->setAlignment(Qt::AlignLeft);
        editsLayout->addWidget(action);

        m_oActions.push_back(action);

        connect(action, SIGNAL(textChanged(QString)), this, SLOT(changeAction(QString)));

        QWidget* taskAction = new QWidget(this);
        taskAction->setLayout(editsLayout);

        vLayout->addWidget(taskAction);
    }

    QWidget* tasksActionsContainer = new QWidget();
    tasksActionsContainer->setLayout(vLayout);
    tasksActionsContainer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);


    QHBoxLayout* hLayoutButton = new QHBoxLayout();
    QPushButton* okButton = new QPushButton("Ok");
    QPushButton* backButton = new QPushButton("Annuler");
    hLayoutButton->addWidget(backButton);
    hLayoutButton->addWidget(okButton);

    QWidget* buttonContainer = new QWidget();
    buttonContainer->setLayout(hLayoutButton);
    buttonContainer->setMaximumHeight(100);
    buttonContainer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);


    QVBoxLayout *verticalLayout = new QVBoxLayout();
    verticalLayout->addWidget(tasksActionsContainer);
    verticalLayout->addSpacerItem(new QSpacerItem(0,0, QSizePolicy::Preferred, QSizePolicy::Expanding));
    verticalLayout->addWidget(buttonContainer);

    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(50,50,50,255));
    this->setLayout(verticalLayout);
    this->setAutoFillBackground(true);
    this->setPalette(pal);


    connect(okButton, SIGNAL(clicked(bool)), this, SLOT(validateChanges()));
    connect(backButton, SIGNAL(clicked(bool)), this, SLOT(discardChanges()));

}

// SIGNALS ***********************************************


void CTeamNodeInspector::validateChanges()
{
    emit closeInspector();
}

void CTeamNodeInspector::discardChanges()
{
    m_pTeamNode->SetTasks(m_oSavedTasks);
    emit closeInspector();
}

void CTeamNodeInspector::changeTask(QString a_oNewTask)
{
    for (int i = 0; i < m_oTasks.size(); ++i)
    {
        if (m_oTasks[i]->hasFocus())
        {
            m_pTeamNode->SetTask(i, a_oNewTask.toStdString());
        }
    }
}

void CTeamNodeInspector::changeAction(QString a_oNewAction)
{
    for (int i = 0; i < m_oActions.size(); ++i)
    {
        if (m_oActions[i]->hasFocus())
        {
            m_pTeamNode->SetAction(i, a_oNewAction.toStdString());
        }
    }
}


void CTeamNodeInspector::closeEvent (QCloseEvent *event)
{
    emit modifyTeamNode(m_pTeamNode);

////    ON_CC_THREAD(LM::CSpriteNode::ChangeAnchor, m_pSprite, m_iSavedAnchor);

    QWidget::closeEvent(event);
}


