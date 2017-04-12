#include "CDashboardInspector.h"
#include "Classes/Engine/Include/CFindEntityFromTypeVisitor.h"
#include "Classes/Engine/Include/CGridNode.h"

#include <QLabel>
#include <QGridLayout>
#include <QSpinBox>
#include <QPushButton>
#include <QSpacerItem>

CDashboardInspector::CDashboardInspector(LM::CSceneNode* a_pDashboard, QWidget *parent):
    QWidget(parent),
    m_pDashboard(a_pDashboard)
{

    Desc<LM::CNode> pNode;
    LM::CFindEntityFromTypeVisitor<LM::CGridNode> oVisitor(pNode);
    oVisitor.Traverse(m_pDashboard);
    if (pNode.IsValid())
    {
        m_pRewardGrid = static_cast<LM::CGridNode*>(pNode.Get());
    }


    QString sID = "Ecran de récompenses";
    QLabel* id = new QLabel(sID, this);
    this->setStyleSheet("QLabel{ color : white; padding : 6px;} QPushButton{ border : 1px solid white; padding : 6px; color : white;}"); // change text color for the widget
    id->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);

    QFrame* separator = new QFrame(this);
    separator->setFrameShape(QFrame::VLine);
    separator->setStyleSheet("QFrame{ color : white; width : 2px;}");


    QLabel* rowLabel = new QLabel(this);
    QLabel* colLabel = new QLabel(this);
    rowLabel->setText("Nombre de lignes :");
    rowLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    colLabel->setText("Nombre de colonnes :");
    colLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);


    m_pRowCountSpin = new QSpinBox(this);
    m_pColCountSpin = new QSpinBox(this);

    m_pRowCountSpin->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);
    m_pColCountSpin->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);

    m_pRowCountSpin->setMaximum(2);
    m_pRowCountSpin->setMinimum(1);
    m_pColCountSpin->setMaximum(5);
    m_pColCountSpin->setMinimum(1);

    m_pRowCountSpin->setValue(m_pRewardGrid->GetRowCount());
    m_pColCountSpin->setValue(m_pRewardGrid->GetColCount());

    QWidget* labels = new QWidget(this);
    QVBoxLayout* labelsLayout = new QVBoxLayout(labels);
    labels->setLayout(labelsLayout);
    labelsLayout->addWidget(rowLabel);
    labelsLayout->addWidget(colLabel);

    QWidget* spinners = new QWidget(this);
    QVBoxLayout* spinnersLayout = new QVBoxLayout(this);
    spinners->setLayout(spinnersLayout);
    spinnersLayout->addWidget(m_pRowCountSpin);
    spinnersLayout->addWidget(m_pColCountSpin);

    QPushButton* cancelButton = new QPushButton("Annuler", this);
    QPushButton* okButton = new QPushButton("Valider", this);

    QWidget* buttonsSpacerWidget = new QWidget(this);
    QVBoxLayout* buttonsSpacerLayout = new QVBoxLayout(buttonsSpacerWidget);
    buttonsSpacerWidget->setLayout(buttonsSpacerLayout);
    QSpacerItem* buttonsSpacer = new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);


    QWidget* buttonsWidget = new QWidget(this);
    QHBoxLayout* buttonsLayout = new QHBoxLayout(this);
    buttonsWidget->setLayout(buttonsLayout);

    buttonsLayout->addWidget(cancelButton);
    buttonsLayout->addWidget(okButton);

    buttonsSpacerLayout->addSpacerItem(buttonsSpacer);
    buttonsSpacerLayout->addWidget(buttonsWidget);


    QHBoxLayout* hDashboardInspectorLayout = new QHBoxLayout(this);
    this->setLayout(hDashboardInspectorLayout);
    hDashboardInspectorLayout->addWidget(id, Qt::AlignLeft);
    hDashboardInspectorLayout->addWidget(separator, Qt::AlignLeft);
    hDashboardInspectorLayout->addWidget(labels);
    hDashboardInspectorLayout->addWidget(spinners);

    QSpacerItem* spacer = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);
    hDashboardInspectorLayout->addSpacerItem(spacer);
    hDashboardInspectorLayout->addWidget(buttonsSpacerWidget);


    connect(okButton, SIGNAL(clicked(bool)), this, SLOT(ChangeGrid(bool)));
    connect(cancelButton, SIGNAL(clicked(bool)), this, SLOT(CancelChangeGrid(bool)));
}



void CDashboardInspector::ChangeGrid(bool a_bState)
{
    m_pRewardGrid->SetGrid(m_pRowCountSpin->value(), m_pColCountSpin->value());
}

void CDashboardInspector::CancelChangeGrid(bool a_bState)
{
    m_pRowCountSpin->setValue(m_pRewardGrid->GetRowCount());
    m_pColCountSpin->setValue(m_pRewardGrid->GetColCount());
}
