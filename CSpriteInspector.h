#ifndef CSPRITEINSPECTOR_H
#define CSPRITEINSPECTOR_H

#include <vector>

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

#include "Classes/Engine/Include/CSpriteNode.h"

class CSpriteInspector : public QWidget
{
    Q_OBJECT
public:
    explicit CSpriteInspector(QWidget *parent = 0);
    CSpriteInspector(LM::CSpriteNode* a_pSprite, QWidget *parent = 0);

signals:
    void closeInspector();

public slots:
    void closeInspectorSlot();
    void pathChanged(const QString&);
    void validatePath();
    void setAnchor();
    void openPathFileDialog();
    void newPathSelected(QString);

private:

    LM::CSpriteNode* m_pSprite;
    QLineEdit* m_pPath;
    int m_iCurrentAnchor;
    std::vector<QPushButton*> m_vAnchorButtons;
};

#endif // CSPRITEINSPECTOR_H
