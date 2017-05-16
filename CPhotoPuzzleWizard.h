#ifndef CPHOTOPUZZLEWIZARD_H
#define CPHOTOPUZZLEWIZARD_H

#include "ui_cphotopuzzlewizard.h"
#include "SNewGame.h"

#include <QDialog>
#include <QSpinBox>
#include <QVector>
#include <QComboBox>


namespace LM
{
class CKernel;
}

class CPhotoPuzzleWizard : public QDialog
{
    Q_OBJECT

private:
    Ui_Dialog* ui;

    SNewGameInfo m_oNewGameInfo;
    LM::CKernel* m_pKernel;


    QVector<QVector<QLabel*>> m_vToFillGridItems;
    QVector<QVector<QComboBox*>> m_vPiecesGridItems;


    void ClearLayout(QLayout* layout, bool deleteWidgets = true);
    void GenerateScenes(const std::vector<int>& values);

public:
    CPhotoPuzzleWizard(const SNewGameInfo& a_rNewGame, LM::CKernel* a_pKernel, QWidget* parent = 0);
    void mousePressEvent(QMouseEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;


public slots:
    void clickOnCancel(bool);
    void clickOnValidate(bool);
    void GenerateGrids();


};

#endif // CPHOTOPUZZLEWIZARD_H
