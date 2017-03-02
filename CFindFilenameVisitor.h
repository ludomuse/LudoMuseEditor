#include "Classes/Engine/Include/CVisitor.h"

#include <QObject>
#include <QString>
#include <QStringList>

#include "Classes/Engine/Include/CNode.h"

class CFindFilenameVisitor : public QObject, public LM::CVisitor
{
    Q_OBJECT
public:
    CFindFilenameVisitor(const QString& a_sFilename);
    QStringList* GetScenesID();

protected:
    QString m_sFilename;
    QStringList* m_vSceneList;

    virtual LM::Result ProcessNodeTopDown(LM::CNode* a_pNode) override;
};
