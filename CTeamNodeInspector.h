#ifndef CTEAMNODEINSPECTOR_H
#define CTEAMNODEINSPECTOR_H

#include <vector>

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

#include "Classes/Engine/Include/CTeamNode.h"

class CTeamNodeInspector : public QWidget
{
    Q_OBJECT
public:
    explicit CTeamNodeInspector(QWidget *parent = 0);
    CTeamNodeInspector(LM::CTeamNode* a_pTeamNode, QWidget *parent = 0);

signals:
    void closeInspector();
    void modifyTeamNode(LM::CEntityNode* a_pSprite);

public slots:
    void validateChanges();
    void discardChanges();

private:

    LM::CTeamNode* m_pTeamNode;
    LM::TTasksArray m_oSavedTasks;

protected:
    virtual void closeEvent(QCloseEvent *event) override;
};

#endif // CTEAMNODEINSPECTOR_H
