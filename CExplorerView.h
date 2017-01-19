#ifndef CEXPLORERVIEW_H
#define CEXPLORERVIEW_H


#include <QListView>
#include <QDragMoveEvent>
#include <QString>
#include <QFileSystemModel>
#include <QDir>
#include <QAction>
#include <QTimer>

/*
 * Classe for drag and drop handling QListView
 */
class CFileSystemModel;

class CExplorerView : public QListView
{
    Q_OBJECT
public:
    CExplorerView(QWidget* a_pParent = Q_NULLPTR);
    void SetProjectPath(const QString& a_sProjectPath);
    void SetCurrentDir(const QString& a_sPath);
//        QDir GetCurrentDir();

protected:
    void dropEvent(QDropEvent* a_pEvent) override;
    void dragEnterEvent(QDragEnterEvent* a_pEvent) override;
    void dragMoveEvent(QDragMoveEvent* a_pEvent) override;
    void dragLeaveEvent(QDragLeaveEvent* a_pEvent) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    void CreateContextMenuActions();

private:
    QString m_sRoot;
    CFileSystemModel* m_pFileModel;

    QAction* m_aOpen;
    QAction* m_aRename;
    QAction* m_aErase;
    QAction* m_aNewDir;

    int m_iDelay = 1000;
    QTimer* m_oTimer;
    QModelIndex m_oHovered;

public slots:
//    void selectIndex(const QModelIndex& a_oIndex);

private slots:
    void open();
    void rename();
    void erase();
    void newDir();

    void dragIdle();
};

class CFileSystemModel : public QFileSystemModel
{
public :
    CFileSystemModel(QWidget* a_pParent = Q_NULLPTR);
    QVariant data(const QModelIndex& index, int role) const override;
};

#endif // CEXPLORERVIEW_H
