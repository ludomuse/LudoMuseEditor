#include "CAddSceneWizard.h"

#include "CTemplateManager.h"
#include "CTemplate.h"
#include "CTemplatePushButton.h"

// Include QT
#include <QBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QScrollArea>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QDebug>
#include <QMessageBox>
#include <QToolBox>

CAddSceneWizard::CAddSceneWizard(int a_iActivePlayer, const std::vector<std::string>& a_rSceneIDP1,
                                 const std::vector<std::string>& a_rSceneIDP2,QWidget* a_pParent,
                                 QString a_sPreviousID1, QString a_sPreviousID2):
    QDialog(a_pParent),
    m_sPreviousID1(a_sPreviousID1),
    m_sPreviousID2(a_sPreviousID2),
    m_rSceneIDP1(a_rSceneIDP1),
    m_rSceneIDP2(a_rSceneIDP2),
    m_iActivePlayer(a_iActivePlayer),
    m_pComboBoxID(Q_NULLPTR),
    m_pComboBoxID2(Q_NULLPTR),
    m_pCurrentTemplateButton(Q_NULLPTR),
    m_bScreensSwaped(false)
{
    QHBoxLayout* hWizardLayout = new QHBoxLayout();

    // Create scroll Area and his content
    QScrollArea* templateScrollArea = new QScrollArea();
    templateScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    templateScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    templateScrollArea->setStyleSheet("QLabel{ border : 1px solid white}");
    templateScrollArea->setMinimumWidth(300);
    templateScrollArea->setMaximumWidth(300);
    QVBoxLayout* vTemplateScrollAreaLayout = new QVBoxLayout();
    templateScrollArea->setLayout(vTemplateScrollAreaLayout);


    m_pGamesTemplatesWidget = CreateTemplatesWidget("games");
    m_pInfoTemplatesWidget = CreateTemplatesWidget("info");
    m_pInitTemplatesWidget = CreateTemplatesWidget("init");

    templateScrollArea->setWidget(m_pGamesTemplatesWidget);
    templateScrollArea->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    QToolBox* toolBox = new QToolBox();
    //toolBox->setStyleSheet("QToolBox::tab{ background-color : rgb(60,60,60)}} QToolBox::tab::title { color : white}");
    toolBox->setMinimumWidth(280);
    toolBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    toolBox->addItem(m_pInitTemplatesWidget, "Ecrans d'initialisation");
    toolBox->addItem(m_pInfoTemplatesWidget, "Ecrans d'information et de narration");
    toolBox->addItem(m_pGamesTemplatesWidget, "Ecrans de jeu");


    // Create preview title
    m_pPreviewTitle = new QLabel("");
    m_pPreviewTitle->setAlignment(Qt::AlignCenter);

    m_pDescriptionLabel = new QLabel("");
    m_pDescriptionLabel->setAlignment(Qt::AlignCenter);

    // Create preview widget
    QWidget* previewAndOptionWidget = new QWidget();
    QVBoxLayout* vPreviewOptionLayout = new QVBoxLayout();
    m_pPreviewWidget = new QWidget();
    QVBoxLayout* vPreviewLayout = new QVBoxLayout();
    vPreviewLayout->setContentsMargins(0, 0, 0, 0);
    m_pPreviewWidget->setLayout(vPreviewLayout);
    m_pPreviewWidget->setMinimumHeight(250);
    m_pPreviewWidget->setMinimumWidth(250);
    m_pPreviewWidget->setStyleSheet("QWidget{ background-color : white;}");

    // Create previous screen comboBox
    m_pComboBoxWidget = new QWidget();
    QHBoxLayout* hComboBoxLayout = new QHBoxLayout();
    QLabel* idComboBoxTitle = new QLabel("écran précédent : ");
    m_pComboBoxID = new QComboBox();
    if(m_sPreviousID1 != Q_NULLPTR)
    {
        FillComboBox(1, m_sPreviousID1);
    }
    else
    {
        FillComboBox(1, "");
    }
    m_pComboBoxID->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    hComboBoxLayout->addWidget(idComboBoxTitle);
    hComboBoxLayout->addWidget(m_pComboBoxID);
    m_pComboBoxWidget->setLayout(hComboBoxLayout);


    // Create otpion widget
    m_pOptionWidget = new QWidget();
    QHBoxLayout* hOptionLayout = new QHBoxLayout();
    QWidget* playerWidget = new QWidget();
    QHBoxLayout* hPlayerLayout = new QHBoxLayout();
    m_pPlayer1CheckBox = new QCheckBox("Joueur 1");
    hPlayerLayout->addWidget(m_pPlayer1CheckBox);
    hPlayerLayout->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    playerWidget->setLayout(hPlayerLayout);


    QWidget* leftPart = new QWidget();
    QVBoxLayout* vLeftPartLayout = new QVBoxLayout();
    QWidget* idWidget = new QWidget();
    QHBoxLayout* hIdLayout = new QHBoxLayout();
    QLabel* idTitle = new QLabel("id :");
    m_pNewID = new QLineEdit();
    m_pNewID->setPlaceholderText("scene id");
    hIdLayout->addWidget(idTitle);
    hIdLayout->addWidget(m_pNewID);
    idWidget->setLayout(hIdLayout);
//    QWidget* soundWidget = new QWidget();
//    QHBoxLayout* hSoundLayout = new QHBoxLayout();
//    QLabel* soundTitle = new QLabel("Fond sonore :");
//    m_pSoundPath = new QLineEdit();
//    m_pSoundPath->setPlaceholderText("Piste Audio");
//    hSoundLayout->addWidget(soundTitle);
//    hSoundLayout->addWidget(m_pSoundPath);
//    soundWidget->setLayout(hSoundLayout);
    vLeftPartLayout->addWidget(idWidget);
//    vLeftPartLayout->addWidget(soundWidget);
    leftPart->setLayout(vLeftPartLayout);
    hOptionLayout->addWidget(leftPart);
    m_pOptionWidget->setLayout(hOptionLayout);

    // Create button widget
    QWidget* buttonsWidget = new QWidget();
    QHBoxLayout* hButtonsLayout = new QHBoxLayout();
    QPushButton* backButton = new QPushButton("Retour");
    QPushButton* validateButton = new QPushButton("Valide");
    hButtonsLayout->addWidget(backButton);
    hButtonsLayout->addWidget(validateButton);
    buttonsWidget->setLayout(hButtonsLayout);

    // Fill Preview and option widget
    vPreviewOptionLayout->addWidget(playerWidget);
    vPreviewOptionLayout->addWidget(m_pPreviewWidget);
    vPreviewOptionLayout->addWidget(m_pComboBoxWidget);
    vPreviewOptionLayout->addWidget(m_pOptionWidget);
    previewAndOptionWidget->setLayout(vPreviewOptionLayout);


    // Create preview widget
    QWidget* previewAndOptionWidget2 = new QWidget();
    QVBoxLayout* vPreviewOptionLayout2 = new QVBoxLayout();
    m_pPreviewWidget2 = new QWidget();
    QVBoxLayout* vPreviewLayout2 = new QVBoxLayout();
    vPreviewLayout2->setContentsMargins(0, 0, 0, 0);
    m_pPreviewWidget2->setLayout(vPreviewLayout2);
    m_pPreviewWidget2->setMinimumHeight(250);
    m_pPreviewWidget2->setMinimumWidth(250);
    m_pPreviewWidget2->setStyleSheet("QWidget{ background-color : white;}");

    // Create previous screen comboBox
    m_pComboBoxWidget2 = new QWidget();
    QHBoxLayout* hComboBoxLayout2 = new QHBoxLayout();
    QLabel* idComboBoxTitle2 = new QLabel("écran précédent : ");
    m_pComboBoxID2 = new QComboBox();
    if(m_sPreviousID2 != Q_NULLPTR)
    {
        FillComboBox(2, m_sPreviousID2);
    }
    else
    {
        FillComboBox(2, "");
    }
    m_pComboBoxID2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    hComboBoxLayout2->addWidget(idComboBoxTitle2);
    hComboBoxLayout2->addWidget(m_pComboBoxID2);
    m_pComboBoxWidget2->setLayout(hComboBoxLayout2);
    // Create otpion widget
    m_pOptionWidget2 = new QWidget();
    QHBoxLayout* hOptionLayout2 = new QHBoxLayout();

    QWidget* playerWidget2 = new QWidget();
    QHBoxLayout* hPlayerLayout2 = new QHBoxLayout();
    m_pPlayer2CheckBox = new QCheckBox("Joueur 2");
    hPlayerLayout2->addWidget(m_pPlayer2CheckBox);
    hPlayerLayout2->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    // initialise check box
    if(a_iActivePlayer == 1)
    {
        m_pPlayer2CheckBox->setChecked(true);
        m_pPlayer1CheckBox->setChecked(false);
//        m_pComboBoxID->setEnabled(false);
    }
    else if(a_iActivePlayer == 0)
    {
        m_pPlayer1CheckBox->setChecked(true);
        m_pPlayer2CheckBox->setChecked(false);
//        m_pComboBoxID2->setEnabled(false);
    }
    else
    {
        m_pPlayer1CheckBox->setChecked(true);
        m_pPlayer2CheckBox->setChecked(true);
//        m_pComboBoxID2->setEnabled(false);
    }
    playerWidget2->setLayout(hPlayerLayout2);



    QWidget* leftPart2 = new QWidget();
    QVBoxLayout* vLeftPartLayout2 = new QVBoxLayout();
    QWidget* idWidget2 = new QWidget();
    QHBoxLayout* hIdLayout2 = new QHBoxLayout();
    QLabel* idTitle2 = new QLabel("id :");
    m_pNewID2 = new QLineEdit();
    m_pNewID2->setPlaceholderText("scene id");
    hIdLayout2->addWidget(idTitle2);
    hIdLayout2->addWidget(m_pNewID2);
    idWidget2->setLayout(hIdLayout2);


    vLeftPartLayout2->addWidget(idWidget2);
//    vLeftPartLayout2->addWidget(soundWidget2);
    leftPart2->setLayout(vLeftPartLayout2);
    hOptionLayout2->addWidget(leftPart2);
    m_pOptionWidget2->setLayout(hOptionLayout2);
    vPreviewOptionLayout2->addWidget(playerWidget2);
    vPreviewOptionLayout2->addWidget(m_pPreviewWidget2);
    vPreviewOptionLayout2->addWidget(m_pComboBoxWidget2);
    vPreviewOptionLayout2->addWidget(m_pOptionWidget2);
    previewAndOptionWidget2->setLayout(vPreviewOptionLayout2);

    // Create swap widget
    m_pSwapButton = new QPushButton("->\r\n<-", this);
    m_pSwapButton ->setContentsMargins(1,5,1,5);
    m_pSwapButton ->setMaximumHeight(50);
    m_pSwapButton ->setMaximumWidth(50);
    m_pSwapButton->setEnabled(false);


    QHBoxLayout* hPreviewsLayout = new QHBoxLayout();
    hPreviewsLayout->addWidget(previewAndOptionWidget);
    hPreviewsLayout->addWidget(m_pSwapButton );
    hPreviewsLayout->addWidget(previewAndOptionWidget2);
    QWidget* previewsWidget = new QWidget();
    previewsWidget->setLayout(hPreviewsLayout);
    QVBoxLayout* vWizardLayout = new QVBoxLayout();
    vWizardLayout->addWidget(m_pPreviewTitle);
    vWizardLayout->addWidget(m_pDescriptionLabel);
    vWizardLayout->addWidget(previewsWidget);
    vWizardLayout->addWidget(buttonsWidget);
    QWidget* mainWidget = new QWidget();
    mainWidget->setLayout(vWizardLayout);
    mainWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    mainWidget->setMinimumWidth(800);
    mainWidget->adjustSize();

    hWizardLayout->addWidget(toolBox);
    hWizardLayout->addWidget(mainWidget);
    this->setLayout(hWizardLayout);

    // Connect pushButton && checkbox
    connect(backButton, SIGNAL(clicked(bool)), this, SLOT(clickOnBack(bool)));
    connect(validateButton, SIGNAL(clicked(bool)), this, SLOT(clickOnValidate(bool)));
    connect(m_pSwapButton, SIGNAL(clicked(bool)), this, SLOT(swapScreens()));
    connect(m_pPlayer1CheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeActivePlayer()));
    connect(m_pPlayer2CheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeActivePlayer()));


    // Set active field
    this->changeActivePlayer();
}

