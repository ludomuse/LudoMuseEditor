#ifndef CLABELINSPECTOR_H
#define CLABELINSPECTOR_H

#include <QWidget>

#include <Classes/Engine/Include/CLabelNode.h>

class CLabelInspector : public QWidget
{
    Q_OBJECT
public:
    explicit CLabelInspector(QWidget *parent = 0);
    CLabelInspector(LM::CLabelNode* a_pLabel, QWidget *parent = 0);

signals:

public slots:

private:
    LM::CLabelNode* m_pLabel;
};

#endif // CLABELINSPECTOR_H
