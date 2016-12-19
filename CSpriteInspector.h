#ifndef CSPRITEINSPECTOR_H
#define CSPRITEINSPECTOR_H

#include <vector>

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QSlider>
#include <QCheckBox>
#include <QRadioButton>

#include "Classes/Engine/Include/CSpriteNode.h"

class CSpriteInspector : public QWidget
{
    Q_OBJECT
public:
    explicit CSpriteInspector(QWidget *parent = 0);
    CSpriteInspector(LM::CSpriteNode* a_pSprite, QWidget *parent = 0);

signals:
    void closeInspector();
    void modifySprite(LM::CEntityNode* a_pSprite);
    void callSoundInspector();

public slots:
//    void closeInspectorSlot();
    void changePath(const QString&);
    void changeAnchor(int a_iAnchorID);
//    void validatePath();
//    void setAnchor();
//    void openPathFileDialog();
//    void newPathSelected(QString);
//    void newDirectoryEntered(QString);
    void heightSliderChange(int a_iValue);
    void widthSliderChange(int a_iValue);
    void heightTextChange(const QString& a_rText);
    void widthTextChange(const QString& a_rText);
    void checkHeight(bool a_rState);
    void checkWidth(bool a_rState);
    void validateChanges();
    void discardChanges();
    void editSound();

private:

    LM::CSpriteNode* m_pSprite;
//    QLineEdit* m_pPath;
    QLineEdit* m_pWidthValue;
    QLineEdit* m_pHeightValue;
//    int m_iCurrentAnchor;
    std::vector<QPushButton*> m_vAnchorButtons;
    QSlider* m_pHeightSlider;
    QSlider* m_pWidthSlider;
    QRadioButton* m_pWidthRadioButton;
    QRadioButton* m_pHeightRadioButton;

    std::string m_sSavedPath;
    int m_iSavedWidth;
    int m_iSavedHeight;
    int m_iSavedAnchor;

protected:
    virtual void closeEvent(QCloseEvent *event) override;
};

#endif // CSPRITEINSPECTOR_H
