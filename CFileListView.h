#ifndef CFILELISTVIEW_H
#define CFILELISTVIEW_H


#include <QListView>
#include <QDragMoveEvent>
#include <QString>

/*
 * Classe for drag and drop handling QListView
 */

class CFileListView : public QListView
{
    Q_OBJECT
public:
    CFileListView(QWidget* a_pParent = Q_NULLPTR);
    void SetCurrentPath(QString a_sPath);

protected:
    void dropEvent(QDropEvent* a_pEvent) override;
    void dragEnterEvent(QDragEnterEvent* a_pEvent) override;
    void dragMoveEvent(QDragMoveEvent* a_pEvent) override;

private:
    QString m_sPath;
};

#endif // CFILELISTVIEW_H
