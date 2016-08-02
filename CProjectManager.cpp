#include "CProjectManager.h"

#include <QDir>

CProjectManager::CProjectManager():
    m_sProjectPath("")
{
    m_sInstallPath = QDir::currentPath();
}

CProjectManager::CProjectManager(const CProjectManager &a_oProjectManager)
{

}

CProjectManager& CProjectManager::operator=(const CProjectManager& a_oProjecManager)
{
    return *this;
}

CProjectManager::~CProjectManager()
{

}

CProjectManager* CProjectManager::Instance()
{
    static CProjectManager instance;
    return &instance;
}

void CProjectManager::SetProjectFile(const QString &a_sPath)
{
    m_sProjectPath = a_sPath;
}

QString CProjectManager::QGetProjectPath()
{
    QFileInfo temp(m_sProjectPath);
    return temp.absolutePath() + "/";
}

QString CProjectManager::QGetInstallPath()
{
    return m_sInstallPath;
}

QString CProjectManager::QGetProjectName()
{
    QFileInfo temp(m_sProjectPath);
    return temp.baseName();
}

std::string CProjectManager::GetProjectPath()
{
    QFileInfo temp(m_sProjectPath);
    return temp.absolutePath().toStdString() + "/";
}

std::string CProjectManager::GetInstallPath()
{
    return m_sInstallPath.toStdString();
}

std::string CProjectManager::GetProjectName()
{
    QFileInfo temp(m_sInstallPath);
    return temp.baseName().toStdString();
}

std::vector<std::string>::iterator CProjectManager::PushBackSource(const std::string& a_sSource)
{
    std::vector<std::string>::iterator it = std::find(m_vSources.begin(), m_vSources.end(), a_sSource);
    if(it != m_vSources.end())
    {
        return it;
    }
    else
    {

        return m_vSources.insert(m_vSources.end(), a_sSource);
    }
}

const std::string& CProjectManager::GetSourceAt(int a_iIndex)
{
    return m_vSources.at(a_iIndex);
}

