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
    void modifyLabel(LM::CEntityNode* a_pLabel);

public slots:
    void changeText();
    void changeAnchor(int a_iAnchorID);
    void changePath(const QString& a_sPath);
    void changeFontSize(int a_iSize);
    void changeColor(const QString& a_sColor);
    void heightSliderChange(int a_iValue);
    void widthSliderChange(int a_iValue);
    void heightTextChange(const QString& a_rText);
    void widthTextChange(const QString& a_rText);
    void validateChanges();
    void discardChanges();

private:
    LM::CLabelNode* m_pLabel;
    QTextEdit* m_pTextEdit;
    QSlider* m_pHeightSlider;
    QSlider* m_pWidthSlider;
    QLineEdit* m_pWidthValue;
    QLineEdit* m_pHeightValue;

    std::string m_sSavedText;
    int m_iSavedHeight;
    int m_iSavedWidth;
    int m_iSavedAnchor;
    std::string m_sSavedFont;
    int m_iSavedFontSize;
    std::string m_sSavedColor;

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // CLABELINSPECTOR_H
