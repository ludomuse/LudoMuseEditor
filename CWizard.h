#ifndef CWIZARD_H
#define CWIZARD_H

#include <QDialog>

#include "SNewGame.h"

namespace LM
{
class CKernel;
}

class CWizard : public QDialog
{
    Q_OBJECT
protected:
    SNewGameInfo m_oNewGameInfo;
    LM::CKernel* m_pKernel;
    int m_iCurrentChapter;

    virtual void WizardValidated() = 0;

public:
    CWizard(QWidget* parent);
    void SetParameters(const SNewGameInfo& a_rNewGame, LM::CKernel* a_pKernel, int a_iChapterNumber = 0);
public slots:
    void clickOnCancel(bool);
    void clickOnValidate(bool);
};



#endif // CWIZARD_H
