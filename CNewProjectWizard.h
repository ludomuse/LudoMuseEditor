#ifndef CNEWPROJECTWIZARD_H
#define CNEWPROJECTWIZARD_H

#include <QWidget>

namespace Ui {
class CNewProjectWizard;
}

class CNewProjectWizard : public QWidget
{
    Q_OBJECT

public:
    explicit CNewProjectWizard(QWidget *parent = 0);
    void ClearError();
    void AnimatedOpening();
    ~CNewProjectWizard();

private:
    bool CheckMandatoryFields();
    Ui::CNewProjectWizard *ui;

public slots:
    void clickOnValidate();
    void clickOnPathExplorer();
    void folderSelected(const QString& a_sPath);

signals:
    void createNewProject(QString a_sProjectFile);
};

#endif // CNEWPROJECTWIZARD_H
