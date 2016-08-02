#include "CLoaderWidget.h"
#include "ui_CLoaderWidget.h"

#include <UtilsQt.h>
#include <CProjectManager.h>

#include <QDir>
#include <QFileDialog>


CLoaderWidget::CLoaderWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CLoaderWidget),
    m_bNewProjectWizard(false),
    m_bLoadExistingProject(false),
    m_pLoadProjectWizard(new CLoadProjectWizard()),
    m_pNewProjectWizard(new CNewProjectWizard())
{
    ui->setupUi(this);
    this->ClearWizardContainer();
    ui->wizardContainer->layout()->addWidget(m_pLoadProjectWizard);
    ui->wizardContainer->layout()->addWidget(m_pNewProjectWizard);
    connect(ui->exitButton, SIGNAL(clicked(bool)), this, SLOT(clickCloseEditor()));
    connect(ui->newButton, SIGNAL(clicked(bool)), this, SLOT(clickNewProject()));
    connect(ui->loadButton, SIGNAL(clicked(bool)), this, SLOT(clickLoadProject()));
    connect(m_pLoadProjectWizard, SIGNAL(loadProjectFile(QString)), this, SLOT(projectSelected(QString)));
    connect(m_pNewProjectWizard, SIGNAL(createNewProject(QString)), this, SLOT(createNewProject(QString)));
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
//    QLayout* wizardContainerLayout = ui->wizardContainer->layout();
//    QLayoutItem *child;
//    while ((child = wizardContainerLayout->takeAt(0)) != 0) {
//        delete child->widget();
//        delete child;
//    }
    m_pLoadProjectWizard->setVisible(false);
    m_pLoadProjectWizard->clearError();
    m_pNewProjectWizard->setVisible(false);
    m_pNewProjectWizard->clearError();
}

void CLoaderWidget::clickCloseEditor()
{
    emit closeEditor();
}

void CLoaderWidget::clickNewProject()
{
    m_bLoadExistingProject = false; // disable other wizard
    this->ClearWizardContainer();
    if(!m_bNewProjectWizard)
    {
        this->SetNewSelect(true);
        this->SetLoadSelect(false);
        //ui->wizardContainer->layout()->addWidget(newWizard);
        //connect(newWizard, SIGNAL(createNewProject(QString)), this, SLOT(createNewProject(QString)));
        m_pNewProjectWizard->setVisible(true);
    }
    else
    {
        this->SetNewSelect(false);
    }
    m_bNewProjectWizard = !m_bNewProjectWizard;
}

void CLoaderWidget::clickLoadProject()
{
    m_bNewProjectWizard = false; // disable other wizard
    this->ClearWizardContainer();
    if(!m_bLoadExistingProject)
    {
        this->SetLoadSelect(true);
        this->SetNewSelect(false);
        m_pLoadProjectWizard->setVisible(true);
    }
    else
    {
        this->SetLoadSelect(false);
    }
    m_bLoadExistingProject = !m_bLoadExistingProject;
}

void CLoaderWidget::projectSelected(QString a_sProjectFile)
{
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

void CLoaderWidget::createNewProject(QString a_sNewProjectFile)
{
    emit loadProject(a_sNewProjectFile);
}
