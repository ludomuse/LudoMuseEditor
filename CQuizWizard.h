#ifndef CQUIZWIZARD_H
#define CQUIZWIZARD_H

#include "SNewGame.h"

#include <QDialog>
#include <QSpinBox>
#include <QVector>
#include <QCheckBox>

namespace LM
{
class CKernel;
}

namespace Ui {
class CQuizWizard;
}

class CQuizWizard : public QDialog
{
    Q_OBJECT


private:
    Ui::CQuizWizard *ui;

    SNewGameInfo m_oNewGameInfo;
    LM::CKernel* m_pKernel;


    QVector<QVector<QCheckBox*>> m_vPlayer1GridItems;
    QVector<QVector<QCheckBox*>> m_vPlayer2GridItems;


    void ClearLayout(QLayout* layout, bool deleteWidgets = true);
    void GenerateScenes();


public:
    explicit CQuizWizard(const SNewGameInfo& a_rNewGame, LM::CKernel* a_pKernel, QWidget *parent = 0);
    ~CQuizWizard();

public slots:
    void clickOnCancel(bool);
    void clickOnValidate(bool);
    void GenerateGrids();

};

#endif // CQUIZWIZARD_H
