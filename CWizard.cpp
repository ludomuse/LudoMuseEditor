#include "CWizard.h"

CWizard::CWizard(QWidget *parent):QDialog(parent)
{

}

void SetParameters(CWizard* a_rWiz,const SNewGameInfo& a_rNewGame, LM::CKernel* a_pKernel, int a_iChapterNumber){
    a_rWiz->m_oNewGameInfo = a_rNewGame;
    a_rWiz->m_pKernel = a_pKernel;
    a_rWiz->m_iCurrentChapter = a_iChapterNumber;
}

void CWizard::clickOnCancel(bool){
    close();
}
void CWizard::clickOnValidate(bool){
    this->WizardValidated();
}
