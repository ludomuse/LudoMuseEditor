#ifndef CSPRITEINSPECTOR_H
#define CSPRITEINSPECTOR_H

#include <vector>

#include <QWidget>
#include <QPushButton>

#include "Classes/Engine/Include/CSpriteNode.h"

class CSpriteInspector : public QWidget
{
    Q_OBJECT
public:
    explicit CSpriteInspector(QWidget *parent = 0);
    CSpriteInspector(LM::CSpriteNode* a_pSprite, QWidget *parent = 0);

signals:

public slots:
    void pathChanged(const QString&);
    void validatePath();
    void setAnchor();

private:

    LM::CSpriteNode* m_pSprite;
    QString m_sPath;
    int m_iCurrentAnchor;
    std::vector<QPushButton*> m_vAnchorButtons;
};

#endif // CSPRITEINSPECTOR_H
