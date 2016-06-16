#include "CSpriteInspector.h"

// Include Kernel to reach ON_CC_THREAD macro
#include "Classes/Engine/Include/CKernel.h"

// Include QT
#include <QDebug>
#include <QFile>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QLineEdit>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>

CSpriteInspector::CSpriteInspector(QWidget *parent):
    QWidget(parent)
{
    // Default constructor
}


CSpriteInspector::CSpriteInspector(LM::CSpriteNode* a_pSprite, QWidget *parent):
    QWidget(parent),
    m_pSprite(a_pSprite)
{
    // Construction de tout les champs swag
    this->m_iCurrentAnchor = this->m_pSprite->GetAnchor();

    QHBoxLayout* hLayoutId= new QHBoxLayout();
    QLineEdit* id = new QLineEdit(this);
    id->setPlaceholderText("id non définie");
    id->setText(QString(this->m_pSprite->GetID().c_str()));
    id->setAlignment(Qt::AlignLeft);
    QLabel* idTitle = new QLabel("ID :");
    idTitle->setAlignment(Qt::AlignRight);
    hLayoutId->addWidget(idTitle);
    hLayoutId->addWidget(id);
    QWidget* idContainer = new QWidget();
    idContainer->setLayout(hLayoutId);
    idContainer->setMaximumHeight(100);
    idContainer->setStyleSheet("border-bottom : 1px solid grey");
    idContainer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    QHBoxLayout* hLayoutPath= new QHBoxLayout();
    QLineEdit* path = new QLineEdit(this);
    path->setPlaceholderText("id non définie");
    path->setText(QString(this->m_pSprite->GetPath().c_str()));
    path->setAlignment(Qt::AlignLeft);
    QLabel* pathTitle = new QLabel("Chemin :");
    pathTitle->setAlignment(Qt::AlignRight);
    hLayoutPath->addWidget(pathTitle);
    hLayoutPath->addWidget(path);
    QWidget* pathContainer = new QWidget();
    pathContainer->setLayout(hLayoutPath);
    pathContainer->setMaximumHeight(100);
    pathContainer->setStyleSheet("border-bottom : 1px solid grey");
    pathContainer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    // Create anchor widget
    QLabel* anchorTitle = new QLabel("Ancrage");
    anchorTitle->setAlignment(Qt::AlignCenter);
    QPushButton* anchor1Button = new QPushButton("1");
    QPushButton* anchor2Button = new QPushButton("2");
    QPushButton* anchor3Button = new QPushButton("3");
    QHBoxLayout* hLayoutAnchorFirstLine = new QHBoxLayout();
    hLayoutAnchorFirstLine->addWidget(anchor1Button);
    hLayoutAnchorFirstLine->addWidget(anchor2Button);
    hLayoutAnchorFirstLine->addWidget(anchor3Button);
    QWidget* anchorFirstLineContainer = new QWidget();
    anchorFirstLineContainer->setLayout(hLayoutAnchorFirstLine);
    QPushButton* anchor8Button = new QPushButton("8");
    QPushButton* anchor0Button = new QPushButton("0");
    QPushButton* anchor4Button = new QPushButton("4");
    QHBoxLayout* hLayoutAnchorSecondLine = new QHBoxLayout();
    hLayoutAnchorSecondLine->addWidget(anchor8Button);
    hLayoutAnchorSecondLine->addWidget(anchor0Button);
    hLayoutAnchorSecondLine->addWidget(anchor4Button);
    QWidget* anchorSecondLineContainer = new QWidget();
    anchorSecondLineContainer->setLayout(hLayoutAnchorSecondLine);
    QPushButton* anchor7Button = new QPushButton("7");
    QPushButton* anchor6Button = new QPushButton("6");
    QPushButton* anchor5Button = new QPushButton("5");
    QHBoxLayout* hLayoutAnchorThirdLine = new QHBoxLayout();
    switch(this->m_iCurrentAnchor){
    case 1:
        anchor1Button->setText("[1]");
        break;
    case 2:
        anchor2Button->setText("[2]");
        break;
    case 3:
        anchor3Button->setText("[3]");
        break;
    case 4:
        anchor4Button->setText("[4]");
        break;
    case 5:
        anchor5Button->setText("[5]");
        break;
    case 6:
        anchor6Button->setText("[6]");
        break;
    case 7:
        anchor7Button->setText("[7]");
        break;
    case 8:
        anchor8Button->setText("[8]");
        break;
    default:
        anchor0Button->setText("[0]");
        break;
    }
    hLayoutAnchorThirdLine->addWidget(anchor7Button);
    hLayoutAnchorThirdLine->addWidget(anchor6Button);
    hLayoutAnchorThirdLine->addWidget(anchor5Button);
    QWidget* anchorThirdLineContainer = new QWidget();
    anchorThirdLineContainer->setLayout(hLayoutAnchorThirdLine);
    QVBoxLayout* vLayoutAnchorScheme = new QVBoxLayout();
    vLayoutAnchorScheme->addWidget(anchorFirstLineContainer);
    vLayoutAnchorScheme->addWidget(anchorSecondLineContainer);
    vLayoutAnchorScheme->addWidget(anchorThirdLineContainer);
    QWidget* anchorScheme = new QWidget();
    QPalette palAnchorScheme(palette());
    palAnchorScheme.setColor(QPalette::Background, QColor(255, 255, 255));
    anchorScheme->setLayout(vLayoutAnchorScheme);
    anchorScheme->setAutoFillBackground(true);
    anchorScheme->setPalette(palAnchorScheme);
    QVBoxLayout* vLayoutAnchor = new QVBoxLayout();
    vLayoutAnchor->addWidget(anchorTitle);
    vLayoutAnchor->addWidget(anchorScheme);
    QWidget* anchorContainer = new QWidget();
    anchorContainer->setLayout(vLayoutAnchor);
    anchorContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout* vLayoutSize = new QVBoxLayout();
    QHBoxLayout* hLayoutHeight = new QHBoxLayout();
    QHBoxLayout* hLayoutWidth = new QHBoxLayout();
    QWidget* heightContainer = new QWidget();
    QWidget* widthContainer = new QWidget();
    QLabel* widthTitle = new QLabel("Width :");
    QLabel* heightTitle = new QLabel("Height :");
    QSlider*  widthSlider = new QSlider(Qt::Horizontal);
    QSlider*  heightSlider = new QSlider(Qt::Horizontal);
    widthSlider->setTickInterval(1);
    widthSlider->setMaximum(100);
    widthSlider->setMinimum(0);
    widthSlider->setSliderPosition(50);
    heightSlider->setTickInterval(1);
    heightSlider->setMaximum(100);
    heightSlider->setMinimum(0);
    heightSlider->setSliderPosition(50);
    hLayoutWidth->addWidget(widthTitle);
    hLayoutWidth->addWidget(widthSlider);
    hLayoutHeight->addWidget(heightTitle);
    hLayoutHeight->addWidget(heightSlider);
    heightContainer->setLayout(hLayoutHeight);
    widthContainer->setLayout(hLayoutWidth);
    QWidget* sizeContainer = new QWidget();
    vLayoutSize->addWidget(heightContainer);
    vLayoutSize->addWidget(widthContainer);
    sizeContainer->setLayout(vLayoutSize);
    sizeContainer->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QHBoxLayout* hLayoutButton = new QHBoxLayout();
    QPushButton* okButton = new QPushButton("Ok");
    QPushButton* backButton = new QPushButton("Retour");
    hLayoutButton->addWidget(backButton);
    hLayoutButton->addWidget(okButton);
    QWidget* buttonContainer = new QWidget();
    buttonContainer->setLayout(hLayoutButton);
    buttonContainer->setMaximumHeight(100);

    QVBoxLayout *verticalLayout = new QVBoxLayout();
    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(100,100,100));
    this->setLayout(verticalLayout);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    this->layout()->addWidget(idContainer);
    this->layout()->addWidget(pathContainer);
    this->layout()->addWidget(anchorContainer);
    this->layout()->addWidget(sizeContainer);
    this->layout()->addWidget(buttonContainer);

    connect(path, SIGNAL(textChanged(QString)), this, SLOT(pathChanged(QString)));
    connect(okButton, SIGNAL(clicked(bool)), this, SLOT(validatePath()));

    // Connect Anchor and stock it in object
    connect(anchor0Button, SIGNAL(clicked(bool)), this, SLOT(setAnchor()));
    connect(anchor1Button, SIGNAL(clicked(bool)), this, SLOT(setAnchor()));
    connect(anchor2Button, SIGNAL(clicked(bool)), this, SLOT(setAnchor()));
    connect(anchor3Button, SIGNAL(clicked(bool)), this, SLOT(setAnchor()));
    connect(anchor4Button, SIGNAL(clicked(bool)), this, SLOT(setAnchor()));
    connect(anchor5Button, SIGNAL(clicked(bool)), this, SLOT(setAnchor()));
    connect(anchor6Button, SIGNAL(clicked(bool)), this, SLOT(setAnchor()));
    connect(anchor7Button, SIGNAL(clicked(bool)), this, SLOT(setAnchor()));
    connect(anchor8Button, SIGNAL(clicked(bool)), this, SLOT(setAnchor()));
    this->m_vAnchorButtons.push_back(anchor0Button);
    this->m_vAnchorButtons.push_back(anchor1Button);
    this->m_vAnchorButtons.push_back(anchor2Button);
    this->m_vAnchorButtons.push_back(anchor3Button);
    this->m_vAnchorButtons.push_back(anchor4Button);
    this->m_vAnchorButtons.push_back(anchor5Button);
    this->m_vAnchorButtons.push_back(anchor6Button);
    this->m_vAnchorButtons.push_back(anchor7Button);
    this->m_vAnchorButtons.push_back(anchor8Button);
}

