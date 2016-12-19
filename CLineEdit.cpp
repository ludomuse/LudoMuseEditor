#include "CLineEdit.h"

#include <QDebug>
#include <QMimeData>
#include <QUrl>
#include <QList>
#include <QFileInfo>
#include <QDir>
#include <QRegExp>
#include <QRegExpValidator>
#include "CProjectManager.h"


CLineEdit::CLineEdit(const QStringList& a_lExtensionList, QWidget* a_pParent):
    QLineEdit(a_pParent)
{
    this->setAcceptDrops(true);
    this->setReadOnly(true);
    //    m_lExtensionList.push_back("jpg");
    //    m_lExtensionList.push_back("png");
    //    m_lExtensionList.push_back("bmp");
    m_lExtensionList = a_lExtensionList;
    //    QString sExtRegExp;
    //    sExtRegExp = QString("^"+CProjectManager::Instance()->QGetProjectPath()+".*(" + a_lExtensionList.join("|") + ")$");
    //    qDebug() << sExtRegExp;
    //    setValidator(new QRegExpValidator(QRegExp(sExtRegExp)));
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
            QString path = currentUrl.toLocalFile();
            qDebug() << path;
            //            selectAll();
            //            insert(path);
            QFileInfo newFile(path);
            if(newFile.exists())
            {
                if(this->m_lExtensionList.contains(newFile.suffix()))
                {
                    qDebug("accepted format");
                    // Checking if file is in project path
                    //  if(path.contains(QDir::currentPath()))
                    QString projectPath = CProjectManager::Instance()->QGetProjectPath();
                    if (path.contains(projectPath))
                    {
                        path.replace(projectPath+'/', "");
                        selectAll();
                        insert(path);
                        //                        this->setText(path);
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

//void CLineEdit::setPath(QString a_sPath)
//{
//    this->setText(a_sPath);
//}
