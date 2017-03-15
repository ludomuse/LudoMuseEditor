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
    QLabel* pTasks = new QLabel("Tasks");
    pTasks->setStyleSheet("QLabel{color : white;}");
    pTasks->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    hLayout->addWidget(pTasks);
    QLabel* pActions = new QLabel("Actions");
    pActions->setStyleSheet("QLabel{color : white;}");
    pActions->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    hLayout->addWidget(pActions);

    vLayout->addWidget(hLayout);


    QLabel* idTitle = new QLabel("ID :");
    idTitle->setStyleSheet("QLabel{color : white;}");
    idTitle->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    hLayoutId->addWidget(idTitle);
    hLayoutId->addWidget(id);


    QLineEdit* id = new QLineEdit(this);
    id->setPlaceholderText("id non définie");
    id->setText(QString::fromStdString(m_sSavedID));
    id->setAlignment(Qt::AlignLeft);
    QLabel* idTitle = new QLabel("ID :");
    idTitle->setStyleSheet("QLabel{color : white;}");
    idTitle->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    hLayoutId->addWidget(idTitle);
    hLayoutId->addWidget(id);
    QWidget* idContainer = new QWidget();
    idContainer->setLayout(hLayoutId);
    idContainer->setMaximumHeight(100);
    idContainer->setStyleSheet("border-bottom : 1px solid grey");
    idContainer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // Create path widget
    QHBoxLayout* hLayoutPath= new QHBoxLayout();

    QLabel* pathTitle = new QLabel("Chemin :");
    pathTitle->setStyleSheet("QLabel{color : white;}");
//    CPathWidget* pathEdit = new CPathWidget(QString::fromStdString(m_sSavedPath), ETypes::Image, this);
    CLineEdit* pathEdit = new CLineEdit(ETypes::Image, this);
    pathEdit->setText(QString::fromStdString(m_sSavedPath));
//    pathWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    hLayoutPath->addWidget(pathTitle);
    hLayoutPath->addWidget(pathEdit);
    QWidget* pathContainer = new QWidget();
    pathContainer->setLayout(hLayoutPath);
    pathContainer->setMaximumHeight(100);
    pathContainer->setStyleSheet("border-bottom : 1px solid grey");
    pathContainer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    // Create anchor widget
    QLabel* anchorTitle = new QLabel("Ancrage");
    anchorTitle->setAlignment(Qt::AlignCenter);
    anchorTitle->setStyleSheet("QLabel{color : white;}");

    CAnchorWidget* anchorWidget = new CAnchorWidget(m_iSavedAnchor, this);
    QVBoxLayout* vLayoutAnchor = new QVBoxLayout();
    vLayoutAnchor->addWidget(anchorTitle);
    vLayoutAnchor->addWidget(anchorWidget);
    QWidget* anchorContainer = new QWidget(this);
    anchorContainer->setLayout(vLayoutAnchor);
    anchorContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Create height and width widget
    QButtonGroup* radiobuttons = new QButtonGroup(this);
    QVBoxLayout* vLayoutSize = new QVBoxLayout();
    QHBoxLayout* hLayoutHeight = new QHBoxLayout();
    QHBoxLayout* hLayoutWidth = new QHBoxLayout();
    QWidget* heightContainer = new QWidget();
    heightContainer->setObjectName("heightContainer");
    QWidget* widthContainer = new QWidget();
    widthContainer->setObjectName("widthContainer");
    m_pHeightRadioButton = new QRadioButton(this);
    m_pWidthRadioButton = new QRadioButton(this);
    radiobuttons->addButton(m_pHeightRadioButton);
    radiobuttons->addButton(m_pWidthRadioButton);
    m_pWidthRadioButton->setAutoExclusive(true);
    m_pHeightRadioButton->setAutoExclusive(true);
    QLabel* widthTitle = new QLabel("Largeur :");
    widthTitle->setStyleSheet("QLabel{color : white;}");
    QLabel* heightTitle = new QLabel("Hauteur :");
    heightTitle->setStyleSheet("QLabel{color : white;}");
    m_pWidthSlider = new QSlider(Qt::Horizontal);
    m_pHeightSlider = new QSlider(Qt::Horizontal);
    m_pWidthSlider->setTickInterval(1);
    m_pWidthSlider->setMaximum(100);
    m_pWidthSlider->setMinimum(0);
    m_pWidthSlider->setSliderPosition(m_pSprite->GetWidth());
    m_pHeightSlider->setTickInterval(1);
    m_pHeightSlider->setMaximum(100);
    m_pHeightSlider->setMinimum(0);
    m_pHeightSlider->setSliderPosition(m_pSprite->GetHeight());
    m_pWidthValue = new QLineEdit(QString::number(m_pSprite->GetWidth()));
    m_pWidthValue->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_pWidthValue->setStyleSheet("QLineEdit{width : 25px;}");
    m_pHeightValue = new QLineEdit(QString::number(m_pSprite->GetHeight()));
    m_pHeightValue->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    m_pHeightValue->setStyleSheet("QLineEdit{width : 25px;}");
    hLayoutWidth->addWidget(m_pWidthRadioButton);
    hLayoutWidth->addWidget(widthTitle);
    hLayoutWidth->addWidget(m_pWidthSlider);
    hLayoutWidth->addWidget(m_pWidthValue);
    hLayoutHeight->addWidget(m_pHeightRadioButton);
    hLayoutHeight->addWidget(heightTitle);
    hLayoutHeight->addWidget(m_pHeightSlider);
    hLayoutHeight->addWidget(m_pHeightValue);
    heightContainer->setLayout(hLayoutHeight);
    widthContainer->setLayout(hLayoutWidth);
    QWidget* sizeContainer = new QWidget();
    vLayoutSize->addWidget(heightContainer);
    vLayoutSize->addWidget(widthContainer);
    sizeContainer->setLayout(vLayoutSize);
    sizeContainer->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    if(this->m_pSprite->GetWidth() == 0)
    {
        m_pHeightRadioButton->setChecked(true);
        //        m_pHeightRadioButton->setEnabled(false);
        m_pWidthRadioButton->setChecked(false);
        m_pWidthSlider->setEnabled(false);
        m_pWidthValue->setEnabled(false);
        widthContainer->parentWidget()->setStyleSheet("#widthContainer{border: none}");
        heightContainer->parentWidget()->setStyleSheet("#heightContainer{border: 1px dashed white}");
    }
    else
    {
        m_pHeightRadioButton->setChecked(false);
        m_pWidthRadioButton->setChecked(true);
        //        m_pWidthRadioButton->setEnabled(false);
        m_pHeightSlider->setEnabled(false);
        m_pHeightValue->setEnabled(false);
        heightContainer->setStyleSheet("#heightContainer{border: none}");
        widthContainer->parentWidget()->setStyleSheet("#widthContainer{border: 1px dashed white}");
    }

    QHBoxLayout* hLayoutButton = new QHBoxLayout();
    QPushButton* okButton = new QPushButton("Ok");
    QPushButton* backButton = new QPushButton("Annuler");
    hLayoutButton->addWidget(backButton);
    hLayoutButton->addWidget(okButton);
    QVBoxLayout* vLayoutButton = new QVBoxLayout();
    QPushButton* soundButton = new QPushButton("Editer son");
    vLayoutButton->addLayout(hLayoutButton);
    vLayoutButton->addWidget(soundButton);
    QWidget* buttonContainer = new QWidget();
    buttonContainer->setLayout(vLayoutButton);
    buttonContainer->setMaximumHeight(100);
    buttonContainer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    QVBoxLayout *verticalLayout = new QVBoxLayout();
    verticalLayout->addWidget(idContainer);
    verticalLayout->addWidget(pathContainer);
    verticalLayout->addWidget(anchorContainer);
    verticalLayout->addWidget(sizeContainer);
    verticalLayout->addSpacerItem(new QSpacerItem(0,0, QSizePolicy::Preferred, QSizePolicy::Expanding));
    verticalLayout->addWidget(buttonContainer);

    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(50,50,50,255));
    this->setLayout(verticalLayout);
    this->setAutoFillBackground(true);
    this->setPalette(pal);

