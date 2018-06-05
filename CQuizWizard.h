#ifndef CQUIZWIZARD_H
#define CQUIZWIZARD_H

#include "SNewGame.h"
#include "CWizard.h"

#include <QDialog>
#include <QSpinBox>
#include <QVector>
#include <QCheckBox>


namespace Ui {
class CQuizWizard;
}

class CQuizWizard : public CWizard
{
    Q_OBJECT


private:
    Ui::CQuizWizard *ui;

    QVector<QVector<QCheckBox*>> m_vPlayer1GridItems;
    QVector<QVector<QCheckBox*>> m_vPlayer2GridItems;


    void ClearLayout(QLayout* layout, bool deleteWidgets = true);
    void GenerateScenes();
    virtual void WizardValidated() override;

public:
    explicit CQuizWizard(QWidget* parent);
    ~CQuizWizard();

public slots:
    void GenerateGrids();

};

#endif // CQUIZWIZARD_H
