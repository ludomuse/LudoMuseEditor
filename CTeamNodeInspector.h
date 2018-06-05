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
    void boxChecked(int boxResult);
    void validateChanges();
    void discardChanges();
    void changeTask(QString a_oNewTask);
    void changeAction(QString a_oNewAction);

private:

    LM::CTeamNode* m_pTeamNode;
    LM::TTasksArray m_oSavedTasks;

    std::vector<QLineEdit*> m_oTasks;
    std::vector<QLineEdit*> m_oActions;

    bool bChecked;

    std::vector<std::string> m_oDefaultTextsActions;
    std::vector<std::string> m_oDefaultImagesPathActions;

protected:
    virtual void closeEvent(QCloseEvent *event) override;
};

#endif // CTEAMNODEINSPECTOR_H
