#ifndef CEDITORKERNEL_H
#define CEDITORKERNEL_H

#include <Classes/Engine/Include/CMenuNode.h>
#include <QObject>

class CEditorKernel: public QObject
{
    Q_OBJECT
public:
    ~CEditorKernel();
    static CEditorKernel* Instance();
    void sendMenuNode(LM::CMenuNode* a_pMenuNodeTouched);
    int m_iCurrentChapter = 0;

private:
    // Constructor
    CEditorKernel();
    CEditorKernel(const CEditorKernel& a_oEditorKernel);
    CEditorKernel& operator=(const CEditorKernel& a_oEditorKernel);

public slots:

signals:
    void sendMenuNodeSignal(LM::CMenuNode* a_pMenuNodeTouched);
};

#endif // CEDITORKERNEL_H
