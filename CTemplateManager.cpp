#include "CTemplateManager.h"
#include "CProjectManager.h"

#include "rapidjson.h"
#include "document.h"

#include <QFile>
#include <QTextStream>
#include <QDirIterator>

#include <QDebug>


CTemplateManager::CTemplateManager()
{
    this->m_sTemplateFolder = CProjectManager::Instance()->QGetInstallPath() + "/templates/";
    QDirIterator itSubdir(m_sTemplateFolder, QStringList() << "*");
    m_vTemplatesMap.insert(itSubdir.fileName(), QVector<CTemplate*>());
    while (itSubdir.hasNext())
    {
        QDirIterator itJson(itSubdir.next(), QStringList() << "*.json", QDir::Files, QDirIterator::Subdirectories);
        while (itJson.hasNext())
        {
            CTemplate* temp = this->BuildTemplateFromFile(itJson.next());
            if(temp != Q_NULLPTR)
            {
                this->m_vTemplatesMap[itSubdir.fileName()].push_back(temp);
            }
        }
    }
}

CTemplateManager::CTemplateManager(const CTemplateManager& a_rCTemplateManager)
{

}

CTemplateManager& CTemplateManager::operator=(const CTemplateManager& a_rCTemplateManager)
{
    return *this;
}

CTemplateManager::~CTemplateManager()
{

}

CTemplateManager* CTemplateManager::Instance()
{
    static CTemplateManager instance;
    return &instance;
}

const QVector<CTemplate*>& CTemplateManager::GetTemplates(const QString& a_sTemplatesSubfolder)
{
    return m_vTemplatesMap[a_sTemplatesSubfolder];
}


CTemplate* CTemplateManager::BuildTemplateFromFile(const QString &a_sPath)
{
    QFile templateFile(a_sPath);
    QString fileContent;
    if(!templateFile.exists())
    {
        qDebug("template doesn't file exist");
        return Q_NULLPTR;
    }

    if (!templateFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug("Cannot open file");
        return Q_NULLPTR;
    }
    else
    {
        fileContent = QString(templateFile.readAll());
    }
    rapidjson::Document doc;
    doc.Parse(fileContent.toStdString().c_str());
    QString templatePicturePath, templatePicturePath2, templateName, templateDescription, wizardName;
    bool templateIsGame = false;

    if(doc.HasMember("game"))
    {
        templateIsGame = doc["game"].GetBool();
    }
    if(doc.HasMember("pictures") && doc["pictures"].IsArray())
    {
        if(templateIsGame)
        {
            // Have 2 different screens
            templatePicturePath = m_sTemplateFolder + QString(doc["pictures"][0].GetString());
            templatePicturePath2 = m_sTemplateFolder + QString(doc["pictures"][1].GetString());
        }
        else
        {
            templatePicturePath = m_sTemplateFolder + QString(doc["pictures"][0].GetString());
            templatePicturePath2 = "";
        }
    }
    else
    {
        qDebug("No picture properties found in template");
        return Q_NULLPTR;
    }

    if(doc.HasMember("name"))
    {
        templateName = QString(doc["name"].GetString());
    }
    if(doc.HasMember("description"))
    {
        templateDescription = QString(doc["description"].GetString());
    }
    if (doc.HasMember(("wizard")))
    {
        wizardName = QString(doc["wizard"].GetString());
    }
    CTemplate* returnTemplate = new CTemplate(a_sPath, templatePicturePath, templateIsGame, templateName,templateDescription, templatePicturePath2, wizardName);

    return returnTemplate;
}
