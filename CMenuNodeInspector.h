#ifndef CMENUNODEINSPECTOR_H
#define CMENUNODEINSPECTOR_H

#include <QWidget>

#include <Classes/Engine/Include/CMenuNode.h>

namespace Ui {
class CMenuNodeInspector;
}

class CMenuNodeInspector : public QWidget
{
    Q_OBJECT

public:
    CMenuNodeInspector(QWidget* parent = 0);
    CMenuNodeInspector(LM::CMenuNode* a_pMenuNode, QWidget *parent = 0);
    ~CMenuNodeInspector();

private:
    Ui::CMenuNodeInspector *ui;
    LM::CMenuNode* m_pMenuNode;

    std::string m_sSavedText;
    bool m_bSavedNav;

protected:
    virtual void closeEvent(QCloseEvent *event);

public slots:
    void checkNext(bool);
    void checkPrev(bool);
    void textChanged(QString);
//    void closeInspectorSlot();
    void validateChanges();
    void discardChanges();

signals:
    void closeInspector();
};

#endif // CMENUNODEINSPECTOR_H
