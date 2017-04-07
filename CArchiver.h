#ifndef CZIPCOMPRESSOR_H
#define CZIPCOMPRESSOR_H

#include <string>
#include <QObject>
#include <QProcess>
#include <QProgressDialog>
#include <QDir>
#include <quazip.h>

class CArchiver : public QObject
{
    Q_OBJECT
private:
    QWidget* m_pParent;
    QProcess* m_pProcess;
    QProgressDialog* m_pDialog;


public:
    CArchiver(QWidget* parent);
    void CompressFolder(const std::string& a_rFolder, const std::string& a_rDestination);

    void ExtractArchive(const QString& a_rArchivePath, const QString& a_rDestination);

private:
    int CountItemsInArchive(QDir& a_rFolder);
    void WriteFolderToZip(QuaZip& a_rZipfile, QDir& a_rParentFolder, const QString& a_rFolderName);

public slots:
    void ArchiveCompleted(int a_iExitCode, QProcess::ExitStatus a_oExitStatus);
    void ProcessReadyRead();
};

#endif // CZIPCOMPRESSOR_H
