#include "CTemplatePushButton.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>

CTemplatePushButton::CTemplatePushButton(CTemplate* a_pTemplate, QWidget* a_pParent):
    QPushButton(a_pParent),
    m_pTemplate(a_pTemplate)
{
    // Set background image
    QPixmap scaled = m_pTemplate->GetImage().scaledToWidth(250, Qt::FastTransformation);
    QLabel *label = new QLabel(this);
    label->setPixmap(scaled);
    QVBoxLayout* vPushButtonLayout = new QVBoxLayout(this);
    vPushButtonLayout->addWidget(label);
    vPushButtonLayout->setContentsMargins(2, 2, 2, 2);
    vPushButtonLayout->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);

    this->setLayout(vPushButtonLayout);
    this->setMinimumHeight(150);
    this->setMaximumWidth(260);
    this->setStyleSheet("QPushButton { border : 1px solid rgb(175, 175, 175) } QPushButton:hover{ border : 1px solid rgb(0, 85, 255)}");

    if(m_pTemplate->HasDescription())
    {
        this->setToolTip(m_pTemplate->GetDescription());
    }

    connect(this, SIGNAL(clicked(bool)), this, SLOT(onClick()));
}

void CTemplatePushButton::Unfocus()
{
    this->setStyleSheet("QPushButton { border : 1px solid rgb(175, 175, 175) } QPushButton:hover{ border : 1px solid rgb(0, 85, 255)}");
}

CTemplate* CTemplatePushButton::GetTemplate()
{
    return this->m_pTemplate;
}

void CTemplatePushButton::onClick()
{
    this->setStyleSheet("QPushButton{border : 2px solid rgb(0, 85, 255)}");
    emit newTemplateSelected(this);
}
