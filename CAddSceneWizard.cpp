#include "CAddSceneWizard.h"

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
    m_iTemplateNumber(1),
    m_rSceneIDP1(a_rSceneIDP1),
    m_rSceneIDP2(a_rSceneIDP2),
    m_iActivePlayer(a_iActivePlayer),
    m_pComboBoxID(Q_NULLPTR),
    m_pComboBoxID2(Q_NULLPTR)
{
    QHBoxLayout* hWizardLayout = new QHBoxLayout();

    QWidget* templateWidget = new QWidget();
    QScrollArea* templateScrollArea = new QScrollArea();
    templateScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    templateScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    templateScrollArea->setStyleSheet("QLabel{ border : 1px solid white}");
    QVBoxLayout* vTemplateLayout = new QVBoxLayout();
    QPushButton* template1 = new QPushButton("template1");
    template1->setMinimumHeight(120);
    QPushButton* template2 = new QPushButton("template2");
    template2->setMinimumHeight(120);
    QPushButton* template3 = new QPushButton("template3");
    template3->setMinimumHeight(120);
    QPushButton* template4 = new QPushButton("template4");
    template4->setMinimumHeight(120);
    QPushButton* template5 = new QPushButton("template5");
    template5->setMinimumHeight(120);
    QPushButton* template6 = new QPushButton("template6");
    template6->setMinimumHeight(120);
    QVBoxLayout* vTemplateScrollAreaLayout = new QVBoxLayout();
    vTemplateLayout->addWidget(template1);
    vTemplateLayout->addWidget(template2);
    vTemplateLayout->addWidget(template3);
    vTemplateLayout->addWidget(template4);
    vTemplateLayout->addWidget(template5);
    vTemplateLayout->addWidget(template6);
    templateScrollArea->setLayout(vTemplateScrollAreaLayout);
    templateWidget->setLayout(vTemplateLayout);
    templateWidget->setMinimumWidth(300);
    templateScrollArea->setWidget(templateWidget);

    // Create preview title
    m_pPreviewTitle = new QLabel("");
    m_pPreviewTitle->setAlignment(Qt::AlignCenter);

    // Create preview widget
    QWidget* previewAndOptionWidget = new QWidget();
    QVBoxLayout* vPreviewOptionLayout = new QVBoxLayout();
    QWidget* previewWidget = new QWidget();
    previewWidget->setMinimumHeight(250);
    previewWidget->setMinimumWidth(250);
    previewWidget->setStyleSheet("QWidget{ background-color : white;}");

    // Create previous screen comboBox
    QWidget* comboBoxWidget = new QWidget();
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
    comboBoxWidget->setLayout(hComboBoxLayout);


    // Create otpion widget
    QWidget* optionWidget = new QWidget();
    QHBoxLayout* hOptionLayout = new QHBoxLayout();
    QWidget* rightPart = new QWidget();
    QVBoxLayout* vRightPartLayout = new QVBoxLayout();
    QWidget* playerWidget = new QWidget();
    QHBoxLayout* hPlayerLayout = new QHBoxLayout();
    m_pPlayer1CheckBox = new QCheckBox("Joueur 1");
    hPlayerLayout->addWidget(m_pPlayer1CheckBox);
    playerWidget->setLayout(hPlayerLayout);
    QWidget* synchroWidget = new QWidget();
    QHBoxLayout* hSynchroLayout = new QHBoxLayout();
    QCheckBox* synchroCheckBox = new QCheckBox("Synchronisée");
    hSynchroLayout->addWidget(synchroCheckBox);
    synchroWidget->setLayout(hSynchroLayout);
    QWidget* dashWidget = new QWidget();
    QHBoxLayout* hDashLayout = new QHBoxLayout();
    QCheckBox* dashCheckBox = new QCheckBox("Déclenche le dashboard");
    hDashLayout->addWidget(dashCheckBox);
    dashWidget->setLayout(hDashLayout);
    vRightPartLayout->addWidget(playerWidget);
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
    QWidget* backgroundWidget = new QWidget();
    QHBoxLayout* hBackgroundLayout = new QHBoxLayout();
    QLabel* backgroundTitle = new QLabel("Background :");
    QLineEdit* backgroundValue = new QLineEdit();
    backgroundValue->setPlaceholderText("Background image");
    hBackgroundLayout->addWidget(backgroundTitle);
    hBackgroundLayout->addWidget(backgroundValue);
    backgroundWidget->setLayout(hBackgroundLayout);
    QWidget* soundWidget = new QWidget();
    QHBoxLayout* hSoundLayout = new QHBoxLayout();
    QLabel* soundTitle = new QLabel("Fond sonore :");
    QLineEdit* soundValue = new QLineEdit();
    soundValue->setPlaceholderText("Piste Audio");
    hSoundLayout->addWidget(soundTitle);
    hSoundLayout->addWidget(soundValue);
    soundWidget->setLayout(hSoundLayout);
    vLeftPartLayout->addWidget(idWidget);
    vLeftPartLayout->addWidget(backgroundWidget);
    vLeftPartLayout->addWidget(soundWidget);
    leftPart->setLayout(vLeftPartLayout);
    hOptionLayout->addWidget(leftPart);
    hOptionLayout->addWidget(rightPart);
    optionWidget->setLayout(hOptionLayout);

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
    vPreviewOptionLayout->addWidget(previewWidget);
    vPreviewOptionLayout->addWidget(comboBoxWidget);
    vPreviewOptionLayout->addWidget(optionWidget);
    previewAndOptionWidget->setLayout(vPreviewOptionLayout);





    // Fill the second preview widget (exactly same as first one)
    m_pPreviewTitle2 = new QLabel("");
    m_pPreviewTitle2->setAlignment(Qt::AlignCenter);

    // Create preview widget
    QWidget* previewAndOptionWidget2 = new QWidget();
    QVBoxLayout* vPreviewOptionLayout2 = new QVBoxLayout();
    QWidget* previewWidget2 = new QWidget();
    previewWidget2->setMinimumHeight(250);
    previewWidget2->setMinimumWidth(250);
    previewWidget2->setStyleSheet("QWidget{ background-color : white;}");

    // Create previous screen comboBox
    QWidget* comboBoxWidget2 = new QWidget();
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
    comboBoxWidget2->setLayout(hComboBoxLayout2);
    // Create otpion widget
    QWidget* optionWidget2 = new QWidget();
    QHBoxLayout* hOptionLayout2 = new QHBoxLayout();
    QWidget* rightPart2 = new QWidget();
    QVBoxLayout* vRightPartLayout2 = new QVBoxLayout();
    QWidget* playerWidget2 = new QWidget();
    QHBoxLayout* hPlayerLayout2 = new QHBoxLayout();
    m_pPlayer2CheckBox = new QCheckBox("Joueur 2");
    hPlayerLayout2->addWidget(m_pPlayer2CheckBox);
    // initialise check box
    if(a_iActivePlayer == 2)
    {
        m_pPlayer2CheckBox->setChecked(true);
        m_pPlayer1CheckBox->setChecked(false);
        m_pComboBoxID->setEnabled(false);
    }
    else if(a_iActivePlayer == 1)
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
    QCheckBox* synchroCheckBox2 = new QCheckBox("Synchronisée");
    hSynchroLayout2->addWidget(synchroCheckBox2);
    synchroWidget2->setLayout(hSynchroLayout2);
    QWidget* dashWidget2 = new QWidget();
    QHBoxLayout* hDashLayout2 = new QHBoxLayout();
    QCheckBox* dashCheckBox2 = new QCheckBox("Déclenche le dashboard");
    hDashLayout2->addWidget(dashCheckBox2);
    dashWidget2->setLayout(hDashLayout2);
    vRightPartLayout2->addWidget(playerWidget2);
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
    QWidget* backgroundWidget2 = new QWidget();
    QHBoxLayout* hBackgroundLayout2 = new QHBoxLayout();
    QLabel* backgroundTitle2 = new QLabel("Background :");
    QLineEdit* backgroundValue2 = new QLineEdit();
    backgroundValue2->setPlaceholderText("Background image");
    hBackgroundLayout2->addWidget(backgroundTitle2);
    hBackgroundLayout2->addWidget(backgroundValue2);
    backgroundWidget2->setLayout(hBackgroundLayout2);
    QWidget* soundWidget2 = new QWidget();
    QHBoxLayout* hSoundLayout2 = new QHBoxLayout();
    QLabel* soundTitle2 = new QLabel("Fond sonore :");
    QLineEdit* soundValue2 = new QLineEdit();
    soundValue2->setPlaceholderText("Piste Audio");
    hSoundLayout2->addWidget(soundTitle2);
    hSoundLayout2->addWidget(soundValue2);
    soundWidget2->setLayout(hSoundLayout2);
    vLeftPartLayout2->addWidget(idWidget2);
    vLeftPartLayout2->addWidget(backgroundWidget2);
    vLeftPartLayout2->addWidget(soundWidget2);
    leftPart2->setLayout(vLeftPartLayout2);
    hOptionLayout2->addWidget(leftPart2);
    hOptionLayout2->addWidget(rightPart2);
    optionWidget2->setLayout(hOptionLayout2);
    vPreviewOptionLayout2->addWidget(m_pPreviewTitle2);
    vPreviewOptionLayout2->addWidget(previewWidget2);
    vPreviewOptionLayout2->addWidget(comboBoxWidget2);
    vPreviewOptionLayout2->addWidget(optionWidget2);
    previewAndOptionWidget2->setLayout(vPreviewOptionLayout2);


    QHBoxLayout* hPreviewsLayout = new QHBoxLayout();
    hPreviewsLayout->addWidget(previewAndOptionWidget);
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


    // Connect pushButton
    connect(backButton, SIGNAL(clicked(bool)), this, SLOT(clickOnBack(bool)));
    connect(validateButton, SIGNAL(clicked(bool)), this, SLOT(clickOnValidate(bool)));
    connect(template1, SIGNAL(clicked(bool)), this, SLOT(setTemplate1()));
    connect(template2, SIGNAL(clicked(bool)), this, SLOT(setTemplate2()));
    connect(template3, SIGNAL(clicked(bool)), this, SLOT(setTemplate3()));
    connect(m_pPlayer1CheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeActivePlayer()));
    connect(m_pPlayer2CheckBox, SIGNAL(stateChanged(int)), this, SLOT(changeActivePlayer()));
}

