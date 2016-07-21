#include "CTemplate.h"

CTemplate::CTemplate(QString a_sPath, bool a_bIsGame) :
    m_sPath(a_sPath),
    m_bIsGame(a_bIsGame)
{

}

bool CTemplate::isGame()
{
    return this->m_bIsGame;
}

const QString& CTemplate::GetPath()
{
    return this->m_sPath;
}
