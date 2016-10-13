#include "CThumbnailWidget.h"

#include <QWidget>
#include <QBoxLayout>
#include <QPalette>
#include <QLabel>
#include <QDebug>
#include <QMouseEvent>
#include <QPoint>

CThumbnailWidget::CThumbnailWidget(QWidget *parent) : QWidget(parent),
    m_bIsHover(false),
    m_bIsPress(false)
{

}

CThumbnailWidget::CThumbnailWidget(QString a_sceneID, int a_iPlayerID, QWidget *parent, int a_iIndex) : QWidget(parent),
    m_sSceneID(a_sceneID),
    m_bIsHover(false),
    m_bIsPress(false),
    m_iPlayerID(a_iPlayerID)
{
    QVBoxLayout *vLayout = new QVBoxLayout();

//    QPixmap pic("D:/IHMTEK/LudoMuseEditorCocos/build-LudoMuseEditor-Clone_de_Desktop_Qt_5_6_0_MSVC2015_32bit-Debug/debug/thumbnails/screen-playerid.png");
//    QPixmap scaled=pic.scaledToWidth(250, Qt::FastTransformation);
//    QLabel *label = new QLabel(this);
//    label->setPixmap(scaled);

    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(150,150,150));
    QLabel* labelID = new QLabel(m_sSceneID, this);
    labelID->setAlignment(Qt::AlignCenter);
//    vLayout->addWidget(label);
    vLayout->addWidget(labelID);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    this->setLayout(vLayout);
    this->setMinimumWidth(250);
    this->setMaximumWidth(250);

//    m_pAddSceneButton = new QPushButton(parent);
//    m_pAddSceneButton->setMaximumHeight(50);
//    m_pAddSceneButton->setMaximumWidth(50);
//    m_pAddSceneButton->setText("+");
//    m_pAddSceneButton->move((a_iIndex * 255) - 10, (int)(parent->height() / 2));
//    m_pAddSceneButton->raise();
//    m_pAddSceneButton->show();

    //m_pSynchroSceneButton = new QPushButton(this);
}

QString CThumbnailWidget::GetSceneID()
{
    return this->m_sSceneID;
}

int CThumbnailWidget::GetPlayerID()
{
    return this->m_iPlayerID;
}

bool CThumbnailWidget::IsSceneID(const QString& a_rSceneID, int a_iPlayerID)
{
    return (this->m_sSceneID.compare(a_rSceneID) && a_iPlayerID == m_iPlayerID);
}

void CThumbnailWidget::Unselect()
{
    this->setMinimumWidth(250);
    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(150,150,150));
    this->setAutoFillBackground(true);
    this->setPalette(pal);
}

void CThumbnailWidget::Select()
{
    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(255,255,255));
    this->setAutoFillBackground(true);
    this->setPalette(pal);
}

void CThumbnailWidget::LastActive()
{
    this->setMinimumWidth(250);
    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(200,200,200));
    this->setAutoFillBackground(true);
    this->setPalette(pal);
}

void CThumbnailWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(this->m_bIsHover && this->m_bIsPress){
//        emit onClick(this->m_sSceneID, this->m_iPlayerID);
    }
}

void CThumbnailWidget::mousePressEvent(QMouseEvent *event)
{
    this->m_bIsPress = true;
    if(this->m_bIsHover && event->button() == Qt::LeftButton)
    {
        emit onClick(this); // substract 1 to match id in Cocos ( 0 & 1 )
//        this->setMinimumWidth(300);
    }
}

void CThumbnailWidget::leaveEvent(QEvent * event)
{
    this->m_bIsHover = false;
    this->m_bIsPress = false;
}

void CThumbnailWidget::enterEvent(QEvent * event)
{
    this->m_bIsHover = true;
}

