#ifndef CIMPORTPROJECTWIZARD_H
#define CIMPORTPROJECTWIZARD_H

#include <QWidget>

namespace Ui {
class CImportProjectWizard;
}

class CImportProjectWizard : public QWidget
{
    Q_OBJECT

public:
    explicit CImportProjectWizard(QWidget *parent = 0);
    void ClearError();
    void AnimatedOpening();
    ~CImportProjectWizard();

private:
    bool CheckMandatoryFields();
    Ui::CImportProjectWizard *ui;

public slots:
    void clickOnValidate();
    void clickOnPathExplorer();
    void clickOnArchiveExplorer();

signals:
    void createNewProject(QString a_sProjectFile);
};

#endif // CIMPORTPROJECTWIZARD_H
