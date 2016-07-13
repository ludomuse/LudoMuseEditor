#include "CLineEdit.h"

#include <QDebug>
#include <QMimeData>
#include <QUrl>
#include <QList>
#include <QFileInfo>
#include <QDir>


CLineEdit::CLineEdit(QWidget* a_pParent):
    QLineEdit(a_pParent)
{
    this->setAcceptDrops(true);
    this->setReadOnly(true);
    m_lExtensionList.push_back("jpg");
    m_lExtensionList.push_back("png");
    m_lExtensionList.push_back("bmp");
}



void CLineEdit::dropEvent(QDropEvent* a_pEvent)
{
    qDebug("call dropEvent");
    qDebug()<<a_pEvent->mimeData()->formats();
    if(a_pEvent->mimeData()->hasFormat("text/uri-list"))
    {
        QList<QUrl> urls = a_pEvent->mimeData()->urls();
        for(QUrl currentUrl : urls)
        {
            QString path = currentUrl.path();
            path = path.mid(1);
            QFileInfo newFile(path);
            if(newFile.exists())
            {
                if(this->m_lExtensionList.contains(newFile.suffix()))
                {
                    qDebug("accepted format");
                    // Checking if file is in project path
                    if(path.contains(QDir::currentPath()))
                    {
                        QString projectPath = QDir::currentPath();
                        path.replace(projectPath+'/', "");
                        this->setText(path);
                    }
                    else
                    {
                        qDebug("File not in project path");
                    }
                }
                else
                {
                    qDebug("refused format");
                }

            }
        }

    }
    else
    {
        qDebug("No file was droped");
    }
}

void CLineEdit::dragEnterEvent(QDragEnterEvent* a_pEvent)
{
    qDebug("call dragEnterEvent");
    a_pEvent->acceptProposedAction();
}
