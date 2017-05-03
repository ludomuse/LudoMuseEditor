#include "CWizardFactory.h"
#include "CPhotoPuzzleWizard.h"

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

QDialog* CWizardFactory::create(const std::string &a_sWizardName, QWidget* parent)
{
    if (a_sWizardName == "PhotoPuzzle")
    {
        return new CPhotoPuzzleWizard(parent);
    }
    else
    {
        return nullptr;
    }
}
