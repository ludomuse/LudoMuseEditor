#include "CImportProjectWizard.h"
#include "ui_CImportProjectWizard.h"

#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QDirIterator>
#include <QPropertyAnimation>

#include <CProjectManager.h>
#include <CArchiver.h>

#include <UtilsQt.h>

CImportProjectWizard::CImportProjectWizard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CImportProjectWizard)
{
    ui->setupUi(this);
    connect(ui->acceptButton, SIGNAL(clicked(bool)), this, SLOT(clickOnValidate()));
    connect(ui->pathExplorerButton, SIGNAL(clicked(bool)), this, SLOT(clickOnPathExplorer()));
    connect(ui->archiveExplorerButton, SIGNAL(clicked(bool)), this, SLOT(clickOnArchiveExplorer()));
    this->AnimatedOpening();
}

CImportProjectWizard::~CImportProjectWizard()
{
    delete ui;
}

void CImportProjectWizard::ClearError()
{
    ui->errorLabel->setText("");
}

void CImportProjectWizard::AnimatedOpening()
{
    this->setMaximumWidth(0);
    this->setVisible(true);
    QPropertyAnimation *animation = new QPropertyAnimation(this, "maximumWidth");
    animation->setDuration(350);
    animation->setStartValue(0);
    animation->setEndValue(800);

    animation->start();
}

bool CImportProjectWizard::CheckMandatoryFields()
{
    QString text = "";
    int numberMissingFields = 0;
    if(ui->pathLineEdit->text().isEmpty())
    {
        text += "   Le chemin du projet \r\n";
        numberMissingFields++;
    }
    if(ui->archivePathLineEdit->text().isEmpty())
    {
        text += "   Le chemin de l'archive \r\n";
        numberMissingFields++;
    }
    if(numberMissingFields > 0)
    {
        if(numberMissingFields > 1)
        {
            text = "Veuillez remplir les champs suivants : \r\n" + text;
        }
        else
        {
            text = "Veuillez remplir le champ suivant : \r\n" + text;
        }
        ui->errorLabel->setText(text);
        return false;
    }
    else
    {
        return true;
    }
}



void CImportProjectWizard::clickOnValidate()
{
    // Test mandatory fields
    if(!this->CheckMandatoryFields())
    {
        return;
    }

    // Create all directories
    QString archivePath = ui->archivePathLineEdit->text();
    QString projectPath = ui->pathLineEdit->text();

    CArchiver archiver(this);
    archiver.ExtractArchive(archivePath, projectPath);


    //emit importProjectFile
}

void CImportProjectWizard::clickOnPathExplorer()
{
    QString projectPath = QFileDialog::getExistingDirectory(this, "Sélectionner le chemin vers lequel importer le scénario", QDir::homePath(), QFileDialog::ShowDirsOnly);
    ui->pathLineEdit->setText(projectPath);
}

void CImportProjectWizard::clickOnArchiveExplorer()
{
    QString archiveFilePath = QFileDialog::getOpenFileName(this, "Sélectionnez l'archive LudoMuse", QDir::homePath(), "*.lm");
    ui->archivePathLineEdit->setText(archiveFilePath);
}



void CImportProjectWizard::folderSelected(const QString& a_sPath)
{
    ui->pathLineEdit->setText(a_sPath);
}
