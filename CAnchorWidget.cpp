#include "CAnchorWidget.h"

#include <QWidget>
#include <QButtonGroup>
#include <QPushButton>
#include <QGridLayout>

#include <string>

CAnchorWidget::CAnchorWidget(int a_iFirstAnchor, QWidget* a_pParent):
    QWidget(a_pParent)
{
    QButtonGroup* buttonsGroup = new QButtonGroup(this);
    buttonsGroup->setExclusive(true);
    QGridLayout* buttonsLayout = new QGridLayout(this);
    QString sStyleSheet = QString("QPushButton{background-color : rgb(200, 200, 200);}")+
//                          QString("QPushButton:checked{background-color : rgb(255, 155, 55);}")+
                          QString("QPushButton:flat{border : none;}")+
                          QString("QPushButton:hover{background-color : rgb(105, 155, 255);}");
    this->setStyleSheet(sStyleSheet);
    QPushButton* anchor1Button = new QPushButton("1", this);
    anchor1Button->setCheckable(true);
    QPushButton* anchor2Button = new QPushButton("2", this);
    anchor2Button->setCheckable(true);
    QPushButton* anchor3Button = new QPushButton("3", this);
    anchor3Button->setCheckable(true);
    QPushButton* anchor8Button = new QPushButton("8", this);
    anchor8Button->setCheckable(true);
    QPushButton* anchor0Button = new QPushButton("0", this);
    anchor0Button->setCheckable(true);
    QPushButton* anchor4Button = new QPushButton("4", this);
    anchor4Button->setCheckable(true);
    QPushButton* anchor7Button = new QPushButton("7", this);
    anchor7Button->setCheckable(true);
    QPushButton* anchor6Button = new QPushButton("6", this);
    anchor6Button->setCheckable(true);
    QPushButton* anchor5Button = new QPushButton("5", this);
    anchor5Button->setCheckable(true);
    buttonsLayout->addWidget(anchor1Button, 0, 0);
    buttonsGroup->addButton(anchor1Button, 1);
    buttonsLayout->addWidget(anchor2Button, 0, 1);
    buttonsGroup->addButton(anchor2Button, 2);
    buttonsLayout->addWidget(anchor3Button, 0, 2);
    buttonsGroup->addButton(anchor3Button, 3);
    buttonsLayout->addWidget(anchor8Button, 1, 0);
    buttonsGroup->addButton(anchor8Button, 8);
    buttonsLayout->addWidget(anchor0Button, 1, 1);
    buttonsGroup->addButton(anchor0Button, 0);
    buttonsLayout->addWidget(anchor4Button, 1, 2);
    buttonsGroup->addButton(anchor4Button, 4);
    buttonsLayout->addWidget(anchor7Button, 2, 0);
    buttonsGroup->addButton(anchor7Button, 7);
    buttonsLayout->addWidget(anchor6Button, 2, 1);
    buttonsGroup->addButton(anchor6Button, 6);
    buttonsLayout->addWidget(anchor5Button, 2, 2);
    buttonsGroup->addButton(anchor5Button, 5);
    QPalette widgetPal(palette());
    widgetPal.setColor(QPalette::Background, QColor(150, 150, 150));
    this->setLayout(buttonsLayout);
    this->setAutoFillBackground(true);
    this->setPalette(widgetPal);

    buttonsGroup->button(a_iFirstAnchor)->toggle();
    connect(buttonsGroup, SIGNAL(buttonClicked(int)), this, SLOT(changeAnchor(int)));
}

void CAnchorWidget::changeAnchor(int a_iAnchorID) {
    emit anchorChanged(a_iAnchorID);
}

