#ifndef CSCENEINSPECTOR_H
#define CSCENEINSPECTOR_H

#include "Classes/Engine/Include/CSceneNode.h"

#include <QWidget>
#include <QCheckBox>
#include <QString>

class CSceneInspector : public QWidget
{
    Q_OBJECT
public:
    explicit CSceneInspector(LM::CSceneNode* a_pScene, int a_iPlayerID, QWidget *parent = 0);

signals:
    void addScene();
    void deleteScene(QString a_sSceneID);

public slots:
    void switchSynchro(bool a_bState);
    void switchDash(bool a_bState);
    void switchP1(bool a_bState);
    void switchP2(bool a_bState);
    void clickOnDelete();
    void clickOnAdd();

private:
    LM::CSceneNode* m_pScene;
};

#endif // CSCENEINSPECTOR_H