// SLOTS

void CAddSceneWizard::clickOnBack(bool)
{
    this->close();
}

void CAddSceneWizard::clickOnValidate(bool)
{
    if(m_pCurrentTemplateButton == Q_NULLPTR)
    {
        this->OpenModalDialog("Selectionnez un template");
        return;
    }
    if(!m_pPlayer1CheckBox->isChecked() && !m_pPlayer2CheckBox->isChecked())
    {
        this->OpenModalDialog("Selectionnez au moins un joueur");
        return;
    }

    // Establish player id return and test id field!
    int idReturn;
    if(m_pPlayer1CheckBox->isChecked())
    {
        if(m_pPlayer2CheckBox->isChecked())
        {
            if(this->m_pCurrentTemplateButton->GetTemplate()->IsGame()){
                idReturn = 3;
                if(m_pNewID->text().isEmpty() || m_pNewID2->text().isEmpty())
                {
                    this->OpenModalDialog("Rentrez un identifiant de scene pour les deux joueurs");
                    return;
                }
            }
            else
            {
                idReturn = 2; // P1 & P2
                if(m_pNewID->text().isEmpty()/* || m_pNewID2->text().isEmpty()*/)
                {
                    this->OpenModalDialog("Rentrez un identifiant de scene pour les deux joueurs");
                    return;
                }
            }
        }
        else
        {
            idReturn = 0; // Only P1
            if(m_pNewID->text().isEmpty())
            {
                this->OpenModalDialog("Rentrez un identifiant de scene pour le joueur 1");
                return;
            }
        }
    }
    else
    {
        idReturn = 1; // Only P2
        if(m_pNewID2->text().isEmpty())
        {
            this->OpenModalDialog("Rentrez un identifiant de scene pour le joueur 2");
            return;
        }
    }

    if(m_pNewID->text() == m_pNewID2->text() && idReturn == 3)
    {
        this->OpenModalDialog("Les deux identifiants doivent être différents");
        return;
    }

    if(m_pPlayer1CheckBox->isChecked() && (FindExistingID(m_pNewID->text().toStdString(), 0) ||
                                           FindExistingID(m_pNewID->text().toStdString(), 1)))
    {
        this->OpenModalDialog("L'identifiant de scène doit être unique");
        return;
    }

    if(m_pPlayer2CheckBox->isChecked() &&
            (!m_pPlayer1CheckBox->isChecked() ||
             m_pCurrentTemplateButton->GetTemplate()->IsGame()) &&
            (FindExistingID(m_pNewID2->text().toStdString(), 0) ||
             FindExistingID(m_pNewID2->text().toStdString(), 1)))
    {
        this->OpenModalDialog("L'identifiant de scène doit être unique");
        return;
    }

    QString previousID = m_pComboBoxID->currentText();
    QString previousID2 = m_pComboBoxID2->currentText();
    if(previousID == "début") // adding scene after empty id -> adding at the beginning
    {
        previousID = "";
    }
    if(previousID2 == "début")
    {
        previousID2 = "";
    }

    // Check if adding game scene
    if(this->m_pCurrentTemplateButton->GetTemplate()->IsGame())
    {
        if(!this->m_bScreensSwaped) // Change template Number in signal
        {
            emit addGameScene(previousID, m_pNewID->text(), previousID2, m_pNewID2->text(),
                              m_pCurrentTemplateButton->GetTemplate(), 0, 1);
            this->close();
            return;
        }
        else
        {
            emit addGameScene(previousID, m_pNewID->text(), previousID2, m_pNewID2->text(),
                              m_pCurrentTemplateButton->GetTemplate(), 1, 0);
            this->close();
            return;
        }
    }
    // Adding simple new scene
    if(idReturn == 2)
    {
//        emit addTwoScene(previousID, m_pNewID->text(), previousID2, m_pNewID2->text(), m_pCurrentTemplateButton->GetTemplate());
        emit addSharedScene(previousID, previousID2, m_pNewID->text(), m_pCurrentTemplateButton->GetTemplate());
    }
    else if(idReturn == 1) // Add scene on player 2 timeline
    {
        emit addOneScene(previousID2, m_pNewID2->text(), idReturn, m_pCurrentTemplateButton->GetTemplate());
    }
    else // Add scene in player 1 timeline
    {
        emit addOneScene(previousID, m_pNewID->text(), idReturn, m_pCurrentTemplateButton->GetTemplate());
    }
    this->close();
}



