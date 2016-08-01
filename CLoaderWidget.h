#ifndef CLOADERWIDGET_H
#define CLOADERWIDGET_H

#include <QWidget>
#include <QString>

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
    void ClearWizardContainer();
    Ui::CLoaderWidget *ui;
    bool m_bNewProjectWizard;
    bool m_bLoadExistingProject;

public slots:
    void clickLoadProject();
    void clickNewProject();
    void clickCloseEditor();
    void projectSelected(QString);
    void createNewProject(QString);

signals:
    void loadProject(QString a_sProjectPath);
    void closeEditor();
};

#endif // CLOADERWIDGET_H
