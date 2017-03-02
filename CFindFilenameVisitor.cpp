#include "CFindFilenameVisitor.h"

#include "Classes/Engine/Include/CEntityNode.h"

using namespace LM;

CFindFilenameVisitor::CFindFilenameVisitor(const QString& a_sFilename):
    QObject(),
    CVisitor(),
    m_sFilename(a_sFilename),
    m_vSceneList(new QStringList())
{

}

Result CFindFilenameVisitor::ProcessNodeTopDown(CNode *a_pNode)
{
//    CEntityNode* pEntity = dynamic_cast<CEntityNode*> (a_pNode);
//    if (pEntity && pEntity->UseFile(m_sFilename.toStdString()))
//    {
//        m_vSceneList->append(QString::fromStdString(pEntity->GetSceneID()));
//        return RESULT_PRUNE;
//    }
    if (a_pNode->UseFile(m_sFilename.toStdString()))
    {
        m_vSceneList->append(QString::fromStdString(a_pNode->GetSceneID()));
        return RESULT_PRUNE;
    }
    return RESULT_CONTINUE;
}

QStringList* CFindFilenameVisitor::GetScenesID()
{
    return m_vSceneList;
}

