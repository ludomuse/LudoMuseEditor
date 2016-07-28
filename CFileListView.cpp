#include "CFileListView.h"

#include <QDebug>
#include <QMimeData>
#include <QFileSystemModel>

CFileListView::CFileListView(QWidget* a_pParent):
    QListView(a_pParent),
    m_sPath("")
{
    this->setAcceptDrops(true);
    this->setDropIndicatorShown(true);
    this->setDragDropMode(QAbstractItemView::DragDrop);
    this->setDragDropOverwriteMode(true);
}



void CFileListView::SetCurrentPath(QString a_sPath)
{
    this->m_sPath = a_sPath;
}


void CFileListView::dropEvent(QDropEvent* a_pEvent)
{
    if(a_pEvent->mimeData()->hasFormat("FileName"))
    {
        QString filePath= a_pEvent->mimeData()->data("FileName");

        // Get only the filename not the whole path
        QFileInfo fileInfo(filePath);

        // Protection if current path not set yet
        if(!this->m_sPath.isEmpty())
        {
            QFile* testFile = new QFile(m_sPath+"\\"+fileInfo.fileName());
            if(testFile->exists())
            {
                qDebug()<<"File already exist, no copy done";
            }
            else
            {
                qDebug()<< " Copy from " << filePath << " to " << (m_sPath+"\\"+fileInfo.fileName());
                QFile::copy(filePath, m_sPath + "\\" + fileInfo.fileName());
            }
        }
    }
}

void CFileListView::dragEnterEvent(QDragEnterEvent *a_pEvent)
{
    qDebug("Drag enter");
    a_pEvent->acceptProposedAction();
}

void CFileListView::dragMoveEvent(QDragMoveEvent *a_pEvent)
{
    qDebug("Drag move");
    a_pEvent->acceptProposedAction();
}