//    this->m_pPath = path;
    connect(id, SIGNAL(textChanged(QString)), this, SLOT(changeID(QString)));

//    connect(pathEdit, SIGNAL(pathChanged(QString)), this, SLOT(changePath(QString)));
    connect(pathEdit, SIGNAL(textChanged(QString)), this, SLOT(changePath(QString)));

//    connect(pathFileDialogButton, SIGNAL(clicked(bool)), this, SLOT(openPathFileDialog()));

    connect(anchorWidget, SIGNAL(anchorChanged(int)), this, SLOT(changeAnchor(int)));

    // Connect slider and text value
    connect(m_pHeightSlider, SIGNAL(valueChanged(int)), this, SLOT(heightSliderChange(int)));
    connect(m_pWidthSlider, SIGNAL(valueChanged(int)), this, SLOT(widthSliderChange(int)));
    connect(m_pHeightValue, SIGNAL(textChanged(QString)), this, SLOT(heightTextChange(QString)));
    connect(m_pWidthValue, SIGNAL(textChanged(QString)), this, SLOT(widthTextChange(QString)));

    // Connect checkbox
    connect(m_pHeightRadioButton, SIGNAL(toggled(bool)), this, SLOT(checkHeight(bool)));
    connect(m_pWidthRadioButton, SIGNAL(toggled(bool)), this, SLOT(checkWidth(bool)));

    connect(okButton, SIGNAL(clicked(bool)), this, SLOT(validateChanges()));
    connect(backButton, SIGNAL(clicked(bool)), this, SLOT(discardChanges()));

    connect(soundButton, SIGNAL(clicked(bool)), this, SLOT(editSound()));
}

// SIGNALS ***********************************************

void CTeamNodeInspector::changeID(const QString &a_sID)
{
    this->m_pSprite->SetID(a_sID.toStdString());
}



void CTeamNodeInspector::validateChanges()
{
    emit closeInspector();
}

void CTeamNodeInspector::discardChanges()
{
    m_pTeamNode->SetTasks(m_oSavedTasks);
    emit closeInspector();
}



void CTeamNodeInspector::closeEvent (QCloseEvent *event)
{
    emit modifyTeamNode(m_pTeamNode);

////    ON_CC_THREAD(LM::CSpriteNode::ChangeAnchor, m_pSprite, m_iSavedAnchor);

    QWidget::closeEvent(event);
}

void CSpriteInspector::editSound()
{
    emit callSoundInspector();
}

