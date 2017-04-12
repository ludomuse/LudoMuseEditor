#ifndef CTEMPLATEMANAGER_H
#define CTEMPLATEMANAGER_H

#include <QVector>
#include <QMap>

#include "CTemplate.h"


class CTemplateManager
{
public:
    static CTemplateManager* Instance();

private:
    // Constructors
    CTemplateManager();
    CTemplateManager(const CTemplateManager& a_rCTemplateManager);
    CTemplateManager& operator=(const CTemplateManager& a_rCTemplateManager);
    // Functions
    /// \brief build CTemplate object from file path
    CTemplate* BuildTemplateFromFile(const QString& a_sPath);
    // Attributs
    //QVector<CTemplate*> m_vTemplates;
    QMap<QString, QVector<CTemplate*>> m_vTemplatesMap;
    QString m_sTemplateFolder;

public:
    ~CTemplateManager();
    const QVector<CTemplate*>& GetTemplates(const QString& a_sTemplatesSubfolder);
};

#endif // CTEMPLATEMANAGER_H
