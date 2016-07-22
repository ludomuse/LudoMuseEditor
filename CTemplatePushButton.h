#ifndef CTEMPLATEPUSHBUTTON_H
#define CTEMPLATEPUSHBUTTON_H

#include <QPushButton>

#include "CTemplate.h"

class CTemplatePushButton : public QPushButton
{
    Q_OBJECT
public:
    CTemplatePushButton(CTemplate* a_pTemplate, QWidget* a_pParent = Q_NULLPTR);

private:
    CTemplate* m_pTemplate;

public slots:
    void onClick();
signals:
    void newTemplateSelected(CTemplate* a_pTemplate);

};

#endif // CTEMPLATEPUSHBUTTON_H