void CSpriteInspector::pathChanged(const QString& a_sPath)
{
    this->m_sPath = a_sPath;
}

void CSpriteInspector::validatePath()
{
    qDebug("trying to valid");
    // Test if file exist!
    QFile myFile;
    myFile.setFileName("./" + this->m_sPath);
    if(myFile.exists())
    {
        QLabel* infoLabel = new QLabel("marche bien" + myFile.fileName());
        this->layout()->addWidget(infoLabel);
        qDebug()<<"validate with path :"<<myFile.fileName();
    }
    else
    {
        QLabel* infoLabel = new QLabel("marche pas" + myFile.fileName());
        this->layout()->addWidget(infoLabel);
        qDebug()<<"File doesn't seem to exist : "<<myFile.fileName();
    }
    this->m_pSprite->SetPath(this->m_sPath.toStdString());
    //ON_CC_THREAD(LM::CSpriteNode::SetPath, this->m_pSprite, this->m_sPath.toStdString());
    //qDebug()<<"new path set";
}

void CSpriteInspector::setAnchor()
{
    QObject* obj = sender();
    QPushButton* anchorButton = dynamic_cast<QPushButton*>(obj);
    if(anchorButton){
        qDebug("Conversion en bouton faite");
        this->m_vAnchorButtons.at(this->m_iCurrentAnchor)->setText(QString::number(this->m_iCurrentAnchor));
        this->m_iCurrentAnchor = anchorButton->text().toInt();
        anchorButton->setText("["+ anchorButton->text() +"]");
        ON_CC_THREAD(LM::CSpriteNode::ChangeAnchor, this->m_pSprite, this->m_iCurrentAnchor);
    }
}