void CAddSceneWizard::changeActivePlayer()
{
    QString dummy("");
    if(m_pPlayer1CheckBox->isChecked())
    {
        if(m_pPlayer2CheckBox->isChecked())
        {
            // Both player -> only end available
            //this->FillComboBox(0, dummy);
            this->SetEnabledPlayerField(0, true);
            this->SetEnabledPlayerField(1, true);
            m_pNewID2->setEnabled(false);
        }
        else
        {
            // Only P1 scene id available
            this->SetEnabledPlayerField(0, true);
            this->SetEnabledPlayerField(1, false);
        }
//        m_pNewID->setEnabled(true);
//        m_pNewID2->setEnabled(false);
    }
    else if(m_pPlayer2CheckBox->isChecked())
    {
        // Only P2 scene available
        this->SetEnabledPlayerField(0, false);
        this->SetEnabledPlayerField(1, true);
//        m_pNewID->setEnabled(false);
//        m_pNewID2->setEnabled(true);
    }
    else
    {
        // Both uncheck
        this->SetEnabledPlayerField(0, false);
        this->SetEnabledPlayerField(1, false);
//        m_pNewID->setEnabled(false);
//        m_pNewID2->setEnabled(false);
    }
}

void CAddSceneWizard::swapScreens()
{
    m_bScreensSwaped = !m_bScreensSwaped;
    this->UpdatePreview();
}

