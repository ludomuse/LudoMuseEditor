#include "CNewProjectWizard.h"
#include "ui_CNewProjectWizard.h"

#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QDir>
#include <QDirIterator>

#include <CProjectManager.h>

#include <UtilsQt.h>

CNewProjectWizard::CNewProjectWizard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CNewProjectWizard)
{
    ui->setupUi(this);
    connect(ui->acceptButton, SIGNAL(clicked(bool)), this, SLOT(clickOnValidate()));
    connect(ui->pathExplorerButton, SIGNAL(clicked(bool)), this, SLOT(clickOnPathExplorer()));
}

CNewProjectWizard::~CNewProjectWizard()
{
    delete ui;
}

void CNewProjectWizard::ClearError()
{
    ui->errorLabel->setText("");
}

bool CNewProjectWizard::CheckMandatoryFields()
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



void CNewProjectWizard::clickOnValidate()
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
    dir.mkdir("res_templ");
    dir.cd("res_templ");
    dir.mkdir("cache");
    dir.mkdir("fonts");
    dir.mkdir("ui");
    CopyFolder(CProjectManager::Instance()->QGetInstallPath() + "/debug/templates/res_templ/cache",
               projectPath + "/res_templ/cache");
    CopyFolder(CProjectManager::Instance()->QGetInstallPath() + "/debug/templates/res_templ/fonts",
               projectPath + "/res_templ/fonts");
    CopyFolder(CProjectManager::Instance()->QGetInstallPath() + "/debug/templates/res_templ/ui",
               projectPath + "/res_templ/ui");

    QFile::copy(CProjectManager::Instance()->QGetInstallPath() + "\\debug\\default\\default.json",     projectPath + "/" + projectName + ".json");
    // Copy file from /cache to /cache


    CopyFolder(CProjectManager::Instance()->QGetInstallPath() + "/debug/default/cache", projectPath + "/cache/");
    CopyFolder(CProjectManager::Instance()->QGetInstallPath() + "/debug/default/ui", projectPath + "/ui/");
    CopyFolder(CProjectManager::Instance()->QGetInstallPath() + "/debug/default/fonts", projectPath + "/fonts/");
    emit createNewProject(projectPath + "/" + projectName + ".json");
}

void CNewProjectWizard::clickOnPathExplorer()
{
    QFileDialog* fileDialog = new QFileDialog();
    fileDialog->setDirectory(QDir::home());
    fileDialog->setFileMode(QFileDialog::Directory);
    fileDialog->setOption(QFileDialog::DontUseNativeDialog);
    fileDialog->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    connect(fileDialog, SIGNAL(fileSelected(QString)), this, SLOT(folderSelected(QString)));

    fileDialog->show();
}

void CNewProjectWizard::folderSelected(const QString& a_sPath)
{
    ui->pathLineEdit->setText(a_sPath);
}
