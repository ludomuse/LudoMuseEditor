#ifndef CPROJECTMANAGER_H
#define CPROJECTMANAGER_H

#include <QString>
#include <QFile>

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
    void ReadLastProject(QFile& a_fConfigFile);
    void EditPrevFile();
    std::size_t GetProjectHash();
    // Attributs
    QString m_sProjectPath;
    QString m_sInstallPath;
    std::vector<std::string*> m_vSources;
    std::vector<std::string> m_vPreviousProjectPaths;


public:
    ~CProjectManager();
    void SetProjectFile(const QString& a_sPath);
    QString QGetProjectPath();
    QString QGetInstallPath();
    QString QGetAbsoluteWritablePath();
    QString QGetProjectName();
    QString QGetProjectJsonFile();
    std::string GetProjectPath();
    std::string GetInstallPath();
    std::string GetAbsoluteWritablePath();
    std::string GetRelativeWritablePath();
    std::string GetProjectName();
    std::string GetProjectJsonFile();
    /// \brief add the new source if necessary and return the index
    std::string* PushBackSource(const std::string& a_sSource);
    const std::string* GetSourceAt(int a_iIndex);
    const std::vector<std::string>& GetPreviousProjectPaths();
    /// \brief update last_project propertie of config.json
    void UpdateLastProject(const QString& a_sNewProject);
};

#endif // CPROJECTMANAGER_H
