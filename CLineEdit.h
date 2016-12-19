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
    Q_OBJECT
public:
    CLineEdit(const QStringList& a_lExtensionList, QWidget* a_pParent);

protected:
    void dropEvent(QDropEvent* a_pEvent) override;
    void dragEnterEvent(QDragEnterEvent* a_pEvent) override;

private:
    QStringList m_lExtensionList;

//public slots:
//    void setPath (QString a_sPath);

//signals:
//    void pathChanged (QString a_sPath);
};

#endif // CLINEEDIT_H
