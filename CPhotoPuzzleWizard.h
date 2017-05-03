#ifndef CPHOTOPUZZLEWIZARD_H
#define CPHOTOPUZZLEWIZARD_H

#include "ui_cphotopuzzlewizard.h"

#include <QDialog>
#include <QSpinBox>
#include <QVector>
#include <QComboBox>


class CPhotoPuzzleWizard : public QDialog
{
    Q_OBJECT

private:
    Ui_Dialog* ui;

    QSpinBox* m_pToFillGridRows;
    QSpinBox* m_pToFillGridColums;
    QSpinBox* m_pPiecesGridRows;
    QSpinBox* m_pPiecesGridColumns;

    QVector<QVector<QLabel*>> m_vToFillGridItems;
    QVector<QVector<QComboBox*>> m_vPiecesGridItems;


    void ClearLayout(QLayout* layout, bool deleteWidgets = true);


public:
    CPhotoPuzzleWizard(QWidget* parent = 0);
    void mousePressEvent(QMouseEvent* event) override;


public slots:
    void clickOnCancel(bool);
    void clickOnValidate(bool);
    void GenerateGrids();


};

#endif // CPHOTOPUZZLEWIZARD_H
