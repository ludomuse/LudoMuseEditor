#include "CExplorerView.h"

#include <QDebug>
#include <QMimeData>
#include <QFileSystemModel>
#include <QFile>
#include <QDesktopServices>
#include <QApplication>
#include <QMenu>
#include <QMessageBox>

CExplorerView::CExplorerView(QWidget* a_pParent):
    QListView(a_pParent),
    m_sRoot(""),
    m_oTimer(new QTimer(this)),
    m_pFileModel(new CFileSystemModel(this))
{
    m_pFileModel->setReadOnly(true);
    this->setContextMenuPolicy(Qt::NoContextMenu);

//    m_pFileModel = new CFileSystemModel(this);
//    m_pFileModel->setFilter(QDir::NoDot | QDir::Files | QDir::Dirs);
//    m_pFileModel->sort(1);
//    setModel(m_pFileModel);
////    SetRootPath(a_sProjectPath);
//    SetCurrentDir(a_sProjectPath);
////    m_pFileView->setViewMode(QListView::IconMode);
//    setSelectionMode( QAbstractItemView::ExtendedSelection );
//    CreateContextMenuActions();

//    this->show();

//    this->setEditTriggers(QAbstractItemView::EditKeyPressed | QAbstractItemView::SelectedClicked);
//    this->setAcceptDrops(true);
//    this->setDropIndicatorShown(true);
//    this->setDragDropMode(QAbstractItemView::DragDrop);
//    this->setDragDropOverwriteMode(true);
//    this->setContextMenuPolicy(Qt::DefaultContextMenu);

//            //    connect(this, SIGNAL(doubleClicked()), this, SLOT(open()));
//    m_oTimer->setSingleShot(true);
//    connect(m_oTimer, SIGNAL(timeout()), this, SLOT(dragIdle()));
}

void CExplorerView::SetProjectPath(const QString& a_sProjectPath)
{
    m_sRoot = QString(a_sProjectPath);
//    m_pFileModel = new CFileSystemModel(this);
    m_pFileModel->setFilter(QDir::NoDot | QDir::Files | QDir::Dirs);
    m_pFileModel->sort(1);
    setModel(m_pFileModel);
    m_pFileModel->setReadOnly(false);
    SetCurrentDir(a_sProjectPath);
    setCurrentIndex(rootIndex());
    setSelectionMode( QAbstractItemView::ExtendedSelection );
    CreateContextMenuActions();

    this->show();
    this->setEditTriggers(QAbstractItemView::EditKeyPressed | QAbstractItemView::SelectedClicked);
    this->setAcceptDrops(true);
    this->setDropIndicatorShown(true);
    this->setDragDropMode(QAbstractItemView::DragDrop);
    this->setDragDropOverwriteMode(true);
    this->setContextMenuPolicy(Qt::DefaultContextMenu);

    m_oTimer->setSingleShot(true);
    connect(m_oTimer, SIGNAL(timeout()), this, SLOT(dragIdle()));
}

void CExplorerView::CreateContextMenuActions()
{
    m_aRename = new QAction(tr("Renomer"), this);
//    m_aRename->setShortcuts(QKeySequence::Replace);
    m_aRename->setStatusTip(tr("Renome le fichier ou le dossier"));
    addAction(m_aRename);
    connect(m_aRename, SIGNAL(triggered()), this, SLOT(rename()));

    m_aErase = new QAction(tr("Supprimer"), this);
    m_aErase->setShortcut(QKeySequence::Delete);
    m_aErase->setStatusTip(tr("Renome le fichier ou le dossier"));
    addAction(m_aErase);
    connect(m_aErase, SIGNAL(triggered()), this, SLOT(erase()));

    m_aOpen = new QAction(tr("Ouvrir"), this);
    m_aOpen->setShortcut(QKeySequence::Open);
    m_aOpen->setStatusTip(tr("Ouvrir le fichier ou le dossier"));
    addAction(m_aOpen);
    connect(m_aOpen, SIGNAL(triggered()), this, SLOT(open()));

    m_aNewDir = new QAction(tr("Nouveau dossier"), this);
    m_aNewDir->setShortcut(QKeySequence::New);
    m_aNewDir->setStatusTip(tr("Créer un nouveau dossier"));
    addAction(m_aNewDir);
    connect(m_aNewDir, SIGNAL(triggered()), this, SLOT(newDir()));
}

//void CExplorerView::SetRootPath(const QString &a_sPath)
//{
//    setRootIndex(m_pFileModel->setRootPath(a_sPath));
//    this->m_sRoot = m_pFileModel->rootPath();
//}

void CExplorerView::SetCurrentDir(const QString& a_sPath)
{
    setRootIndex(m_pFileModel->setRootPath(a_sPath));
    if (QFileInfo(m_sRoot) == QFileInfo(a_sPath))
    {
        m_pFileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs);
    }
    else {
        m_pFileModel->setFilter(QDir::NoDot | QDir::Files | QDir::Dirs);
    }
    m_pFileModel->sort(0);
}


