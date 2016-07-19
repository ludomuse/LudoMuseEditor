#ifndef CSCENEINSPECTOR_H
#define CSCENEINSPECTOR_H

#include "Classes/Engine/Include/CSceneNode.h"

#include <QWidget>
#include <QCheckBox>

class CSceneInspector : public QWidget
{
    Q_OBJECT
public:
    explicit CSceneInspector(LM::CSceneNode* a_pScene, int a_iPlayerID, QWidget *parent = 0);

signals:

public slots:

private:
    LM::CSceneNode* m_pScene;
};

#endif // CSCENEINSPECTOR_H
