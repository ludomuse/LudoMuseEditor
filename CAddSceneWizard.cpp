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
    m_pTemplatesWidget = new QWidget();
    QScrollArea* templateScrollArea = new QScrollArea();
    templateScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    templateScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    templateScrollArea->setStyleSheet("QLabel{ border : 1px solid white}");
    templateScrollArea->setMinimumWidth(300);
    QVBoxLayout* vTemplateScrollAreaLayout = new QVBoxLayout();
    templateScrollArea->setLayout(vTemplateScrollAreaLayout);
    QVBoxLayout* vTemplateLayout = new QVBoxLayout();
    m_pTemplatesWidget->setLayout(vTemplateLayout);
    m_pTemplatesWidget->setMinimumWidth(350);

    m_pTemplatesWidget = this->CreateTemplatesWidget();
    templateScrollArea->setWidget(m_pTemplatesWidget);

    // Create preview title
    m_pPreviewTitle = new QLabel("");
    m_pPreviewTitle->setAlignment(Qt::AlignCenter);

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
        m_pComboBoxID->addItem("fin");
        m_pComboBoxID->setCurrentIndex(0);
        m_pComboBoxID->setEnabled(false);
    }
    m_pComboBoxID->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    hComboBoxLayout->addWidget(idComboBoxTitle);
    hComboBoxLayout->addWidget(m_pComboBoxID);
    m_pComboBoxWidget->setLayout(hComboBoxLayout);


    // Create otpion widget
    m_pOptionWidget = new QWidget();
    QHBoxLayout* hOptionLayout = new QHBoxLayout();
    QWidget* rightPart = new QWidget();
    QVBoxLayout* vRightPartLayout = new QVBoxLayout();
    QWidget* playerWidget = new QWidget();
    QHBoxLayout* hPlayerLayout = new QHBoxLayout();
    m_pPlayer1CheckBox = new QCheckBox("Joueur 1");
    hPlayerLayout->addWidget(m_pPlayer1CheckBox);
    hPlayerLayout->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    playerWidget->setLayout(hPlayerLayout);
    QWidget* synchroWidget = new QWidget();
    QHBoxLayout* hSynchroLayout = new QHBoxLayout();
    m_pSynchroCheckBox = new QCheckBox("Synchronisée");
    hSynchroLayout->addWidget(m_pSynchroCheckBox);
    synchroWidget->setLayout(hSynchroLayout);
    QWidget* dashWidget = new QWidget();
    QHBoxLayout* hDashLayout = new QHBoxLayout();
    m_pDashCheckBox = new QCheckBox("Déclenche le dashboard");
    hDashLayout->addWidget(m_pDashCheckBox);
    dashWidget->setLayout(hDashLayout);
//    vRightPartLayout->addWidget(playerWidget);
    vRightPartLayout->addWidget(synchroWidget);
    vRightPartLayout->addWidget(dashWidget);
    rightPart->setLayout(vRightPartLayout);
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
    QWidget* soundWidget = new QWidget();
    QHBoxLayout* hSoundLayout = new QHBoxLayout();
    QLabel* soundTitle = new QLabel("Fond sonore :");
    m_pSoundPath = new QLineEdit();
    m_pSoundPath->setPlaceholderText("Piste Audio");
    hSoundLayout->addWidget(soundTitle);
    hSoundLayout->addWidget(m_pSoundPath);
    soundWidget->setLayout(hSoundLayout);
    vLeftPartLayout->addWidget(idWidget);
    vLeftPartLayout->addWidget(soundWidget);
    leftPart->setLayout(vLeftPartLayout);
    hOptionLayout->addWidget(leftPart);
    hOptionLayout->addWidget(rightPart);
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
    vPreviewOptionLayout->addWidget(m_pPreviewTitle);
    vPreviewOptionLayout->addWidget(playerWidget);
    vPreviewOptionLayout->addWidget(m_pPreviewWidget);
    vPreviewOptionLayout->addWidget(m_pComboBoxWidget);
    vPreviewOptionLayout->addWidget(m_pOptionWidget);
    previewAndOptionWidget->setLayout(vPreviewOptionLayout);





    // Fill the second preview widget (exactly same as first one)
    m_pPreviewTitle2 = new QLabel("");
    m_pPreviewTitle2->setAlignment(Qt::AlignCenter);

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
    QWidget* rightPart2 = new QWidget();
    QVBoxLayout* vRightPartLayout2 = new QVBoxLayout();
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
        m_pComboBoxID->setEnabled(false);
    }
    else if(a_iActivePlayer == 0)
    {
        m_pPlayer1CheckBox->setChecked(true);
        m_pPlayer2CheckBox->setChecked(false);
        m_pComboBoxID2->setEnabled(false);
    }
    else
    {
        m_pPlayer1CheckBox->setChecked(true);
        m_pPlayer2CheckBox->setChecked(true);
    }
    playerWidget2->setLayout(hPlayerLayout2);
    QWidget* synchroWidget2 = new QWidget();
    QHBoxLayout* hSynchroLayout2 = new QHBoxLayout();
    m_pSynchroCheckBox2 = new QCheckBox("Synchronisée");
    hSynchroLayout2->addWidget(m_pSynchroCheckBox2);
    synchroWidget2->setLayout(hSynchroLayout2);
    QWidget* dashWidget2 = new QWidget();
    QHBoxLayout* hDashLayout2 = new QHBoxLayout();
    m_pDashCheckBox2 = new QCheckBox("Déclenche le dashboard");
    hDashLayout2->addWidget(m_pDashCheckBox2);
    dashWidget2->setLayout(hDashLayout2);
