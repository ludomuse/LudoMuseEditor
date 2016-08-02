#include "CLoadProjectWizard.h"
#include "ui_CLoadProjectWizard.h"

#include <QFileDialog>

CLoadProjectWizard::CLoadProjectWizard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CLoadProjectWizard)
{
    ui->setupUi(this);
    connect(ui->acceptButton, SIGNAL(clicked(bool)), this, SLOT(clickOnValidate()));
    connect(ui->pathExplorerButton, SIGNAL(clicked(bool)), this, SLOT(clickOnPathExplorer()));
}

CLoadProjectWizard::~CLoadProjectWizard()
{
    delete ui;
}

void CLoadProjectWizard::clearError()
{
    ui->errorLabel->setText("");
}

bool CLoadProjectWizard::checkMandatoryFields()
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
    if(!this->checkMandatoryFields())
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
