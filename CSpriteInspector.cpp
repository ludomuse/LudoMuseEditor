#include "CSpriteInspector.h"

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
#include <QCheckBox>
#include <QSlider>
#include <QLineEdit>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QSizePolicy>

// Personnal include
#include "CLineEdit.h"

CSpriteInspector::CSpriteInspector(QWidget *parent):
    QWidget(parent)
{
    // Default constructor
}


CSpriteInspector::CSpriteInspector(LM::CSpriteNode* a_pSprite, QWidget *parent):
    QWidget(parent),
    m_pSprite(a_pSprite),
    m_pPath(Q_NULLPTR)
{
    // Construction de tout les champs swag
    this->m_iCurrentAnchor = this->m_pSprite->GetAnchor();

    QHBoxLayout* hLayoutId= new QHBoxLayout();
    QLineEdit* id = new QLineEdit(this);
    id->setPlaceholderText("id non définie");
    id->setText(QString(this->m_pSprite->GetID().c_str()));
    id->setAlignment(Qt::AlignLeft);
    QLabel* idTitle = new QLabel("ID :");
    idTitle->setStyleSheet("QLabel{color : white;}");
    idTitle->setAlignment(Qt::AlignRight);
    hLayoutId->addWidget(idTitle);
    hLayoutId->addWidget(id);
    QWidget* idContainer = new QWidget();
    idContainer->setLayout(hLayoutId);
    idContainer->setMaximumHeight(100);
    idContainer->setStyleSheet("border-bottom : 1px solid grey");
    idContainer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    // Create path widget
    QHBoxLayout* hLayoutPath= new QHBoxLayout();
    CLineEdit* path = new CLineEdit(this);
    path->setPlaceholderText("id non définie");
    path->setText(QString(this->m_pSprite->GetPath().c_str()));
    path->setAlignment(Qt::AlignLeft);
    path->setAttribute(Qt::WA_TranslucentBackground, false);
    QLabel* pathTitle = new QLabel("Chemin :");
    pathTitle->setStyleSheet("QLabel{color : white;}");
    QPushButton* pathFileDialogButton = new QPushButton();
    pathFileDialogButton->setText("...");
    pathFileDialogButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    pathTitle->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    hLayoutPath->addWidget(pathTitle);
    hLayoutPath->addWidget(path);
    hLayoutPath->addWidget(pathFileDialogButton);
    QWidget* pathContainer = new QWidget();
    pathContainer->setLayout(hLayoutPath);
    pathContainer->setMaximumHeight(100);
    pathContainer->setStyleSheet("border-bottom : 1px solid grey");
    pathContainer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    // Create anchor widget
    QLabel* anchorTitle = new QLabel("Ancrage");
    anchorTitle->setAlignment(Qt::AlignCenter);
    anchorTitle->setStyleSheet("QLabel{color : white;}");
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

    // Create height and width widget
    QVBoxLayout* vLayoutSize = new QVBoxLayout();
    QHBoxLayout* hLayoutHeight = new QHBoxLayout();
    QHBoxLayout* hLayoutWidth = new QHBoxLayout();
    QWidget* heightContainer = new QWidget();
    heightContainer->setObjectName("heightContainer");
    QWidget* widthContainer = new QWidget();
    widthContainer->setObjectName("widthContainer");
    m_pHeightCheckBox = new QCheckBox(heightContainer);
    m_pWidthCheckBox = new QCheckBox(widthContainer);
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
    hLayoutWidth->addWidget(m_pWidthCheckBox);
    hLayoutWidth->addWidget(widthTitle);
    hLayoutWidth->addWidget(m_pWidthSlider);
    hLayoutWidth->addWidget(m_pWidthValue);
    hLayoutHeight->addWidget(m_pHeightCheckBox);
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
        m_pHeightCheckBox->setChecked(true);
        m_pHeightCheckBox->setEnabled(false);
        m_pWidthCheckBox->setChecked(false);
        m_pWidthSlider->setEnabled(false);
        m_pWidthValue->setEnabled(false);
        widthContainer->parentWidget()->setStyleSheet("#widthContainer{border: none}");
        heightContainer->parentWidget()->setStyleSheet("#heightContainer{border: 1px dashed white}");
    }
    else
    {
        m_pHeightCheckBox->setChecked(false);
        m_pWidthCheckBox->setChecked(true);
        m_pWidthCheckBox->setEnabled(false);
        m_pHeightSlider->setEnabled(false);
        m_pHeightValue->setEnabled(false);
        heightContainer->setStyleSheet("#heightContainer{border: none}");
        widthContainer->parentWidget()->setStyleSheet("#widthContainer{border: 1px dashed white}");
    }

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
    pal.setColor(QPalette::Background, QColor(50,50,50,255));
    this->setLayout(verticalLayout);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    this->layout()->addWidget(idContainer);
    this->layout()->addWidget(pathContainer);
    this->layout()->addWidget(anchorContainer);
    this->layout()->addWidget(sizeContainer);
    this->layout()->addWidget(buttonContainer);

    this->m_pPath = path;

    connect(path, SIGNAL(textChanged(QString)), this, SLOT(pathChanged(QString)));
    connect(okButton, SIGNAL(clicked(bool)), this, SLOT(validatePath()));
    connect(pathFileDialogButton, SIGNAL(clicked(bool)), this, SLOT(openPathFileDialog()));
    connect(backButton, SIGNAL(clicked(bool)), this, SLOT(closeInspectorSlot()));

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

    // Connect slider and text value
    connect(m_pHeightSlider, SIGNAL(valueChanged(int)), this, SLOT(heightSliderChange(int)));
    connect(m_pWidthSlider, SIGNAL(valueChanged(int)), this, SLOT(widthSliderChange(int)));
    connect(m_pHeightValue, SIGNAL(textChanged(QString)), this, SLOT(heightTextChange(QString)));
    connect(m_pWidthValue, SIGNAL(textChanged(QString)), this, SLOT(widthTextChange(QString)));

    // Connect checkbox
    connect(m_pHeightCheckBox, SIGNAL(stateChanged(int)), this, SLOT(checkHeight(int)));
    connect(m_pWidthCheckBox, SIGNAL(stateChanged(int)), this, SLOT(checkWidth(int)));
}



