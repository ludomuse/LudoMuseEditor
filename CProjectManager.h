#ifndef CPROJECTMANAGER_H
#define CPROJECTMANAGER_H

#include <QString>

#include <string>
#include <vector>

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
    std::vector<std::string> m_vSources;


public:
    ~CProjectManager();
    void SetProjectFile(const QString& a_sPath);
    QString QGetProjectPath();
    QString QGetInstallPath();
    QString QGetProjectName();
    std::string GetProjectPath();
    std::string GetInstallPath();
    std::string GetProjectName();
    /// \brief add the new source if necessary and return the index
    std::vector<std::string>::iterator PushBackSource(const std::string& a_sSource);
    const std::string& GetSourceAt(int a_iIndex);
};

#endif // CPROJECTMANAGER_H
