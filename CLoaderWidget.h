#ifndef CLOADERWIDGET_H
#define CLOADERWIDGET_H

#include <QWidget>
#include <QString>


#include "CNewProjectWizard.h"
#include "CLoadProjectWizard.h"
#include "CImportProjectWizard.h"

namespace Ui {
class CLoaderWidget;
}

class CLoaderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CLoaderWidget(QWidget *parent = 0);
    ~CLoaderWidget();

private:
    void SetNewSelect(bool a_bActive);
    void SetLoadSelect(bool a_bActive);
    void SetImportSelect(bool a_bActive);
    void ClearWizardContainer();
    Ui::CLoaderWidget *ui;
    bool m_bNewProjectWizard;
    bool m_bLoadExistingProject;
    bool m_bImportProject;
    CNewProjectWizard* m_pNewProjectWizard;
    CLoadProjectWizard* m_pLoadProjectWizard;
    CImportProjectWizard* m_pImportProjectWizard;

public slots:
    void clickLoadProject();
    void clickNewProject();
    void clickCloseEditor();
    void clickImportProject();
    void projectSelected(QString);
    void createNewProject(QString);

signals:
    void loadProject(QString a_sProjectPath);
    void closeEditor();
};

#endif // CLOADERWIDGET_H