void CAddSceneWizard::setCurrentTemplate(CTemplatePushButton* a_pTemplatePushButton)
{
    // TODO
    if(m_pCurrentTemplateButton != Q_NULLPTR)
    {
        m_pCurrentTemplateButton->Unfocus();
    }
    m_pCurrentTemplateButton = a_pTemplatePushButton;
    CTemplate* pTemplate = a_pTemplatePushButton->GetTemplate();
    m_pPreviewTitle->setText(pTemplate->GetName());
    m_pDescriptionLabel->setText(pTemplate->GetDescription());

    if(pTemplate->IsGame())
    {
        m_pSwapButton->setEnabled(true);
        m_pPlayer1CheckBox->setChecked(true);
        m_pPlayer2CheckBox->setChecked(true);
        this->SetEnabledPlayerField(0, true);
        this->SetEnabledPlayerField(1, true);
        m_pPlayer1CheckBox->setEnabled(false);
        m_pPlayer2CheckBox->setEnabled(false);
    }
    else
    {
        m_pSwapButton->setEnabled(false);

        if(m_iActivePlayer == 0) // P1
        {
            m_pPlayer1CheckBox->setChecked(true);
            m_pPlayer2CheckBox->setChecked(false);
            this->SetEnabledPlayerField(0, true);
            this->SetEnabledPlayerField(1, false);
        }
        else if (m_iActivePlayer == 1) // P2
        {
            m_pPlayer1CheckBox->setChecked(false);
            m_pPlayer2CheckBox->setChecked(true);
            this->SetEnabledPlayerField(0, false);
            this->SetEnabledPlayerField(1, true);

        }
        else
        {
            m_pPlayer1CheckBox->setChecked(true);
            m_pPlayer2CheckBox->setChecked(true);
            this->SetEnabledPlayerField(0, true);
            this->SetEnabledPlayerField(1, false);

        }
        m_pPlayer1CheckBox->setEnabled(true);
        m_pPlayer2CheckBox->setEnabled(true);
    }
    m_bScreensSwaped = false; // reset swaping state
    this->UpdatePreview();
}