void CSpriteInspector::closeInspectorSlot()
{
    emit closeInspector();
}



// SIGNALS ***********************************************

void CSpriteInspector::pathChanged(const QString& a_sPath)
{
    // Keep this for latter animation
    QPalette pal(palette());
    pal.setColor(QPalette::Text, QColor(0,0,0,255));
    this->m_pPath->setPalette(pal);
    this->m_pPath->update();
}

void CSpriteInspector::validatePath()
{
    // Test if file exist!
    QFile myFile;
    QString path = this->m_pPath->text();
    myFile.setFileName(path);
    if(myFile.exists())
    {
//        QLabel* infoLabel = new QLabel("marche bien" + path);
//        this->layout()->addWidget(infoLabel);
        qDebug()<<"validate with path :"<<path;
        this->m_pSprite->SetPath(path.toStdString());
    }
    else
    {
//        QLabel* infoLabel = new QLabel("marche pas" + path);
//        this->layout()->addWidget(infoLabel);
        qDebug()<<"File doesn't seem to exist : "<<path;
        QPalette pal(palette());
        pal.setColor(QPalette::Text, QColor(255,0,0,255));
        this->m_pPath->setPalette(pal);
        this->m_pPath->update();
    }
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


void CSpriteInspector::openPathFileDialog()
{
    QFileDialog* fileDialog = new QFileDialog();

    //qDebug()<<currentDir.absolutePath();
    //qDebug()<<" Image path : "<<this->m_pPath->text();
    QDir currentDir = QDir::currentPath();

    std::vector<std::string> pathToImage = StringSplit(this->m_pPath->text().toStdString(), '/');

    for (int i = 0; i < pathToImage.size() - 1; ++i)
    {
        currentDir.cd(pathToImage[i].c_str());
    }
    fileDialog->setDirectory(currentDir);
    fileDialog->selectFile(pathToImage[pathToImage.size() - 1].c_str());

    fileDialog->setNameFilter("Images (*.jpeg, *.jpg, *.png");

    connect(fileDialog, SIGNAL(fileSelected(QString)), this, SLOT(newPathSelected(QString)));

    fileDialog->show();
}


void CSpriteInspector::newPathSelected(QString a_sPath)
{
    QDir currentDir = QDir::currentPath();
    QString currentPath = currentDir.absolutePath();
    a_sPath.remove(currentPath + "/");
    this->m_pPath->setText(a_sPath);
    qDebug()<<"etablish new path via file windonw -"<<a_sPath;
}


void CSpriteInspector::heightSliderChange(int a_iValue)
{
    qDebug()<< "Change slider height value to:"<<a_iValue;
    this->m_pHeightValue->setText(QString::number(a_iValue));
    this->m_pSprite->SetHeight(a_iValue);
}

void CSpriteInspector::widthSliderChange(int a_iValue)
{
    qDebug()<< "Change slider width value to:"<<a_iValue;
    this->m_pWidthValue->setText(QString::number(a_iValue));
    this->m_pSprite->SetWidth(a_iValue);
}

void CSpriteInspector::heightTextChange(const QString& a_rText)
{
    qDebug()<< "Change height value to:"<<a_rText;
    // Try convert string to int
    bool succes;
    int value = a_rText.toInt(&succes);
    if(succes)
    {
        m_pHeightSlider->setSliderPosition(value);
        m_pSprite->SetHeight(value);
    }
}

void CSpriteInspector::widthTextChange(const QString& a_rText)
{
    qDebug()<< "Change width value to:"<<a_rText;
    // Try convert string to int
    bool succes;
    int value = a_rText.toInt(&succes);
    if(succes)
    {
        m_pWidthSlider->setSliderPosition(value);
        m_pSprite->SetWidth(value);
    }
}


void CSpriteInspector::checkHeight(int a_rState)
{
    if(a_rState == 0)
    {
        qDebug("Swag");
        // Do Nothing
    }
    else
    {
        m_pWidthCheckBox->setEnabled(true);
        m_pHeightCheckBox->setChecked(true);
        m_pWidthCheckBox->setChecked(false);
        m_pHeightCheckBox->parentWidget()->setStyleSheet("#heightContainer{border: 1px dashed white}");
        m_pWidthCheckBox->parentWidget()->setStyleSheet("#widthContainer{border: none}");
        m_pHeightCheckBox->setEnabled(false);
        m_pHeightSlider->setEnabled(true);
        m_pHeightValue->setEnabled(true);
        // Disabling width fields
        m_pWidthSlider->setEnabled(false);
        m_pWidthValue->setEnabled(false);
        // Setting size propertie
        m_pSprite->SetWidth(0);
        m_pSprite->SetHeight(m_pHeightSlider->value());
    }
}

void CSpriteInspector::checkWidth(int a_rState)
{

    qDebug("Swag 2");
    if(a_rState == 0)
    {
        // Do nothing
    }
    else
    {
        m_pHeightCheckBox->setEnabled(true);
        m_pHeightCheckBox->setChecked(false);
        m_pWidthCheckBox->setChecked(true);
        m_pHeightCheckBox->parentWidget()->setStyleSheet("#heightContainer{border: none}");
        m_pWidthCheckBox->parentWidget()->setStyleSheet("#widthContainer{border: 1px dashed white}");
        m_pWidthCheckBox->setEnabled(false);
        m_pWidthSlider->setEnabled(true);
        m_pWidthValue->setEnabled(true);
        // Disabling height fields
        m_pHeightSlider->setEnabled(false);
        m_pHeightValue->setEnabled(false);
        // Setting size propertie
        m_pSprite->SetWidth(m_pWidthSlider->value());
    }
}
