#ifndef CZIPCOMPRESSOR_H
#define CZIPCOMPRESSOR_H

#include <string>
#include <QObject>
#include <QProcess>
#include <QProgressDialog>
#include <QDir>

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

    void ExtractArchive(const std::string& a_rArchive);

private:
    int CountItemsInArchive(QDir a_rFolder);

public slots:
    void ArchiveCompleted(int a_iExitCode, QProcess::ExitStatus a_oExitStatus);
    void ProcessReadyRead();
};

#endif // CZIPCOMPRESSOR_H