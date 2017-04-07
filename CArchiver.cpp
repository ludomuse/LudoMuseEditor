#include "CArchiver.h"
//#include <bzlib.h>
//#include <fstream>
#include <QDebug>
#include <QFileDialog>
#include <QProgressDialog>
#include <quazip.h>
#include <quazipfile.h>

CArchiver::CArchiver(QWidget *parent) : m_pParent(parent),
    m_pProcess(new QProcess(parent)),
    m_pDialog(nullptr)
{
}

void CArchiver::CompressFolder(const std::string &a_rFolder, const std::string& a_rDestination)
{

    QDir compressedFolder(a_rFolder.c_str());
    int nbItemsInArchive = CountItemsInArchive(compressedFolder);
    std::string folderName = compressedFolder.dirName().toStdString();
    compressedFolder.cdUp();

//    connect(m_pProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(ArchiveCompleted(int, QProcess::ExitStatus)));
//    connect(m_pProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(ProcessReadyRead()));

//    m_pProcess->setProcessChannelMode(QProcess::MergedChannels);

    if (m_pDialog == nullptr)
    {
        m_pDialog = new QProgressDialog(m_pParent);
        m_pDialog->setLabelText("Création de l'archive ...");
        m_pDialog->setWindowModality(Qt::WindowModal);
        m_pDialog->setCancelButton(nullptr);
    }

    m_pDialog->setValue(0);
    m_pDialog->setRange(0, nbItemsInArchive);
    m_pDialog->show();

//    m_pProcess->start((std::string("tar -cvjf ") +
//                  a_rDestination + "/" + folderName + ".lm -C " +
//                  compressedFolder.absolutePath().toStdString() + " " + folderName).c_str());


    QuaZip zip(QString::fromStdString(a_rDestination + "/" + folderName + ".lm"));
    zip.open(QuaZip::mdCreate);
    WriteFolderToZip(zip, compressedFolder, QString::fromStdString(folderName));
    zip.close();

    //    m_pDialog->deleteLater();
    //    m_pDialog = nullptr;

    // WARNING memory leak on purpose otherwise the app crashes
    //delete zip;
}


void CArchiver::ExtractArchive(const std::string &a_rArchive)
{
    // tar xjf a_rfolder.lm
    system(std::string(std::string("tar xjf ") + a_rArchive + ".lm").c_str());
}





int CArchiver::CountItemsInArchive(QDir& a_rFolder)
{
    int nbItem = 0;
    a_rFolder.setFilter(QDir::Files);
    nbItem += a_rFolder.count(); // - 2; // -2 because of . and .. directories
    a_rFolder.setFilter(QDir::Dirs);
    QStringList subfolders = a_rFolder.entryList();
    for (QString subfolder : subfolders)
    {
        if (subfolder != "." && subfolder != "..")
        {
            QDir subdir(a_rFolder);
            subdir.cd(subfolder);
            nbItem += CountItemsInArchive(subdir);
        }
    }
    return nbItem;
}


void CArchiver::WriteFolderToZip(QuaZip& a_rZipfile, QDir& a_rParentFolder, const QString& a_rFolderName)
{
    QDir folderToAdd(a_rParentFolder);
    folderToAdd.cd(a_rFolderName);
    folderToAdd.setFilter(QDir::Files);
    QStringList subfiles = folderToAdd.entryList();
    for (QString file : subfiles)
    {
        QuaZipFile fileInZip(&a_rZipfile);
        fileInZip.open(QuaZipFile::WriteOnly, QuaZipNewInfo(a_rFolderName + "/" + file));

        QDataStream ds(&fileInZip);

        QFile fileToAdd(a_rParentFolder.absoluteFilePath(a_rFolderName + "/" + file));
        fileToAdd.open(QIODevice::ReadOnly);
        QByteArray dataToWrite = fileToAdd.readAll();
        ds.writeRawData(dataToWrite.data(), dataToWrite.size());

        qDebug() << "writing file " << a_rFolderName + "/" + file << " to archive";

        fileInZip.close();
        fileToAdd.close();

        m_pDialog->setValue(m_pDialog->value() + 1);
    }

    folderToAdd.setFilter(QDir::Dirs);
    QStringList subfolders = folderToAdd.entryList();
    for (QString subfolder : subfolders)
    {
        if (subfolder != "." && subfolder != "..")
        {
            WriteFolderToZip(a_rZipfile, a_rParentFolder, a_rFolderName + "/" + subfolder);
        }
    }
}


//////////////////////// SLOTS


void CArchiver::ArchiveCompleted(int a_iExitCode, QProcess::ExitStatus a_oExitStatus)
{
    qDebug() << "Archive completed with exit code : " << a_iExitCode;
    m_pDialog->deleteLater();
    m_pDialog = nullptr;
}


void CArchiver::ProcessReadyRead()
{
    char buf[1024];
    while (int bytesRead = m_pProcess->readLine(buf, sizeof(buf)))
    {
        if (bytesRead > 1)
        {
            m_pDialog->setValue(m_pDialog->value() + 1);
        }
    }
}
