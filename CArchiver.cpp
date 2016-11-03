#include "CArchiver.h"
//#include <bzlib.h>
//#include <fstream>
#include <QDebug>
#include <QFileDialog>
#include <QProgressDialog>


CArchiver::CArchiver(QWidget *parent) : m_pParent(parent),
    m_pProcess(new QProcess(parent)),
    m_pDialog(nullptr)
{
}

void CArchiver::CompressFolder(const std::string &a_rFolder, const std::string& a_rDestination)
{

    // TODO : add error handling
    if (m_pDialog != nullptr)
        return;

    QDir compressedFolder(a_rFolder.c_str());
    int nbItemsInArchive = CountItemsInArchive(compressedFolder);
    std::string folderName = compressedFolder.dirName().toStdString();
    compressedFolder.cdUp();

    connect(m_pProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(ArchiveCompleted(int, QProcess::ExitStatus)));
    connect(m_pProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(ProcessReadyRead()));

    m_pProcess->setProcessChannelMode(QProcess::MergedChannels);

    m_pDialog = new QProgressDialog(m_pParent);
    m_pDialog->setLabelText("Création de l'archive ...");
    m_pDialog->setWindowModality(Qt::WindowModal);
    m_pDialog->setCancelButton(nullptr);

    m_pDialog->setValue(0);
    m_pDialog->setRange(0, nbItemsInArchive);
    m_pDialog->show();

    m_pProcess->start((std::string("tar -cvjf ") +
                  a_rDestination + "/" + folderName + ".lm -C " +
                  compressedFolder.absolutePath().toStdString() + " " + folderName).c_str());

}


void CArchiver::ExtractArchive(const std::__1::string &a_rArchive)
{
    // tar xjf a_rfolder.lm
    system(std::string(std::string("tar xjf ") + a_rArchive + ".lm").c_str());
}





int CArchiver::CountItemsInArchive(QDir a_rFolder)
{
    int nbItem = 0;
    a_rFolder.setFilter(QDir::Files|QDir::Dirs);
    nbItem += a_rFolder.count() - 2; // -2 because of . and .. directories
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
