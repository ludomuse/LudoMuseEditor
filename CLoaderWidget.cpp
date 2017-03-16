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
    CProjectManager::Instance(); // Build CprojectManager instance.

    ui->setupUi(this);
    this->ClearWizardContainer();
    ui->wizardContainer->layout()->addWidget(m_pLoadProjectWizard);
    ui->wizardContainer->layout()->addWidget(m_pNewProjectWizard);
    connect(ui->exitButton, SIGNAL(clicked(bool)), this, SLOT(clickCloseEditor()));
    connect(ui->newButton, SIGNAL(clicked(bool)), this, SLOT(clickNewProject()));
    connect(ui->loadButton, SIGNAL(clicked(bool)), this, SLOT(clickLoadProject()));
    connect(m_pLoadProjectWizard, SIGNAL(loadProjectFile(QString)), this, SLOT(projectSelected(QString)));
    connect(m_pNewProjectWizard, SIGNAL(createNewProject(QString)), this, SLOT(createNewProject(QString)));

    ui->loadButton->setIcon(QIcon(CProjectManager::Instance()->QGetInstallPath().append("/resources/ic_open_in_browser_white_48dp_2x.png")));
    ui->newButton->setIcon(QIcon(CProjectManager::Instance()->QGetInstallPath().append("/resources/ic_create_new_folder_white_48dp_2x.png")));
    ui->exitButton->setIcon(QIcon(CProjectManager::Instance()->QGetInstallPath().append("/resources/ic_close_white_48dp_2x.png")));
}

CLoaderWidget::~CLoaderWidget()
{
    delete ui;
}

void CLoaderWidget::SetNewSelect(bool a_bActive)
{
    if(a_bActive)
    {
        ui->newProject->setStyleSheet("#newProject{"
                                      "border-left : 1px solid rgb(255, 170, 0);"
                                      "border-bottom : 1px solid rgb(255, 170, 0); "
                                      "border-top : 1px solid rgb(255, 170, 0); "
                                      "background-color : rgb(65,65,65);}");

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
        ui->loadProject->setStyleSheet("#loadProject{"
                                       "border-left : 1px solid rgb(255, 170, 0); "
                                       "border-bottom : 1px solid rgb(255, 170, 0); "
                                       "border-top : 1px solid rgb(255, 170, 0); "
                                       "background-color : rgb(65,65,65);}");

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
    m_pLoadProjectWizard->ClearError();
    m_pNewProjectWizard->setVisible(false);
    m_pNewProjectWizard->ClearError();
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
        m_pNewProjectWizard->AnimatedOpening();
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
        m_pLoadProjectWizard->AnimatedOpening();
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
    CopyFolder(CProjectManager::Instance()->QGetInstallPath() + "/default/cache", projectPath + "/cache");
    CopyFolder(CProjectManager::Instance()->QGetInstallPath() + "/default/fonts", projectPath + "/fonts");
    CopyFolder(CProjectManager::Instance()->QGetInstallPath() + "/default/ui", projectPath + "/ui");

    emit loadProject(a_sProjectFile);
}

void CLoaderWidget::createNewProject(QString a_sNewProjectFile)
{
    emit loadProject(a_sNewProjectFile);
}
