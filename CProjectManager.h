#ifndef CPROJECTMANAGER_H
#define CPROJECTMANAGER_H

#include <QString>

#include <string>

class CProjectManager
{
public:
    static CProjectManager* Instance();

private:
    // Constructor
    CProjectManager();
    CProjectManager(const CProjectManager& a_oProjectManager);
    CProjectManager& operator=(const CProjectManager& a_oProjectManager);
    // Function
    // Attributs
    QString m_sProjectPath;
    QString m_sInstallPath;


public:
    ~CProjectManager();
    void SetProjectPath(const QString& a_sPath);
    QString QGetProjectPath();
    QString QGetInstallPath();
    std::string GetProjectPath();
    std::string GetInstallPath();
};

#endif // CPROJECTMANAGER_H