//    vRightPartLayout2->addWidget(playerWidget2);
    vRightPartLayout2->addWidget(synchroWidget2);
    vRightPartLayout2->addWidget(dashWidget2);
    rightPart2->setLayout(vRightPartLayout2);
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
    QWidget* soundWidget2 = new QWidget();
    QHBoxLayout* hSoundLayout2 = new QHBoxLayout();
    QLabel* soundTitle2 = new QLabel("Fond sonore :");
    m_pSoundPath2 = new QLineEdit();
    m_pSoundPath2->setPlaceholderText("Piste Audio");
    hSoundLayout2->addWidget(soundTitle2);
    hSoundLayout2->addWidget(m_pSoundPath2);
    soundWidget2->setLayout(hSoundLayout2);
    vLeftPartLayout2->addWidget(idWidget2);
    vLeftPartLayout2->addWidget(soundWidget2);
    leftPart2->setLayout(vLeftPartLayout2);
    hOptionLayout2->addWidget(leftPart2);
    hOptionLayout2->addWidget(rightPart2);
    m_pOptionWidget2->setLayout(hOptionLayout2);
    vPreviewOptionLayout2->addWidget(m_pPreviewTitle2);
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
    vWizardLayout->addWidget(previewsWidget);
    vWizardLayout->addWidget(buttonsWidget);
    QWidget* mainWidget = new QWidget();
    mainWidget->setLayout(vWizardLayout);

    hWizardLayout->addWidget(templateScrollArea);
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

    // Establish player id return
    int idReturn;
    if(m_pPlayer1CheckBox->isChecked())
    {
        if(m_pPlayer2CheckBox->isChecked())
        {
            idReturn = 3; // P1 & P2
        }
        else
        {
            idReturn = 0; // Only P1
        }
    }
    else
    {
        idReturn = 1; // Only P2
    }

    if(m_pNewID->text() == m_pNewID2->text() && idReturn == 3)
    {
        this->OpenModalDialog("Les deux identifiants doivent être différents");
        return;
    }

    QString previousID = m_pComboBoxID->currentText();
    QString previousID2 = m_pComboBoxID2->currentText();

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
    if(idReturn == 3)
    {
        if(m_pNewID->text().isEmpty() || m_pNewID2->text().isEmpty())
        {
            this->OpenModalDialog("Rentrez un identifiant de scene pour les deux joueurs");
            return;
        }
        emit addTwoScene(previousID, m_pNewID->text(), previousID2, m_pNewID2->text(), m_pCurrentTemplateButton->GetTemplate());
    }
    else if(idReturn == 1) // Add scene on player 2 timeline
    {
        if(m_pNewID2->text().isEmpty())
        {
            this->OpenModalDialog("Rentrez un identifiant de scene pour le joueur 2");
            return;
        }
        emit addOneScene(previousID2, m_pNewID2->text(), idReturn, m_pCurrentTemplateButton->GetTemplate());
    }
    else // Add scene in player 1 timeline
    {
        if(m_pNewID->text().isEmpty())
        {
            this->OpenModalDialog("Rentrez un identifiant de scene pour le joueur 1");
            return;
        }
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
       }
       else
       {
           // Only P1 scene id available
           this->SetEnabledPlayerField(0, true);
           this->SetEnabledPlayerField(1, false);       }
    }
    else if(m_pPlayer2CheckBox->isChecked())
    {
        // Only P2 scene available
        this->SetEnabledPlayerField(0, false);
        this->SetEnabledPlayerField(1, true);
    }
    else
    {
        // Both uncheck
        this->SetEnabledPlayerField(0, false);
        this->SetEnabledPlayerField(1, false);
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
        this->m_pCurrentTemplateButton->Unfocus();
    }
    this->m_pCurrentTemplateButton = a_pTemplatePushButton;
    CTemplate* pTemplate = a_pTemplatePushButton->GetTemplate();
    this->m_pPreviewTitle->setText(pTemplate->GetName());
    this->m_pPreviewTitle2->setText(pTemplate->GetName());
    if(pTemplate->IsGame())
    {
        m_pSwapButton->setEnabled(true);
        m_pSynchroCheckBox->setChecked(true);
        m_pSynchroCheckBox2->setChecked(true);
        m_pPlayer1CheckBox->setChecked(true);
        m_pPlayer2CheckBox->setChecked(true);
        this->SetEnabledPlayerField(0, true);
        this->SetEnabledPlayerField(1, true);
        m_pSynchroCheckBox->setEnabled(false);
        m_pSynchroCheckBox2->setEnabled(false);
        m_pPlayer1CheckBox->setEnabled(false);
        m_pPlayer2CheckBox->setEnabled(false);
    }
    else
    {
        m_pSwapButton->setEnabled(false);
        m_pSynchroCheckBox->setChecked(false);
        m_pSynchroCheckBox2->setChecked(false);
        m_pSynchroCheckBox->setEnabled(true);
        m_pSynchroCheckBox2->setEnabled(true);
        if(m_iActivePlayer == 0) // P1
        {
            m_pPlayer1CheckBox->setChecked(true);
            m_pPlayer2CheckBox->setChecked(false);
            this->SetEnabledPlayerField(0, true);
            this->SetEnabledPlayerField(1, false);
            m_pSynchroCheckBox->setEnabled(true);
            m_pSynchroCheckBox2->setEnabled(false);
        }
        else // P2
        {
            m_pPlayer1CheckBox->setChecked(false);
            m_pPlayer2CheckBox->setChecked(true);
            this->SetEnabledPlayerField(0, false);
            this->SetEnabledPlayerField(1, true);
            m_pSynchroCheckBox->setEnabled(false);
            m_pSynchroCheckBox2->setEnabled(true);
        }
        m_pPlayer1CheckBox->setEnabled(true);
        m_pPlayer2CheckBox->setEnabled(true);
    }
    m_bScreensSwaped = false; // reset swaping state
    this->UpdatePreview();
}

