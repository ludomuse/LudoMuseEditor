#include "CLoadProjectWizard.h"
#include "ui_CLoadProjectWizard.h"

#include <vector>
#include <string>

#include <QFileDialog>
#include <QPropertyAnimation>

#include <CProjectPushButton.h>
#include <CProjectManager.h>

CLoadProjectWizard::CLoadProjectWizard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CLoadProjectWizard)
{
    ui->setupUi(this);
    this->CreatePreviousProjectButtons();
    connect(ui->acceptButton, SIGNAL(clicked(bool)), this, SLOT(clickOnValidate()));
    connect(ui->pathExplorerButton, SIGNAL(clicked(bool)), this, SLOT(clickOnPathExplorer()));
    this->AnimatedOpening();
}

CLoadProjectWizard::~CLoadProjectWizard()
{
    delete ui;
}

void CLoadProjectWizard::ClearError()
{
    ui->errorLabel->setText("");
}

void CLoadProjectWizard::AnimatedOpening()
{
    this->setMaximumWidth(0);
    this->setVisible(true);
    QPropertyAnimation *animation = new QPropertyAnimation(this, "maximumWidth");
    animation->setDuration(350);
    animation->setStartValue(0);
    animation->setEndValue(800);

    animation->start();
}

void CLoadProjectWizard::CreatePreviousProjectButtons()
{
    for(std::string currentPath : CProjectManager::Instance()->GetPreviousProjectPaths())
    {
        CProjectPushButton* temp = new CProjectPushButton(QString(currentPath.c_str()));
        ui->recentProjectList->layout()->addWidget(temp);
        connect(temp, SIGNAL(onClick(QString)), this, SLOT(clickOnPrevProject(QString)));
    }
}

bool CLoadProjectWizard::CheckMandatoryFields()
{
    if(ui->pathLineEdit->text().isEmpty())
    {
        ui->errorLabel->setText("Veuillez séléctionner un fichier ou cliquer sur un scénario récent");
        return false;
    }
    return true;
}

void CLoadProjectWizard::clickOnValidate()
{
    if(!this->CheckMandatoryFields())
    {
        return;
    }
    emit loadProjectFile(ui->pathLineEdit->text());
}

void CLoadProjectWizard::clickOnPathExplorer()
{
    QFileDialog* fileDialog = new QFileDialog();
    fileDialog->setDirectory(QDir::home());
    fileDialog->setNameFilter("*.json");
    connect(fileDialog, SIGNAL(fileSelected(QString)), this, SLOT(folderSelected(QString)));

    fileDialog->show();
}

void CLoadProjectWizard::folderSelected(const QString& a_sPath)
{
    ui->pathLineEdit->setText(a_sPath);
}

void CLoadProjectWizard::clickOnPrevProject(const QString &a_sProjectFile)
{
    emit loadProjectFile(a_sProjectFile);
}