void CAddSceneWizard::FillComboBox(int a_iPlayerID, const QString& a_rActiveScene)
{
    int index = 1;
    bool activeSceneFound = false;
    if(a_iPlayerID == 1)
    {
        if(m_pComboBoxID != Q_NULLPTR)
        {
            m_pComboBoxID->clear();
        }
        m_pComboBoxID->addItem("début");
        for(std::string id : m_rSceneIDP1)
        {
            if(id.empty()) // ignore empty id
            {
                continue;
            }
            m_pComboBoxID->addItem(QString(id.c_str()));
            // Test if active scene id is found in player id list
            // If active screen is empty jump this step
            if(QString(id.c_str()) == a_rActiveScene)
            {
                m_pComboBoxID->setCurrentIndex(index);
                activeSceneFound = true;
            }
            index++;
        }
        m_pComboBoxID->setEnabled(true);
    }
    if(a_iPlayerID == 2)
    {
        if(m_pComboBoxID2 != Q_NULLPTR)
        {
            m_pComboBoxID2->clear();
        }
        m_pComboBoxID2->addItem("début");
        for(std::string id : m_rSceneIDP2)
        {
            if(id.empty()) // Ignore empty id
            {
                continue;
            }
            m_pComboBoxID2->addItem(QString(id.c_str()));

            // Test if active scene id is found in player id list
            // If active screen is empty jump this step
            if(QString(id.c_str()) == a_rActiveScene)
            {
                m_pComboBoxID2->setCurrentIndex(index);
                activeSceneFound = true;
            }
            index++;
        }
        m_pComboBoxID2->setEnabled(true);
    }

    if(!activeSceneFound)
    {
        if(a_iPlayerID == 2)
        {
            m_pComboBoxID2->setCurrentIndex(0);
        }
        else
        {
            m_pComboBoxID->setCurrentIndex(0);
        }
    }
}