void CAddSceneWizard::FillComboBox(int a_iPlayerID, const QString& a_rActiveScene)
{
    int index = 0;
    bool activeSceneFound = false;
    if(a_iPlayerID == 1)
    {
        if(m_pComboBoxID != Q_NULLPTR)
        {
            m_pComboBoxID->clear();
        }
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
        this->m_pPreviewTitle2->setEnabled(a_bEnabled);
        this->m_pComboBoxID2->setEnabled(a_bEnabled);
        this->m_pNewID2->setEnabled(a_bEnabled);
        this->m_pSoundPath2->setEnabled(a_bEnabled);
        this->m_pDashCheckBox2->setEnabled(a_bEnabled);
        this->m_pSynchroCheckBox2->setEnabled(a_bEnabled);
        this->m_pPreviewWidget2->setEnabled(a_bEnabled);
        this->m_pComboBoxWidget2->setEnabled(a_bEnabled);
        this->m_pOptionWidget2->setEnabled(a_bEnabled);
    }
    else
    {
        this->m_pPreviewTitle->setEnabled(a_bEnabled);
        this->m_pComboBoxID->setEnabled(a_bEnabled);
        this->m_pNewID->setEnabled(a_bEnabled);
        this->m_pSoundPath->setEnabled(a_bEnabled);
        this->m_pDashCheckBox->setEnabled(a_bEnabled);
        this->m_pSynchroCheckBox->setEnabled(a_bEnabled);
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
    QLayout* templatesLayout = m_pTemplatesWidget->layout();
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

QWidget* CAddSceneWizard::CreateTemplatesWidget()
{
    QVBoxLayout* vTemplatesLayout = new QVBoxLayout();
    for(CTemplate* currentTemplate : CTemplateManager::Instance()->GetTemplates())
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
    returnWidget->setMinimumWidth(420);
    return returnWidget;
}
