#include "CLoaderWidget.h"
#include "ui_CLoaderWidget.h"
#include "CNewProjectWizard.h"

#include <UtilsQt.h>
#include <CProjectManager.h>

#include <QDir>
#include <QFileDialog>


CLoaderWidget::CLoaderWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CLoaderWidget),
    m_bNewProjectWizard(false),
    m_bLoadExistingProject(false)
{
    ui->setupUi(this);
    connect(ui->exitButton, SIGNAL(clicked(bool)), this, SLOT(clickCloseEditor()));
    connect(ui->newButton, SIGNAL(clicked(bool)), this, SLOT(clickNewProject()));
    connect(ui->loadButton, SIGNAL(clicked(bool)), this, SLOT(clickLoadProject()));
}

CLoaderWidget::~CLoaderWidget()
{
    delete ui;
}

void CLoaderWidget::SetNewSelect(bool a_bActive)
{
    if(a_bActive)
    {
        ui->newProject->setStyleSheet("#newProject{border-left : 2px dotted rgb(255, 170, 0); border-bottom : 2px dotted rgb(255, 170, 0); border-top : 2px dotted rgb(255, 170, 0); background-color : rgb(65,65,65);}");

    }
    else
    {
        ui->newProject->setStyleSheet("#newProject{ border : 1px dashed white;}");
    }
}

void CLoaderWidget::SetLoadSelect(bool a_bActive)
{
    if(a_bActive)
    {
        ui->loadProject->setStyleSheet("#loadProject{border-left : 2px dotted rgb(255, 170, 0); border-bottom : 2px dotted rgb(255, 170, 0); border-top : 2px dotted rgb(255, 170, 0); background-color : rgb(65,65,65);}");

    }
    else
    {
        ui->loadProject->setStyleSheet("#loadProject{ border : 1px dashed white;}");
    }
}

void CLoaderWidget::ClearWizardContainer()
{
    QLayout* wizardContainerLayout = ui->wizardContainer->layout();
    QLayoutItem *child;
    while ((child = wizardContainerLayout->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }
}

void CLoaderWidget::clickCloseEditor()
{
    emit closeEditor();
}

void CLoaderWidget::clickNewProject()
{
    m_bLoadExistingProject = false;
    this->ClearWizardContainer();
    if(!m_bNewProjectWizard)
    {
        this->SetNewSelect(true);
        this->SetLoadSelect(false);
        ui->wizardContainer->layout()->addWidget(new CNewProjectWizard());
    }
    else
    {
        this->SetNewSelect(false);
    }
    m_bNewProjectWizard = !m_bNewProjectWizard;
}

void CLoaderWidget::clickLoadProject()
{
    m_bNewProjectWizard = false;
    this->ClearWizardContainer();
    if(!m_bLoadExistingProject)
    {
        this->SetLoadSelect(true);
        this->SetNewSelect(false);
    }
    else
    {
        this->SetLoadSelect(false);
    }
    m_bLoadExistingProject = !m_bLoadExistingProject;

    QFileDialog* fileDialog = new QFileDialog();
    fileDialog->setDirectory(QDir::home());
    fileDialog->setNameFilter("*.json");
    connect(fileDialog, SIGNAL(fileSelected(QString)), this, SLOT(projectSelected(QString)));

    fileDialog->show();
}

void CLoaderWidget::projectSelected(QString a_sProjectFile)
{
    // Update CprojetManager
    CProjectManager::Instance()->SetProjectPath(a_sProjectFile);

    // Search for template folder in projectPath
    QFileInfo projectFile(a_sProjectFile);
    QString projectPath = projectFile.absolutePath();
    QDir projectDir(projectPath);
    if(!projectDir.cd("res_templ"))
    {
        projectDir.mkdir("res_templ");
    }
    projectDir.cd("res_templ");
    if(projectDir.cd("cache"))
    {
        projectDir.cdUp();
    }
    else
    {
        projectDir.mkdir("cache");
    }
    if(projectDir.cd("fonts"))
    {
        projectDir.cdUp();
    }
    else
    {
        projectDir.mkdir("fonts");
    }
    if(projectDir.cd("ui"))
    {
        projectDir.cdUp();
    }
    else
    {
        projectDir.mkdir("ui");
    }
    CopyFolder(CProjectManager::Instance()->QGetInstallPath() + "/debug/templates/res_templ/cache",
               projectPath + "/res_templ/cache");
    CopyFolder(CProjectManager::Instance()->QGetInstallPath() + "/debug/templates/res_templ/fonts",
               projectPath + "/res_templ/fonts");
    CopyFolder(CProjectManager::Instance()->QGetInstallPath() + "/debug/templates/res_templ/ui",
               projectPath + "/res_templ/ui");

    emit loadProject(a_sProjectFile);
}

void CLoaderWidget::validateNewProject()
{

}
