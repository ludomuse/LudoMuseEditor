#include "CWizardFactory.h"
#include "CPhotoPuzzleWizard.h"
#include "CQuizWizard.h"
#include "CEditorKernel.h"

CWizardFactory::CWizardFactory()
{

}

CWizardFactory::CWizardFactory(const CWizardFactory &)
{

}


CWizardFactory& CWizardFactory::operator=(const CWizardFactory& a_rWizardFactory)
{
    return *this;
}




CWizardFactory* CWizardFactory::Instance()
{
    static CWizardFactory instance;
    return &instance;
}

QDialog* CWizardFactory::create(const std::string &a_sWizardName, QWidget* parent, const SNewGameInfo& a_rNewGame, LM::CKernel* a_pKernel)
{
    CWizard* pWizard = nullptr;
    if (a_sWizardName == "PhotoPuzzle")
    {
        pWizard = new CPhotoPuzzleWizard(parent);
    }
    else if (a_sWizardName == "Quiz")
    {
        pWizard = new CQuizWizard(parent);
    }
    if(pWizard){
        pWizard->SetParameters(a_rNewGame, a_pKernel, CEditorKernel::Instance()->m_iCurrentChapter);
    }
    return pWizard;
}
