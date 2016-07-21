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
        //qDebug() << it.next();
        CTemplate* temp = this->BuildTemplateFromFile(it.next());
        this->m_vTemplates.push_back(temp);
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

CTemplate* CTemplateManager::BuildTemplateFromFile(const QString &a_sPath)
{
    QFile templateFile(a_sPath);
    QString fileContent;
    if(templateFile.exists())
    {
        qDebug("template file exist");
    }
    if (!templateFile.open(QFile::ReadOnly | QFile::Text))
    {
        return Q_NULLPTR;
    }
    else
    {
        fileContent = QString(templateFile.readAll());
    }
    rapidjson::Document doc;
    doc.Parse(fileContent.toStdString().c_str());
    if(doc.HasMember("name"))
    {
        qDebug("NAME SWAG!");
    }
    return Q_NULLPTR;
}
