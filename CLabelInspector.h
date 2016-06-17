#ifndef CLABELINSPECTOR_H
#define CLABELINSPECTOR_H

#include <QWidget>
#include <QString>
#include <QTextEdit>

#include <Classes/Engine/Include/CLabelNode.h>

class CLabelInspector : public QWidget
{
    Q_OBJECT
public:
    explicit CLabelInspector(QWidget *parent = 0);
    CLabelInspector(LM::CLabelNode* a_pLabel, QWidget *parent = 0);

signals:
    void closeInspector();

public slots:
    void changeText();
    void closeInspectorSlot();

private:
    LM::CLabelNode* m_pLabel;
    QTextEdit* m_pTextEdit;
};

#endif // CLABELINSPECTOR_H