void CExplorerView::dropEvent(QDropEvent* a_pEvent)
{
    m_oTimer->stop();
    m_oHovered = QModelIndex();
    if (a_pEvent->mimeData()->hasUrls())
    {
        QString sCurrentPath;
        QFileInfo sCurrentFile = m_pFileModel->fileInfo(currentIndex());
        if (sCurrentFile.isDir())
        {
            sCurrentPath = sCurrentFile.absoluteFilePath();
        }
        else
        {
            sCurrentPath = sCurrentFile.absolutePath();
        }
        qDebug() << sCurrentPath;
        QListIterator<QUrl> itUrl(a_pEvent->mimeData()->urls());
        for (itUrl.toFront(); itUrl.hasNext(); itUrl.next())
        {
            QFile* pNewFile = new QFile(sCurrentPath+"/"+itUrl.peekNext().fileName());
            if(pNewFile->exists())
            {
                qDebug()<<"File already exist, no copy done";
            }
            else
            {
                qDebug()<< " Copy from " << itUrl.peekNext().toLocalFile() << " to " << (sCurrentPath+"/"+itUrl.peekNext().fileName());
                QFile::copy(itUrl.peekNext().toLocalFile(), sCurrentPath+"/"+itUrl.peekNext().fileName());
            }
        }
    }
}

void CExplorerView::dragEnterEvent(QDragEnterEvent *a_pEvent)
{
    if (a_pEvent->mimeData()->hasUrls()) {
        a_pEvent->acceptProposedAction();
    }
}

void CExplorerView::dragMoveEvent(QDragMoveEvent *a_pEvent)
{
    QModelIndex hovered = indexAt(a_pEvent->pos());
    if (hovered.isValid() && m_pFileModel->isDir(hovered))
    {
        if (hovered != m_oHovered)
        {
            m_oHovered = hovered;
            m_oTimer->start(m_iDelay);
        }
        else if (!m_oTimer->isActive())
        {
            m_oTimer->start(m_iDelay);
        }
    }
    else
    {
        m_oTimer->stop();
    }
}

void CExplorerView::dragLeaveEvent(QDragLeaveEvent *a_pEvent)
{
    m_oTimer->stop();
}

void CExplorerView::dragIdle()
{
    qDebug() << "timer end";
    if (m_oHovered.isValid() && m_pFileModel->isDir(m_oHovered))
    {
        setCurrentIndex(m_oHovered);
        SetCurrentDir(m_pFileModel->filePath(m_oHovered));
    }
}

void CExplorerView::contextMenuEvent(QContextMenuEvent *event)
{
    QModelIndex clicked = indexAt(event->pos());
    if (!selectedIndexes().contains(clicked))
    {
        setCurrentIndex(clicked);
    }
    QMenu menu(this);
    if (m_aOpen != nullptr && currentIndex().isValid())
        menu.addAction(m_aOpen);
    if (m_aRename != nullptr && currentIndex().isValid())
        menu.addAction(m_aRename);
    if (m_aErase != nullptr && !selectedIndexes().empty())
        menu.addAction(m_aErase);
    if (m_aNewDir != nullptr)
        menu.addAction(m_aNewDir);
    menu.exec(event->globalPos());
}

void CExplorerView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QModelIndex clicked = indexAt(event->pos());
    setCurrentIndex(clicked);
    open();
}

void CExplorerView::open()
{
    if(currentIndex().isValid())
    {
        if (m_pFileModel->isDir(currentIndex()))
        {
            SetCurrentDir(m_pFileModel->filePath(currentIndex()));
        }
        else
        {
            QDesktopServices::openUrl(QUrl::fromLocalFile(m_pFileModel->filePath(currentIndex())));
        }
    }
}

void CExplorerView::rename()
{
    if(currentIndex().isValid())
    {
        this->edit(currentIndex());
    }
}

void CExplorerView::erase()
{
    if (selectedIndexes().length()>0)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, tr("Attention"),
                                     tr("Souhaitez-vous supprimer le(s) fichier(s) sélectionné(s) ? (la suppression est irréversible)"),
                                     QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            QModelIndexList selected = selectedIndexes();
            QModelIndexList::iterator it;
            for (it = selected.begin(); it < selected.end(); it++) {
                m_pFileModel->remove(*it);
            }
        }
    }
}

void CExplorerView::newDir()
{
    QModelIndex newDir = m_pFileModel->mkdir(rootIndex(), tr("Nouveau dossier"));
    if (newDir.isValid())
    {
        setCurrentIndex(newDir);
    }
}


CFileSystemModel::CFileSystemModel(QWidget* a_pParent):
    QFileSystemModel(a_pParent)
{

}

QVariant CFileSystemModel::data(const QModelIndex& index, int role) const
{
    if(role == Qt::DecorationRole && index.data() == "..")
    {
        return QApplication::style()->standardIcon(QStyle::SP_ArrowBack);
    }
    return QFileSystemModel::data(index, role);
}
