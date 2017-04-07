#include "CImportProjectWizard.h"
#include "ui_CImportProjectWizard.h"

#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QDirIterator>
#include <QPropertyAnimation>

#include <CProjectManager.h>

#include <UtilsQt.h>

CImportProjectWizard::CImportProjectWizard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CImportProjectWizard)
{
    ui->setupUi(this);
    connect(ui->acceptButton, SIGNAL(clicked(bool)), this, SLOT(clickOnValidate()));
    connect(ui->pathExplorerButton, SIGNAL(clicked(bool)), this, SLOT(clickOnPathExplorer()));
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
    if(ui->projectNameLineEdit->text().isEmpty())
    {
        text += "   Le nom du projet \r\n";
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
    QDir dir(ui->pathLineEdit->text());
    QString projectName = ui->projectNameLineEdit->text();
    QString projectPath = ui->pathLineEdit->text() + "/" + projectName;
    dir.mkdir(projectName);
    dir.cd(projectName);
    dir.mkdir("cache");
    dir.mkdir("ui");
    dir.mkdir("fonts");


    QFile::copy(CProjectManager::Instance()->QGetInstallPath() + "/default/default.json",     projectPath + "/" + projectName + ".json");
    // Copy file from /cache to /cache


    CopyFolder(CProjectManager::Instance()->QGetInstallPath() + "/default/cache", projectPath + "/cache/");
    CopyFolder(CProjectManager::Instance()->QGetInstallPath() + "/default/ui", projectPath + "/ui/");
    CopyFolder(CProjectManager::Instance()->QGetInstallPath() + "/default/fonts", projectPath + "/fonts/");
    emit createNewProject(projectPath + "/" + projectName + ".json");
}

void CImportProjectWizard::clickOnPathExplorer()
{
    QFileDialog* fileDialog = new QFileDialog();
    fileDialog->setDirectory(QDir::home());
    fileDialog->setFileMode(QFileDialog::Directory);
    fileDialog->setOption(QFileDialog::DontUseNativeDialog);
    fileDialog->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    connect(fileDialog, SIGNAL(fileSelected(QString)), this, SLOT(folderSelected(QString)));

    fileDialog->show();
}

void CImportProjectWizard::folderSelected(const QString& a_sPath)
{
    ui->pathLineEdit->setText(a_sPath);
}
