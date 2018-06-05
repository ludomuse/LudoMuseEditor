#ifndef CPHOTOPUZZLEWIZARD_H
#define CPHOTOPUZZLEWIZARD_H

#include "ui_cphotopuzzlewizard.h"
#include "SNewGame.h"
#include "CWizard.h"

#include <QDialog>
#include <QSpinBox>
#include <QVector>
#include <QComboBox>


namespace LM
{
class CKernel;
}

class CPhotoPuzzleWizard : public CWizard
{
    Q_OBJECT

private:
    Ui_Dialog* ui;

    QVector<QVector<QLabel*>> m_vToFillGridItems;
    QVector<QVector<QComboBox*>> m_vPiecesGridItems;


    void ClearLayout(QLayout* layout, bool deleteWidgets = true);
    void GenerateScenes(const std::vector<int>& values);
    virtual void WizardValidated() override;

public:
    CPhotoPuzzleWizard(QWidget* parent);
    void mousePressEvent(QMouseEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;


public slots:
    void GenerateGrids();


};

#endif // CPHOTOPUZZLEWIZARD_H
