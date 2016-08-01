#include "UtilsQt.h"

#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QStringList>

#include <QDebug>

void CopyFolder(const QString &a_sFolderPath, const QString &a_sNewFolderPath)
{
    QDirIterator it(a_sFolderPath, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        QString currentFile = it.next();

        // Create new path
        QString newFile = currentFile;
        QString fileRelativePath = newFile.remove(a_sFolderPath + "/");
        QString newFileAbsPath =  a_sNewFolderPath + "/" + fileRelativePath;

        // Create folder for target path
        QFileInfo newFileInfo(newFileAbsPath);
        QString fileName = newFileInfo.fileName();
        newFileInfo.setFile(newFileInfo.absolutePath());

        QString newFilePath = newFileInfo.absoluteFilePath();
        if(!newFileInfo.exists())
        {
            qDebug("folder inexsitant");
            QDir targetFolder(a_sNewFolderPath);
            if(targetFolder.mkpath(newFilePath))
            {
                qDebug("creating path failed");
            }
        }

        // Copying file
        if(QFile::copy(currentFile,newFileAbsPath))
        {
                qDebug("bite");
        }
    }
}
