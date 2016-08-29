#include "CProjectPushButton.h"

#include <QFileInfo>

CProjectPushButton::CProjectPushButton(QString a_sPath, QWidget *parent) :
    QPushButton(parent),
    m_sPath(a_sPath)
{
    QFileInfo tempFileinfo(m_sPath);
    QString ProjectName = tempFileinfo.baseName();
    this->setText(ProjectName + "\n" + m_sPath);
    this->setFlat(true);
    this->setStyleSheet("QPushButton{"
                        "background-color : rgb(65,65,65);"
                        "color : white;"
                        "text-align : left;"
                        "border : none;"
                        "}"
                        "QPushButton:hover{"
                        "background-color : rgb(75,75,75);"
                        "color : rgb(255, 170, 0);"
                        "text-align : left;"
                        "border : none;"
                        "}");
    connect(this, SIGNAL(clicked(bool)), this, SLOT(onClick(bool)));
}

CProjectPushButton::~CProjectPushButton()
{
}

void CProjectPushButton::onClick(bool)
{
    emit onClick(m_sPath);
}
