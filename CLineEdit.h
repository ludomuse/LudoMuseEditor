#ifndef CLINEEDIT_H
#define CLINEEDIT_H

#include <QLineEdit>
#include <QDragEnterEvent>
#include <QStringList>
/*
 * Classe for drop handling in QLineEdit
 */
class CLineEdit : public QLineEdit
{
public:
    CLineEdit(QWidget* a_pParent);

protected:
    void dropEvent(QDropEvent* a_pEvent) override;
    void dragEnterEvent(QDragEnterEvent* a_pEvent) override;

private:
    QStringList m_lExtensionList;
};

#endif // CLINEEDIT_H
