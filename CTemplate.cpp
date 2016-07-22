#include "CTemplate.h"

#include <QFile>

CTemplate::CTemplate(QString a_sPath, QString a_sPicturePath,
                     bool a_bIsGame, QString a_sName, QString a_sDescription) :
    m_sPath(a_sPath),
    m_bIsGame(a_bIsGame),
    m_sName(a_sName),
    m_sDescription(a_sDescription)
{
    QFile pictureFile(a_sPicturePath);
    if(!pictureFile.exists())
    {
        qDebug("cannot load image file");
    }
    else
    {
        m_imPicture = QPixmap(a_sPicturePath);
    }
}

bool CTemplate::IsGame()
{
    return this->m_bIsGame;
}

bool CTemplate::HasDescription()
{
    return !m_sDescription.isEmpty();
}

bool CTemplate::HasName()
{
    return !m_sName.isEmpty();
}

const QString& CTemplate::GetName()
{
    return m_sName;
}

const QString& CTemplate::GetDescription()
{
    return m_sDescription;
}

const QString& CTemplate::GetPath()
{
    return this->m_sPath;
}

const QPixmap& CTemplate::GetImage()
{
    return this->m_imPicture;
}
