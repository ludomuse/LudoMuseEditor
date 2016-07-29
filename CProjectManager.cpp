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

void CProjectManager::SetProjectPath(const QString &a_sPath)
{
    m_sProjectPath = a_sPath;
}

QString CProjectManager::QGetProjectPath()
{
    return m_sProjectPath;
}

QString CProjectManager::QGetInstallPath()
{
    return m_sInstallPath;
}

std::string CProjectManager::GetProjectPath()
{
    return m_sProjectPath.toStdString();
}


std::string CProjectManager::GetInstallPath()
{
    return m_sInstallPath.toStdString();
}
