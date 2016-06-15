#ifndef CSPRITEINSPECTOR_H
#define CSPRITEINSPECTOR_H

#include <QWidget>

#include "Classes/Engine/Include/CSpriteNode.h"

class CSpriteInspector : public QWidget
{
    Q_OBJECT
public:
    explicit CSpriteInspector(QWidget *parent = 0);
    CSpriteInspector(LM::CSpriteNode* a_pSprite, QWidget *parent = 0);

signals:

public slots:

private:
    LM::CSpriteNode* m_pSprite;
};

#endif // CSPRITEINSPECTOR_H