// SLOTS

void CAddSceneWizard::clickOnBack(bool)
{
    this->close();
}

void CAddSceneWizard::clickOnValidate(bool)
{
    // Check  for
    if(!m_pPlayer1CheckBox->isChecked() && !m_pPlayer2CheckBox->isChecked())
    {
        this->openModalDialog("Selectionnez au moins un joueur");
        return;
    }

    // Establish player id return
    int idReturn;
    if(m_pPlayer1CheckBox->isChecked())
    {
        if(m_pPlayer2CheckBox->isChecked())
        {
            idReturn = 0; // P1 & P2
        }
        else
        {
            idReturn = 1; // Only P1
        }
    }
    else
    {
        idReturn = 2; // Only P2
    }

    QString previousID = m_pComboBoxID->currentText();
    QString previousID2 = m_pComboBoxID2->currentText();
    if(idReturn == 0)
    {
        if(m_pNewID->text().isEmpty() || m_pNewID2->text().isEmpty())
        {
            this->openModalDialog("Rentrez un identifiant de scene pour les deux joueurs");
            return;
        }
        emit addTwoScene(previousID, m_pNewID->text(), previousID2, m_pNewID2->text(), m_iTemplateNumber);
    }
    else if(idReturn == 2) // Add scene on player 2 timeline
    {
        if(m_pNewID2->text().isEmpty())
        {
            this->openModalDialog("Rentrez un identifiant de scene pour le joueur 2");
            return;
        }
        emit addOneScene(previousID2, m_pNewID2->text(), idReturn, m_iTemplateNumber);
    }
    else // Add scene in player 1 timeline
    {
        if(m_pNewID->text().isEmpty())
        {
            this->openModalDialog("Rentrez un identifiant de scene pour le joueur 1");
            return;
        }
        emit addOneScene(previousID, m_pNewID->text(), idReturn, m_iTemplateNumber);
    }

    this->close();

}