void CAddSceneWizard::OpenModalDialog(const QString &a_sText)
{
    QMessageBox* pEmptyId = new QMessageBox();
    pEmptyId->setText(a_sText);
    pEmptyId->addButton("Ok", QMessageBox::AcceptRole);
    pEmptyId->show();
    pEmptyId->setModal(true);
}

void CAddSceneWizard::SetEnabledPlayerField(int a_iPlayerID, bool a_bEnabled)
{
    if(a_iPlayerID == 1)
    {
        this->m_pComboBoxID2->setEnabled(a_bEnabled);
        this->m_pNewID2->setEnabled(a_bEnabled);
//        this->m_pSoundPath2->setEnabled(a_bEnabled);

        this->m_pPreviewWidget2->setEnabled(a_bEnabled);
        this->m_pComboBoxWidget2->setEnabled(a_bEnabled);
        this->m_pOptionWidget2->setEnabled(a_bEnabled);
    }
    else
    {
        this->m_pComboBoxID->setEnabled(a_bEnabled);
        this->m_pNewID->setEnabled(a_bEnabled);
//        this->m_pSoundPath->setEnabled(a_bEnabled);

        this->m_pPreviewWidget->setEnabled(a_bEnabled);
        this->m_pComboBoxWidget->setEnabled(a_bEnabled);
        this->m_pOptionWidget->setEnabled(a_bEnabled);
    }
}

