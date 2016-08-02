#ifndef CLOADPROJECTWIZARD_H
#define CLOADPROJECTWIZARD_H

#include <QWidget>
#include <QString>

namespace Ui {
class CLoadProjectWizard;
}

class CLoadProjectWizard : public QWidget
{
    Q_OBJECT

public:
    explicit CLoadProjectWizard(QWidget *parent = 0);
    ~CLoadProjectWizard();
    void ClearError();
    void AnimatedOpening();

private:
    Ui::CLoadProjectWizard *ui;
    bool CheckMandatoryFields();
    void CreatePreviousProjectButtons();

public slots:
    void clickOnValidate();
    void clickOnPathExplorer();
    void folderSelected(const QString& a_sPath);
    void clickOnPrevProject(const QString& a_sProjectFile);

signals:
    void loadProjectFile(const QString& a_sFilePath);

};

#endif // CLOADPROJECTWIZARD_H
