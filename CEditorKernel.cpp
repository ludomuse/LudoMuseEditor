#include "CEditorKernel.h"

CEditorKernel::CEditorKernel()
{

}

CEditorKernel::CEditorKernel(const CEditorKernel &a_oEditorKernel)
{

}

CEditorKernel& CEditorKernel::operator=(const CEditorKernel& a_oEditorKernel)
{
    return *this;
}

CEditorKernel::~CEditorKernel()
{

}

void CEditorKernel::sendMenuNode(LM::CMenuNode *a_pMenuNodeTouched)
{
    emit sendMenuNodeSignal(a_pMenuNodeTouched);
}

CEditorKernel* CEditorKernel::Instance()
{
    static CEditorKernel instance;
    return &instance;
}