void CAddSceneWizard::UpdatePreview()
{
    //qDebug()<< "width :"<< m_pPreviewWidget->width() << " Height :"<< m_pPreviewWidget->height();
    if(m_pCurrentTemplateButton == Q_NULLPTR)
    {
        qDebug()<<"no selected template, cannot update preview";
        return;
    }

    // Clear old preview's content
    QLayout* previewLayout = this->m_pPreviewWidget->layout();
    QLayoutItem *child;
    while ((child = previewLayout->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }
    QLayout* previewLayout2 = this->m_pPreviewWidget2->layout();
    QLayoutItem *child2;
    while ((child2 = previewLayout2->takeAt(0)) != 0) {
        delete child2->widget();
        delete child2;
    }
    //  Adding picture in background of both preview
    QPixmap scaled = m_pCurrentTemplateButton->GetTemplate()->GetImage().scaledToWidth(m_pPreviewWidget->width(), Qt::FastTransformation);
    QPixmap scaled2;
    if(!(m_pCurrentTemplateButton->GetTemplate()->IsGame()))
    {
        scaled2= m_pCurrentTemplateButton->GetTemplate()->GetImage().scaledToWidth(m_pPreviewWidget2->width(), Qt::FastTransformation);
    }
    else
    {
        scaled2= m_pCurrentTemplateButton->GetTemplate()->GetImage2().scaledToWidth(m_pPreviewWidget2->width(), Qt::FastTransformation);
    }
    QLabel *label = new QLabel(this);
    label->setPixmap(scaled);
    QLabel *label2 = new QLabel(this);
    label2->setPixmap(scaled2);
    if(m_pCurrentTemplateButton->GetTemplate()->IsGame() && m_bScreensSwaped)
    {
        previewLayout->addWidget(label2);
        previewLayout2->addWidget(label);
    }
    else
    {
        previewLayout->addWidget(label);
        previewLayout2->addWidget(label2);
    }
}

void CAddSceneWizard::UnfocusTemplates()
{
    QLayout* templatesLayout = m_pGamesTemplatesWidget->layout();
    for (int i = 0; i < templatesLayout->count(); i++)
    {
        QWidget* widget = templatesLayout->itemAt(i)->widget();
        if (widget)
        {
            CTemplatePushButton* pushButton = dynamic_cast<CTemplatePushButton*>(widget);
            if(pushButton)
            {
                pushButton->Unfocus();
            }
        }
    }
}

QWidget* CAddSceneWizard::CreateTemplatesWidget(const QString& templatesSubfolder)
{
    QVBoxLayout* vTemplatesLayout = new QVBoxLayout();
    for(CTemplate* currentTemplate : CTemplateManager::Instance()->GetTemplates(templatesSubfolder))
    {
        qDebug()<<"Création d'un nouveau push button";
        CTemplatePushButton* temp = new CTemplatePushButton(currentTemplate);
        //temp->setFocusPolicy(Qt::NoFocus);
        temp->setDefault(false);
        temp->setAutoDefault(false);
        connect(temp, SIGNAL(newTemplateSelected(CTemplatePushButton*)), this, SLOT(setCurrentTemplate(CTemplatePushButton*)));
        vTemplatesLayout->addWidget(temp);
    }
    vTemplatesLayout->setContentsMargins(5, 5, 5, 5);
    QWidget* returnWidget = new QWidget();
    returnWidget->setLayout(vTemplatesLayout);
    returnWidget->setMinimumWidth(260);
    returnWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    return returnWidget;
}

bool CAddSceneWizard::FindExistingID(const std::string& a_rSceneID, int a_iPlayerID)
{
    std::vector<std::string> rSceneID;
    if (a_iPlayerID == 0)
    {
        rSceneID = m_rSceneIDP1;
    }
    else
    {
        rSceneID = m_rSceneIDP2;
    }
    std::vector<std::string>::iterator itSceneID;
    for (itSceneID = rSceneID.begin();
         itSceneID != rSceneID.end();
         ++itSceneID)
    {
        if (*itSceneID == a_rSceneID)
        {
            return true;
        }
    }
    return false;
}
