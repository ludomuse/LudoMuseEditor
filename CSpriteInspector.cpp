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
#include <QRadioButton>
#include <QButtonGroup>

// Personnal include
#include "CLineEdit.h"
#include "CPathWidget.h"
#include "CProjectManager.h"
#include "CSoundInspector.h"
#include "CAnchorWidget.h"
#include "ETypes.h"

CSpriteInspector::CSpriteInspector(QWidget *parent):
    QWidget(parent)
{
    // Default constructor
}


CSpriteInspector::CSpriteInspector(LM::CSpriteNode* a_pSprite, QWidget *parent):
    QWidget(parent),
    m_pSprite(a_pSprite),
//    m_pPath(Q_NULLPTR),
    m_sSavedID(a_pSprite->GetID()),
    m_sSavedPath(a_pSprite->GetPath()),
    m_iSavedHeight(a_pSprite->GetHeight()),
    m_iSavedWidth(a_pSprite->GetWidth()),
    m_iSavedAnchor(a_pSprite->GetAnchor())
{
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    // Construction de tous les champs swag
//    this->m_iCurrentAnchor = this->m_pSprite->GetAnchor();

    QHBoxLayout* hLayoutId= new QHBoxLayout();
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
//    QStringList lExtensions;
//    lExtensions << "jpg" << "png" << "jpeg";
//    CLineEdit* path = new CLineEdit(lExtensions, this);
//    path->setPlaceholderText("id non définie");
//    path->setText(QString(this->m_pSprite->GetPath().c_str()));
//    path->setAlignment(Qt::AlignLeft);
//    path->setAttribute(Qt::WA_TranslucentBackground, false);
//    QLabel* pathTitle = new QLabel("Chemin :");
//    pathTitle->setStyleSheet("QLabel{color : white;}");
//    QPushButton* pathFileDialogButton = new QPushButton();
//    pathFileDialogButton->setText("...");
//    pathFileDialogButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
//    pathTitle->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
//    hLayoutPath->addWidget(pathTitle);
//    hLayoutPath->addWidget(path);
//    hLayoutPath->addWidget(pathFileDialogButton);
    QLabel* pathTitle = new QLabel("Chemin :");
    pathTitle->setStyleSheet("QLabel{color : white;}");
    CPathWidget* pathWidget = new CPathWidget(QString::fromStdString(m_sSavedPath), ETypes::Image, this);
//    pathWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    hLayoutPath->addWidget(pathTitle);
    hLayoutPath->addWidget(pathWidget);
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
    QPushButton* backButton = new QPushButton("Retour");
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

    connect(pathWidget, SIGNAL(pathChanged(QString)), this, SLOT(changePath(QString)));
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

void CSpriteInspector::changeID(const QString &a_sID)
{
    this->m_pSprite->SetID(a_sID.toStdString());
}

void CSpriteInspector::changePath(const QString& a_sPath)
{
    // Keep this for latter animation
//    QPalette pal(palette());
//    pal.setColor(QPalette::Text, QColor(0,0,0,255));
//    this->m_pPath->setPalette(pal);
//    this->m_pPath->update();

    // Test if file exist!
//    QFile myFile;
//    myFile.setFileName(a_sPath);
//    if(myFile.exists())
//    {
        //        QLabel* infoLabel = new QLabel("marche bien" + path);
        //        this->layout()->addWidget(infoLabel);
//        qDebug()<<"validate with path :"<<a_sPath;
//        this->m_pSprite->SetPath(a_sPath.toStdString());
//    }
//    else
//    {
        //        QLabel* infoLabel = new QLabel("marche pas" + path);
        //        this->layout()->addWidget(infoLabel);
//        qDebug()<<"File doesn't seem to exist : "<<a_sPath;
//        QPalette pal(palette());
//        pal.setColor(QPalette::Text, QColor(255,0,0,255));
//        this->m_pPath->setPalette(pal);
//        this->m_pPath->update();
//    }
    this->m_pSprite->SetPath(a_sPath.toStdString());
}

void CSpriteInspector::validateChanges()
{
    m_sSavedID = m_pSprite->GetID();
    m_sSavedPath = m_pSprite->GetPath();
    m_iSavedHeight = m_pSprite->GetHeight();
    m_iSavedWidth = m_pSprite->GetWidth();
    m_iSavedAnchor = m_pSprite->GetAnchor();
    emit modifySprite(m_pSprite);
    emit closeInspector();
}

void CSpriteInspector::discardChanges()
{
    emit closeInspector();
}

//void CSpriteInspector::setAnchor()
//{
//    QObject* obj = sender();
//    QPushButton* anchorButton = dynamic_cast<QPushButton*>(obj);
//    if(anchorButton){
//        qDebug("Conversion en bouton faite");
//        this->m_vAnchorButtons.at(this->m_iCurrentAnchor)->setText(QString::number(this->m_iCurrentAnchor));
//        this->m_iCurrentAnchor = anchorButton->text().toInt();
//        anchorButton->setText("["+ anchorButton->text() +"]");
//        ON_CC_THREAD(LM::CSpriteNode::ChangeAnchor, this->m_pSprite, this->m_iCurrentAnchor);
//    }
//}

void CSpriteInspector::changeAnchor(int a_iAnchorID)
{
//    ON_CC_THREAD(LM::CSpriteNode::ChangeAnchor, this->m_pSprite, a_iAnchorID);
    m_pSprite->ChangeAnchor(a_iAnchorID);
}

//void CSpriteInspector::openPathFileDialog()
//{
//    QFileDialog* fileDialog = new QFileDialog();

//    //qDebug()<<currentDir.absolutePath();
//    //qDebug()<<" Image path : "<<this->m_pPath->text();
//    //    QDir currentDir = QDir::currentPath();
//    QDir currentDir = QDir(CProjectManager::Instance()->QGetProjectPath());

//    std::vector<std::string> pathToImage = StringSplit(this->m_pPath->text().toStdString(), '/');

//    for (int i = 0; i < pathToImage.size() - 1; ++i)
//    {
//        currentDir.cd(pathToImage[i].c_str());
//    }
//    fileDialog->setDirectory(currentDir);
//    fileDialog->selectFile(pathToImage[pathToImage.size() - 1].c_str());

//    fileDialog->setNameFilter("Images (*.jpeg, *.jpg, *.png)");

//    connect(fileDialog, SIGNAL(directoryEntered(QString)), this, SLOT(newDirectoryEntered(QString)));
//    connect(fileDialog, SIGNAL(fileSelected(QString)), this, SLOT(newPathSelected(QString)));

//    fileDialog->show();
//}


//void CSpriteInspector::newPathSelected(QString a_sPath)
//{
//    QDir currentDir = QDir::currentPath();
//    QString currentPath = currentDir.absolutePath();
//    a_sPath.remove(currentPath + "/");
//    this->m_pPath->setText(a_sPath);
//    qDebug()<<"etablish new path via file windonw -"<<a_sPath;
//}

//void CSpriteInspector::newDirectoryEntered(QString a_sPath)
//{
//    QString sProjectPath = CProjectManager::Instance()->QGetProjectPath();
//    if (!a_sPath.contains(sProjectPath))
//    {
//        QFileDialog* fileDialog = dynamic_cast<QFileDialog*>(QObject::sender());
//        if (fileDialog != Q_NULLPTR)
//        {
//            fileDialog->setDirectory(sProjectPath);
//        }
//    }
//}

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


void CSpriteInspector::checkHeight(bool a_rState)
{
    if(a_rState)
    {
        //        m_pWidthRadioButton->setEnabled(true);
        //        m_pHeightRadioButton->setChecked(true);
        //        m_pWidthRadioButton->setChecked(false);
        m_pHeightRadioButton->parentWidget()->setStyleSheet("#heightContainer{border: 1px dashed white}");
        m_pWidthRadioButton->parentWidget()->setStyleSheet("#widthContainer{border: none}");
        //        m_pHeightRadioButton->setEnabled(false);
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

void CSpriteInspector::checkWidth(bool a_rState)
{
    if(a_rState)
    {
        //        m_pHeightRadioButton->setEnabled(true);
        //        m_pHeightRadioButton->setChecked(false);
        //        m_pWidthRadioButton->setChecked(true);
        m_pHeightRadioButton->parentWidget()->setStyleSheet("#heightContainer{border: none}");
        m_pWidthRadioButton->parentWidget()->setStyleSheet("#widthContainer{border: 1px dashed white}");
        //        m_pWidthRadioButton->setEnabled(false);
        m_pWidthSlider->setEnabled(true);
        m_pWidthValue->setEnabled(true);
        // Disabling height fields
        m_pHeightSlider->setEnabled(false);
        m_pHeightValue->setEnabled(false);
        // Setting size propertie
        m_pSprite->SetWidth(m_pWidthSlider->value());
    }
}

void CSpriteInspector::closeEvent (QCloseEvent *event)
{
    m_pSprite->SetID(m_sSavedID);
    m_pSprite->SetPath(m_sSavedPath);
    m_pSprite->SetWidth(m_iSavedWidth);
    m_pSprite->SetHeight(m_iSavedHeight);
//    ON_CC_THREAD(LM::CSpriteNode::ChangeAnchor, m_pSprite, m_iSavedAnchor);
    m_pSprite->ChangeAnchor(m_iSavedAnchor);
    QWidget::closeEvent(event);
}

void CSpriteInspector::editSound()
{
    emit callSoundInspector();
}

