#ifndef CWIZARDFACTORY_H
#define CWIZARDFACTORY_H

#include <QDialog>
#include "SNewGame.h"


namespace LM
{
class CKernel;
}


class CWizardFactory
{
private:
    CWizardFactory();
    CWizardFactory(const CWizardFactory&);
    CWizardFactory& operator=(const CWizardFactory& a_rWizardFactory);


public:
    static CWizardFactory* Instance();
    QDialog* create(const std::string& a_sWizardName, QWidget* parent, const SNewGameInfo& a_rNewGame, LM::CKernel* a_pKernel);

};


#endif // CWIZARDFACTORY_H
