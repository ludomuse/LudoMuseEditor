#ifndef CDASHBOARDINSPECTOR_H
#define CDASHBOARDINSPECTOR_H

#include "Classes/Engine/Include/CSceneNode.h"
#include "Classes/Engine/Include/CGridNode.h"

#include <QWidget>
#include <QSpinBox>

class CDashboardInspector : public QWidget
{
    Q_OBJECT

public:
    CDashboardInspector(LM::CSceneNode* a_pDashboard, QWidget *parent = 0);

public slots:
    void ChangeGrid(bool a_bState);
    void CancelChangeGrid(bool a_bState);

private:
    LM::CSceneNode* m_pDashboard;
    LM::CGridNode* m_pRewardGrid;

    QSpinBox* m_pRowCountSpin;
    QSpinBox* m_pColCountSpin;
};

#endif // CDASHBOARDINSPECTOR_H
