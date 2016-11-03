#ifndef CZIPCOMPRESSOR_H
#define CZIPCOMPRESSOR_H

#include <string>
#include <QObject>
#include <QProcess>
#include <QProgressDialog>

class CArchiver : public QObject
{
    Q_OBJECT
private:
    QProcess* m_pProcess;
    QProgressDialog* m_pDialog;
    QWidget* m_pParent;


public:
    CArchiver(QWidget* parent);
    void CompressFolder(const std::string& a_rFolder, const std::string& a_rDestination);

    void ExtractArchive(const std::string& a_rArchive);

public slots:
    void ArchiveCompleted(int a_iExitCode, QProcess::ExitStatus a_oExitStatus);
};

#endif // CZIPCOMPRESSOR_H
