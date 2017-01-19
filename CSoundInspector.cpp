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
    m_pPlaySoundCheckButton->setText("Ajouter un son au sprite");
    m_pPlaySoundCheckButton->setStyleSheet("QCheckBox { color: white; border-bottom : 0px }");
    hLayoutCheck->addWidget(m_pPlaySoundCheckButton);

    QWidget* checkContainer = new QWidget();
    checkContainer->setLayout(hLayoutCheck);
    checkContainer->setMaximumHeight(100);
    checkContainer->setStyleSheet("border-bottom : 1px solid grey");
    checkContainer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    checkContainer->setLayout(hLayoutCheck);


    // Create path widget
    QHBoxLayout* hLayoutPath= new QHBoxLayout();
    QLabel* pathTitle = new QLabel("Chemin :");
    pathTitle->setStyleSheet("QLabel{ color: white; border-bottom : 0px }");
//    QStringList lExtensions;
//    lExtensions << "mp3" << "wav";
//    CLineEdit* path = new CLineEdit(lExtensions, this);
//    path->setPlaceholderText("chemin non défini");
//    path->setAlignment(Qt::AlignLeft);
//    path->setAttribute(Qt::WA_TranslucentBackground, false);

        CPathWidget* pathWidget = new CPathWidget("", ETypes::Sound, this);
        hLayoutPath->addWidget(pathTitle);
        hLayoutPath->addWidget(pathWidget);
        QWidget* pathContainer = new QWidget();
        pathContainer->setLayout(hLayoutPath);
        pathContainer->setMaximumHeight(100);
        pathContainer->setStyleSheet("border-bottom : 1px solid grey");
        pathContainer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
//    QPushButton* pathFileDialogButton = new QPushButton();
//    pathFileDialogButton->setText("...");
//    pathFileDialogButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
//    pathTitle->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
//    hLayoutPath->addWidget(pathTitle);
//    hLayoutPath->addWidget(path);
//    hLayoutPath->addWidget(pathFileDialogButton);
//    QWidget* pathContainer = new QWidget(this);
//    pathContainer->setLayout(hLayoutPath);
//    pathContainer->setMaximumHeight(40);
//    pathContainer->setStyleSheet("border-bottom : 1px solid grey");
//    pathContainer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
//    m_pPath = path;

    // Create listener selector
    QButtonGroup* radiobuttons = new QButtonGroup(this);
    QVBoxLayout* vLayoutListener = new QVBoxLayout();
    m_pInitiateRadioButton = new QRadioButton(this);
    m_pInitiateRadioButton->setText("Initialisation");
    m_pInitiateRadioButton->setStyleSheet("QRadioButton { color: white; border-bottom : 0px }");
    m_pValidateRadioButton = new QRadioButton(this);
    m_pValidateRadioButton->setText("Validation");
    m_pValidateRadioButton->setStyleSheet("QRadioButton { color: white; border-bottom : 0px }");
    m_pInteractRadioButton = new QRadioButton(this);
    m_pInteractRadioButton->setText("Interaction");
    m_pInteractRadioButton->setStyleSheet("QRadioButton { color: white; border-bottom : 0px }");
    m_pDisplaceRadioButton = new QRadioButton(this);
    m_pDisplaceRadioButton->setText("Déplacement");
    m_pDisplaceRadioButton->setStyleSheet("QRadioButton { color: white; border-bottom : 0px }");
    radiobuttons->addButton(m_pInitiateRadioButton);
    radiobuttons->addButton(m_pValidateRadioButton);
    radiobuttons->addButton(m_pInteractRadioButton);
    radiobuttons->addButton(m_pDisplaceRadioButton);
    m_pInitiateRadioButton->setAutoExclusive(true);
    m_pValidateRadioButton->setAutoExclusive(true);
    m_pInteractRadioButton->setAutoExclusive(true);
    m_pDisplaceRadioButton->setAutoExclusive(true);
    QLabel* listenerTitle = new QLabel("Déclencher le son lors de :");
    listenerTitle->setStyleSheet("QLabel{ color: white; border-bottom : 0px }");
    vLayoutListener->addWidget(listenerTitle);
    vLayoutListener->addWidget(m_pInitiateRadioButton);
    vLayoutListener->addWidget(m_pValidateRadioButton);
    vLayoutListener->addWidget(m_pInteractRadioButton);
    vLayoutListener->addWidget(m_pDisplaceRadioButton);
    //    heightContainer->setLayout(hLayoutHeight);
    //    widthContainer->setLayout(hLayoutWidth);
    //    QWidget* sizeContainer = new QWidget();
    //    vLayoutSize->addWidget(heightContainer);
    //    vLayoutSize->addWidget(widthContainer);
    //    sizeContainer->setLayout(vLayoutSize);
    //    sizeContainer->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    QWidget* listenerContainer = new QWidget();
    listenerContainer->setLayout(vLayoutListener);
    listenerContainer->setMaximumHeight(160);
    listenerContainer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    QHBoxLayout* hLayoutButton = new QHBoxLayout();
    QPushButton* okButton = new QPushButton("Ok");
    QPushButton* backButton = new QPushButton("Retour");
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
    verticalLayout->addWidget(pathContainer);
    verticalLayout->addWidget(listenerContainer);
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
    pathWidget->SetPath(m_pPath);
    // Connect path file dialog
    connect(pathWidget, SIGNAL(pathChanged(QString)), this, SLOT(changePath(QString)));
