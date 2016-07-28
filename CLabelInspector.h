#ifndef CLABELINSPECTOR_H
#define CLABELINSPECTOR_H

#include <QWidget>
#include <QString>
#include <QTextEdit>
#include <QSlider>
#include <QLineEdit>

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
    void heightSliderChange(int a_iValue);
    void widthSliderChange(int a_iValue);
    void heightTextChange(const QString& a_rText);
    void widthTextChange(const QString& a_rText);

private:
    LM::CLabelNode* m_pLabel;
    QTextEdit* m_pTextEdit;
    QSlider* m_pHeightSlider;
    QSlider* m_pWidthSlider;
    QLineEdit* m_pWidthValue;
    QLineEdit* m_pHeightValue;
};

#endif // CLABELINSPECTOR_H
