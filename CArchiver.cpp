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
    std::string folderName = compressedFolder.dirName().toStdString();
    compressedFolder.cdUp();

    connect(m_pProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(ArchiveCompleted(int, QProcess::ExitStatus)));

    m_pDialog = new QProgressDialog(m_pParent);
    m_pDialog->setLabelText("Création de l'archive ...");
    m_pDialog->setWindowModality(Qt::WindowModal);
    m_pDialog->setRange(0, 0);
    m_pDialog->setCancelButton(nullptr);
    m_pDialog->show();

    m_pProcess->start((std::string("tar -cjf ") +
                  a_rDestination + "/" + folderName + ".lm -C " +
                  compressedFolder.absolutePath().toStdString() + " " + folderName).c_str());


}


void CArchiver::ExtractArchive(const std::__1::string &a_rArchive)
{
    // tar xjf a_rfolder.lm
    system(std::string(std::string("tar xjf ") + a_rArchive + ".lm").c_str());
}


void CArchiver::ArchiveCompleted(int a_iExitCode, QProcess::ExitStatus a_oExitStatus)
{
    qDebug() << "Archive completed with exit code : " << a_iExitCode;
    m_pDialog->deleteLater();
    m_pDialog = nullptr;
}
