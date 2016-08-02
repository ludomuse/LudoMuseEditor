#include "CProjectManager.h"

#include <rapidjson.h>
#include <document.h>
#include <stringbuffer.h>
#include <writer.h>

#include <QDir>
#include <QDebug>

CProjectManager::CProjectManager():
    m_sProjectPath("")
{
    m_sInstallPath = QDir::currentPath();
    QFile configFile(m_sInstallPath + "/debug/prev.json");
    this->ReadLastProject(configFile);
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

void CProjectManager::ReadLastProject(QFile& a_fConfigFile)
{
    if(!a_fConfigFile.exists())
    {
        return;
    }

    QString fileContent;
    if (!a_fConfigFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug("Cannot open file");
        return;
    }
    else
    {
        fileContent = QString(a_fConfigFile.readAll());
    }

    rapidjson::Document doc;
    doc.Parse(fileContent.toStdString().c_str());
    if(doc.HasMember("last_projects") && doc["last_projects"].IsArray())
    {
        int size = doc["last_projects"].Size();
        rapidjson::Value& lastProjectArray = doc["last_projects"];
        for(int i = 0; i < size; i++)
        {
            if(lastProjectArray[i].HasMember("path"))
            {
                std::string currentValue = lastProjectArray[i]["path"].GetString();
                if(!currentValue.empty())
                {
                    m_vPreviousProjectPaths.push_back(currentValue);
                }
            }
        }
    }
}

void CProjectManager::EditPrevFile()
{
    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer(s);

    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

    rapidjson::Value last_projects(rapidjson::kArrayType);
    rapidjson::Value proj0(rapidjson::kObjectType);
    std::string temp = m_vPreviousProjectPaths.at(0);
    temp = m_vPreviousProjectPaths.at(1);
    temp = m_vPreviousProjectPaths.at(2);
    proj0.AddMember("path",
                    rapidjson::Value(m_vPreviousProjectPaths.at(0).c_str(), m_vPreviousProjectPaths.at(0).length()),
                    allocator);
    rapidjson::Value proj1(rapidjson::kObjectType);
    proj1.AddMember("path",
                    rapidjson::Value(m_vPreviousProjectPaths.at(1).c_str(), m_vPreviousProjectPaths.at(1).length()),
                    allocator);
    rapidjson::Value proj2(rapidjson::kObjectType);
    proj2.AddMember("path",
                    rapidjson::Value(m_vPreviousProjectPaths.at(2).c_str(), m_vPreviousProjectPaths.at(2).length()),
                    allocator);
    last_projects.PushBack(proj0, allocator);
    last_projects.PushBack(proj1, allocator);
    last_projects.PushBack(proj2, allocator);

    document.AddMember("last_projects", last_projects, allocator);

    document.Accept(writer);
    qDebug()<< s.GetString();

    QFile file( m_sInstallPath + "/debug/prev.json" );
    if(!file.exists())
    {
        return;
    }
    QString jsonResult;
    if ( file.open(QIODevice::ReadWrite | QFile::Truncate) )
    {
        QTextStream stream( &file );
        stream.setCodec("UTF-8");
        jsonResult = QString::fromUtf8(s.GetString());
        stream << jsonResult;
        stream.flush();
        file.close();
    }
}

CProjectManager* CProjectManager::Instance()
{
    static CProjectManager instance;
    return &instance;
}

void CProjectManager::SetProjectFile(const QString &a_sPath)
{
    m_sProjectPath = a_sPath;
    this->UpdateLastProject(m_sProjectPath);
    this->EditPrevFile();
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

const std::vector<std::string>& CProjectManager::GetPreviousProjectPaths()
{
    return m_vPreviousProjectPaths;
}

void CProjectManager::UpdateLastProject(const QString &a_sNewProject)
{
    std::vector<std::string>::iterator it = find(m_vPreviousProjectPaths.begin(), m_vPreviousProjectPaths.end(), a_sNewProject.toStdString());
    if(it != m_vPreviousProjectPaths.end())
    {
        // Moving path to first position in vector
        std::vector<std::string> tempVector;
        int count = 0;
        for(std::string currentString : m_vPreviousProjectPaths)
        {
            if(currentString == a_sNewProject.toStdString())
            {
                tempVector.insert(tempVector.begin(), currentString);
            }
            else
            {
                tempVector.push_back(currentString);
            }

            if(++count == 3)
            {
                break;
            }
        }
        m_vPreviousProjectPaths = tempVector;
    }
    else
    {
        // Adding path to first posititon in vector
        m_vPreviousProjectPaths.insert(m_vPreviousProjectPaths.begin(), a_sNewProject.toStdString());
        // limite size to 3
        if(m_vPreviousProjectPaths.size() > 3)
        {
            m_vPreviousProjectPaths.erase(m_vPreviousProjectPaths.begin() + 3, m_vPreviousProjectPaths.end());
        }
    }
}
