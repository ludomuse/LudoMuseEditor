#include "CWizard.h"

CWizard::CWizard(QWidget *parent):QDialog(parent)
{

}

void CWizard::SetParameters(const SNewGameInfo& a_rNewGame, LM::CKernel* a_pKernel, int a_iChapterNumber){
    m_oNewGameInfo = a_rNewGame;
    m_pKernel = a_pKernel;
    m_iCurrentChapter = a_iChapterNumber;
}

void CWizard::clickOnCancel(bool){
    close();
}
void CWizard::clickOnValidate(bool){
    this->WizardValidated();
}