//    connect(pathFileDialogButton, SIGNAL(clicked(bool)), this, SLOT(openPathFileDialog()));

    connect(okButton, SIGNAL(clicked(bool)), this, SLOT(validateChanges()));
    connect(backButton, SIGNAL(clicked(bool)), this, SLOT(discardChanges()));
}

void CSoundInspector::Initialize()
{
    std::pair<std::string, LM::CEventCallback>* pCallback = m_pNode->GetCallback("PlaySound");
    //    m_pDisplaceRadioButton->setEnabled(false);
    if (m_pNode->IsMovable())
    {
        m_pInteractRadioButton->hide();
    }
    else
    {
        m_pDisplaceRadioButton->hide();
    }
    if (pCallback)
    {
        m_pPlaySoundCheckButton->toggle();
        m_pPath = QString(CProjectManager::Instance()->QGetProjectPath()+
                         QString::fromStdString(pCallback->second.getArg().m_sStringValue));
        if(pCallback->first == "Init")
        {
            m_pInitiateRadioButton->setChecked(true);
        }
        else if (pCallback->first == "Validate")
        {
            m_pValidateRadioButton->setChecked(true);
        }
        else if (pCallback->first == "Touch")
        {
            m_pInteractRadioButton->setChecked(true);
        }
        else if (pCallback->first == "Move")
        {
            m_pDisplaceRadioButton->setChecked(true);
        }
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

bool CSoundInspector::PathIsValid()
{
//    QFileInfo myFile;
//    myFile.setFile(m_pPath);
//    return myFile.exists()/* && (myFile.suffix() == "mp3" || myFile.suffix() == "wav")*/;
    return !m_pPath.isEmpty();
}

void CSoundInspector::changePath(const QString& a_sPath)
{
//    QFile myFile;
//    myFile.setFileName(a_sPath);
//    if(myFile.exists())
//    {        qDebug()<<"validate with path :"<<a_sPath;
//        m_pPath = a_sPath;
//    }
//    else
//    {
//        qDebug()<<"File doesn't seem to exist : "<<a_sPath;
//    }
    m_pPath = a_sPath;
}

void CSoundInspector::validateChanges()
{
    //    // Test if file exist!
    //    QFile myFile;
    //    QString path = this->m_pPath->text();
    //    myFile.setFileName(path);
    //    if(myFile.exists())
    //    {
    //        //        QLabel* infoLabel = new QLabel("marche bien" + path);
    //        //        this->layout()->addWidget(infoLabel);
    //        qDebug()<<"validate with path :"<<path;
    //        this->m_pSprite->SetPath(path.toStdString());
    //        emit modifySprite(m_pSprite);
    //    }
    //    else
    //    {
    //        //        QLabel* infoLabel = new QLabel("marche pas" + path);
    //        //        this->layout()->addWidget(infoLabel);
    //        qDebug()<<"File doesn't seem to exist : "<<path;
    //        QPalette pal(palette());
    //        pal.setColor(QPalette::Text, QColor(255,0,0,255));
    //        this->m_pPath->setPalette(pal);
    //        this->m_pPath->update();
    //    }
    if (!m_pPlaySoundCheckButton->isChecked())
    {
        emit removeSound(m_pNode);
        emit closeInspector();
    }
    else
    {
        if (GetEvent() == "")
        {
            QMessageBox* msgBox = new QMessageBox(QMessageBox::Warning, "Erreur", "Veuillez selectionner un type d'évènement");
            msgBox->exec();
        }
        else if (!PathIsValid())
        {
            QMessageBox* msgBox = new QMessageBox(QMessageBox::Warning, "Erreur", "Veuillez entrer un chemin valide");
            msgBox->exec();
        }
        else
        {
            QString path = m_pPath.remove(CProjectManager::Instance()->QGetProjectPath());
            emit modifySound(m_pNode, GetEvent(), path);
            emit closeInspector();
        }
    }
}

QString CSoundInspector::GetEvent()
{
    if(m_pInitiateRadioButton->isChecked())
    {
        return QString("Init");
    }
    else if (m_pValidateRadioButton->isChecked())
    {
        return QString("Validate");
    }
    else if (m_pInteractRadioButton->isChecked())
    {
        return QString("Touch");
    }
    else if (m_pDisplaceRadioButton->isChecked())
    {
        return QString("Move");
    }
    else
    {
        return QString("");
    }
}

void CSoundInspector::discardChanges()
{
    emit closeInspector();
}

//void CSoundInspector::openPathFileDialog()
//{
//    QFileDialog* fileDialog = new QFileDialog();

//    //qDebug()<<currentDir.absolutePath();
//    //qDebug()<<" Image path : "<<this->m_pPath->text();
//    //    QDir currentDir = QDir::currentPath();
//    QDir currentDir = QDir(CProjectManager::Instance()->QGetProjectPath());

//    std::vector<std::string> pathToSound = StringSplit(this->m_pPath->text().toStdString(), '/');

//    for (int i = 0; i < pathToSound.size() - 1; ++i)
//    {
//        currentDir.cd(pathToSound[i].c_str());
//    }
//    fileDialog->setDirectory(currentDir);
//    fileDialog->selectFile(pathToSound[pathToSound.size() - 1].c_str());

//    fileDialog->setNameFilter("Audio format (*.mp3 *.wav)");

//    connect(fileDialog, SIGNAL(directoryEntered(QString)), this, SLOT(newDirectoryEntered(QString)));
//    connect(fileDialog, SIGNAL(fileSelected(QString)), this, SLOT(newPathSelected(QString)));

//    fileDialog->show();
//}


//void CSoundInspector::newPathSelected(QString a_sPath)
//{
//    QDir currentDir = QDir::currentPath();
//    QString currentPath = currentDir.absolutePath();
//    a_sPath.remove(currentPath + "/");
//    this->m_pPath->setText(a_sPath);
//    qDebug()<<"etablish new path via file windonw -"<<a_sPath;
//}

//void CSoundInspector::newDirectoryEntered(QString a_sPath)
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

void CSoundInspector::closeEvent (QCloseEvent *event)
{
    //    m_pSprite->SetPath(m_sSavedPath);
    //    m_pSprite->SetWidth(m_iSavedWidth);
    //    m_pSprite->SetHeight(m_iSavedHeight);
    //    ON_CC_THREAD(LM::CSpriteNode::ChangeAnchor, m_pSprite, m_iSavedAnchor);
    //    discardChanges();
    QWidget::closeEvent(event);
}

