#include "CLineEdit.h"

#include <QDebug>
#include <QMimeData>
#include <QUrl>
#include <QList>
#include <QFileInfo>
#include <QDir>
#include <QRegExp>
#include <QMessageBox>
#include <QRegExpValidator>
#include "CProjectManager.h"
#include "Classes/Engine/Include/CMacroManager.h"
#include "ETypes.h"


CLineEdit::CLineEdit(ETypes::Type a_eType, QWidget* a_pParent):
    QLineEdit(a_pParent)
{
    this->setAcceptDrops(true);
    this->setReadOnly(true);
    this->setPlaceholderText("Faites glisser une ressource");
    m_eType = a_eType;
    //    m_lExtensionList.push_back("jpg");
    //    m_lExtensionList.push_back("png");
    //    m_lExtensionList.push_back("bmp");
//    m_lExtensionList = a_lExtensionList;
    //    QString sExtRegExp;
    //    sExtRegExp = QString("^"+CProjectManager::Instance()->QGetProjectPath()+".*(" + a_lExtensionList.join("|") + ")$");
    //    qDebug() << sExtRegExp;
    //    setValidator(new QRegExpValidator(QRegExp(sExtRegExp)));
}



void CLineEdit::dropEvent(QDropEvent* a_pEvent)
{
    qDebug("call dropEvent");
    qDebug()<<a_pEvent->mimeData()->formats();
    this->setFocus();
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
//                if(m_lExtensionList.contains(newFile.suffix()))
                if (ETypes::TypeFromExtension(newFile.suffix()) == m_eType)
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
    else if (a_pEvent->mimeData()->hasFormat("text/plain"))
    {
        QString macro = a_pEvent->mimeData()->text();
        if (macro.at(0) == '#')
        {
            QStringList list = macro.split(':');
            QString type = list.takeLast();
            if (ETypes::TypeFromString(type) == m_eType)
            {
                QString name = list.join("");
                if (LM::CMacroManager::Instance()->GetDefinition(name.toStdString()).empty())
                {
                    QMessageBox msgBox;
                    msgBox.setText(tr("Ajout impossible : la macro ne contient pas de définition."));
                    msgBox.setStandardButtons(QMessageBox::Ok);
                    msgBox.setDefaultButton(QMessageBox::Ok);
                    msgBox.setIcon(QMessageBox::Warning);
                    int ret = msgBox.exec();
                }
                else
                {
                    selectAll();
                    insert(name);
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
