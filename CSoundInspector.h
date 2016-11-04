#ifndef CSOUNDINSPECTOR_H
#define CSOUNDINSPECTOR_H

#include <vector>

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QSlider>
#include <QCheckBox>
#include <QRadioButton>

#include "Classes/Engine/Include/CSpriteNode.h"

class CSoundInspector : public QWidget
{
    Q_OBJECT
public:
    explicit CSoundInspector(QWidget *parent = 0);
    CSoundInspector(LM::CEntityNode* a_pNode, QWidget *parent = 0);

    QString GetEvent();
    void Initialize();
    bool PathIsValid();

signals:
    void closeInspector();
    void modifySound(LM::CEntityNode* a_pNode, QString a_sEvent, QString a_sArgument);
    void removeSound(LM::CEntityNode* a_pNode);

public slots:
//    void closeInspectorSlot();
    void pathChanged(const QString&);
//    void validatePath();
    void openPathFileDialog();
    void newPathSelected(QString);
    void newDirectoryEntered(QString);
    void validateChanges();
    void discardChanges();

private:
    LM::CEntityNode* m_pNode;

    QLineEdit* m_pPath;
    QCheckBox* m_pPlaySoundCheckButton;
    QRadioButton* m_pInitiateRadioButton;
    QRadioButton* m_pValidateRadioButton;
    QRadioButton* m_pInteractRadioButton;
    QRadioButton* m_pDisplaceRadioButton;

protected:
    virtual void closeEvent(QCloseEvent *event) override;
};

#endif // CSOUNDINSPECTOR_H
