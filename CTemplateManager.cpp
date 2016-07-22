#include "CTemplateManager.h"

#include "rapidjson.h"
#include "document.h"

#include <QFile>
#include <QTextStream>
#include <QDirIterator>

#include <QDebug>


CTemplateManager::CTemplateManager()
{
    this->m_sTemplateFolder = "D:\\IHMTEK\\LudoMuseEditorCocos\\build-LudoMuseEditor-Clone_de_Desktop_Qt_5_6_0_MSVC2015_32bit-Debug\\debug\\templates";
    QDirIterator it(m_sTemplateFolder, QStringList() << "*.json", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        CTemplate* temp = this->BuildTemplateFromFile(it.next());
        if(temp != Q_NULLPTR)
        {
            this->m_vTemplates.push_back(temp);
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

const QVector<CTemplate*>& CTemplateManager::GetTemplates()
{
    return m_vTemplates;
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
    QString templatePicturePath, templateName, templateDescription;
    bool templateIsGame = false;

    if(doc.HasMember("picture"))
    {
        templatePicturePath = QString(doc["picture"].GetString());
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
    if(doc.HasMember("game"))
    {
        templateIsGame = doc["game"].GetBool();
    }
    CTemplate* returnTemplate = new CTemplate(a_sPath, templatePicturePath, templateIsGame, templateName,templateDescription);

    return returnTemplate;
}
