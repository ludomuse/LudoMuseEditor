#include "CSoundInspector.h"

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
#include <QMessageBox>

// Personnal include
#include "CLineEdit.h"
#include "CProjectManager.h"
#include "CPathWidget.h"

CSoundInspector::CSoundInspector(QWidget *parent):
    QWidget(parent)
{
    // Default constructor
}


CSoundInspector::CSoundInspector(LM::CEntityNode* a_pNode, QWidget *parent):
    QWidget(parent),
    m_pNode(a_pNode)
{
    this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    // Create sound activation button
    QHBoxLayout* hLayoutCheck = new QHBoxLayout();
    m_pPlaySoundCheckButton = new QCheckBox(this);
    m_pPlaySoundCheckButton->setText(tr("Ajouter un son au sprite"));
    m_pPlaySoundCheckButton->setStyleSheet("QCheckBox { color: white; border-bottom : 0px }");
    hLayoutCheck->addWidget(m_pPlaySoundCheckButton);

    QHBoxLayout* hDistantLayout = new QHBoxLayout();
    m_pDistantCheckButton = new QCheckBox(this);
    m_pDistantCheckButton->setText(tr("Jouer sur l'autre tablette"));
    m_pDistantCheckButton->setStyleSheet("QCheckBox { color: white; border-bottom : 0px }");
    hDistantLayout->addWidget(m_pDistantCheckButton);

    QWidget* checkContainer = new QWidget();
    checkContainer->setLayout(hLayoutCheck);
    checkContainer->setMaximumHeight(100);
    checkContainer->setStyleSheet("border-bottom : 1px solid grey");
    checkContainer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    QWidget* distantContainer = new QWidget();
    distantContainer->setLayout(hDistantLayout);
    distantContainer->setMaximumHeight(100);
    distantContainer->setStyleSheet("border-bottom : 1px solid grey");
    distantContainer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // Create path widget
    QHBoxLayout* hLayoutPath= new QHBoxLayout();
    QLabel* pathTitle = new QLabel("Chemin :");
    pathTitle->setStyleSheet("QLabel{ color: white; border-bottom : 0px }");

    m_pPathField = new CLineEdit(ETypes::Sound, this);
    hLayoutPath->addWidget(pathTitle);
    hLayoutPath->addWidget(m_pPathField);
    QWidget* pathContainer = new QWidget();
    pathContainer->setLayout(hLayoutPath);
    pathContainer->setMaximumHeight(100);
    pathContainer->setStyleSheet("border-bottom : 1px solid grey");
    pathContainer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

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
    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(50,50,50,255));
    verticalLayout->addWidget(checkContainer);
    verticalLayout->addWidget(distantContainer);
    verticalLayout->addWidget(pathContainer);
    verticalLayout->addSpacerItem(new QSpacerItem(0,0, QSizePolicy::Preferred, QSizePolicy::Expanding));
    verticalLayout->addWidget(buttonContainer);
    this->setLayout(verticalLayout);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    //    this->layout()->addWidget(idContainer);
    //    this->layout()->addWidget(pathContainer);
    //    this->layout()->addWidget(anchorContainer);
    //    this->layout()->addWidget(sizeContainer);
    //    this->layout()->addWidget(buttonContainer);

    Initialize();
    // Connect path file dialog
//    connect(lineEdit, SIGNAL(pathChanged(QString)), this, SLOT(changePath(QString)));
//    connect(pathFileDialogButton, SIGNAL(clicked(bool)), this, SLOT(openPathFileDialog()));
    connect(m_pPlaySoundCheckButton, SIGNAL(toggled(bool)), m_pPathField, SLOT(setEnabled(bool)));
    connect(m_pPlaySoundCheckButton, SIGNAL(toggled(bool)), m_pDistantCheckButton, SLOT(setEnabled(bool)));
    connect(m_pPlaySoundCheckButton, SIGNAL(clicked(bool)), m_pPathField, SLOT(clear()));
    connect(okButton, SIGNAL(clicked(bool)), this, SLOT(validateChanges()));
    connect(backButton, SIGNAL(clicked(bool)), this, SLOT(discardChanges()));
}

void CSoundInspector::Initialize()
{
    std::pair<std::string, LM::CEventCallback>* pCallback = m_pNode->GetCallback("PlaySound");
    if (pCallback)
    {
        m_pPlaySoundCheckButton->toggle();
        QString argsString = QString::fromStdString(pCallback->second.getArg().m_sStringValue);
        std::vector<std::string> args = LM::StringSplit(argsString.toStdString());
        QString sValue(QString::fromStdString(args[0]));
        m_bSaveIsDistant = false;
        if (args.size() > 1 && args[1] == "distant")
        {
            m_pDistantCheckButton->toggle();
            m_bSaveIsDistant = true;
        }
        if (sValue.at(0) != '#')
        {
            m_sSavedPath = QString(CProjectManager::Instance()->QGetProjectPath()+ sValue);
        }
        m_pPathField->setText(m_sSavedPath);
    }
    else
    {
        m_pPathField->setEnabled(false);
        m_pDistantCheckButton->setEnabled(false);
    }
}

// SIGNALS ***********************************************

//void CSoundInspector::pathChanged(const QString& a_sPath)
//{
//    // Keep this for latter animation
//    QPalette pal(palette());
//    pal.setColor(QPalette::Text, QColor(0,0,0,255));
//    this->m_pPath->setPalette(pal);
//    this->m_pPath->update();

//    // Test if file exist!
//    QFile myFile;
//    myFile.setFileName(a_sPath);
//    if(myFile.exists())
//    {
//        //        QLabel* infoLabel = new QLabel("marche bien" + path);
//        //        this->layout()->addWidget(infoLabel);
//        qDebug()<<"validate with path :"<<a_sPath;
//        //        this->m_pSprite->SetPath(a_sPath.toStdString());
//    }
//    else
//    {
//        //        QLabel* infoLabel = new QLabel("marche pas" + path);
//        //        this->layout()->addWidget(infoLabel);
//        qDebug()<<"File doesn't seem to exist : "<<a_sPath;
//        QPalette pal(palette());
//        pal.setColor(QPalette::Text, QColor(255,0,0,255));
//        this->m_pPath->setPalette(pal);
//        this->m_pPath->update();
//    }
//}

bool CSoundInspector::applyPath(const QString& a_sPath)
{
    if (!m_pPlaySoundCheckButton->isChecked() || a_sPath.isEmpty())
    {
        emit removeSound(m_pNode);
        return false;
    }
    else
    {
        QString arg = a_sPath;
        if (a_sPath.at(0) != '#')
        {
            arg = arg.remove(CProjectManager::Instance()->QGetProjectPath());
        }
        if (m_pDistantCheckButton->isChecked())
        {
            arg += ":distant";
        }
        emit modifySound(m_pNode, GetEvent(), arg);
        return true;
    }
}

void CSoundInspector::validateChanges()
{
    emit closeInspector();
}

QString CSoundInspector::GetEvent()
{
    if (m_pNode->IsMovable())
    {
        return QString("Move");
    }
    else
    {
        return QString("Touch");
    }
}

void CSoundInspector::discardChanges()
{
    m_pPathField->setText(m_sSavedPath);
    m_pDistantCheckButton->setChecked(m_bSaveIsDistant);
    emit closeInspector();
}

void CSoundInspector::closeEvent (QCloseEvent *event)
{
    applyPath(m_pPathField->text());
    QWidget::closeEvent(event);
}