void CAddSceneWizard::setTemplate1()
{
    m_pPreviewTitle->setText("Template 1");
    m_pPreviewTitle2->setText("Template 1");
    m_iTemplateNumber = 1;
}

void CAddSceneWizard::setTemplate2()
{
    m_pPreviewTitle->setText("Template 2");
    m_pPreviewTitle2->setText("Template 2");
    m_iTemplateNumber = 2;
}

void CAddSceneWizard::setTemplate3()
{
    m_pPreviewTitle->setText("Template 3");
    m_pPreviewTitle2->setText("Template 3");
    m_iTemplateNumber = 3;
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
           this->m_pComboBoxID->setEnabled(true);
           this->m_pComboBoxID2->setEnabled(true);
       }
       else
       {
           // Only P1 scene id available
           this->m_pComboBoxID->setEnabled(true);
           this->m_pComboBoxID2->setEnabled(false);       }
    }
    else if(m_pPlayer2CheckBox->isChecked())
    {
        // Only P2 scene available
        this->m_pComboBoxID->setEnabled(false);
        this->m_pComboBoxID2->setEnabled(true);
    }
    else
    {
        // Both uncheck
        this->m_pComboBoxID->setEnabled(false);
        this->m_pComboBoxID2->setEnabled(false);
    }
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

void CAddSceneWizard::openModalDialog(const QString &a_sText)
{
    QMessageBox* pEmptyId = new QMessageBox();
    pEmptyId->setText(a_sText);
    pEmptyId->addButton("Ok", QMessageBox::AcceptRole);
    pEmptyId->show();
    pEmptyId->setModal(true);
}
