#ifndef CWIZARDFACTORY_H
#define CWIZARDFACTORY_H

#include <QDialog>

class CWizardFactory
{
private:
    CWizardFactory();
    CWizardFactory(const CWizardFactory&);
    CWizardFactory& operator=(const CWizardFactory& a_rWizardFactory);


public:
    static CWizardFactory* Instance();
    QDialog* create(const std::string& a_sWizardName, QWidget* parent);

};

#endif // CWIZARDFACTORY_H
