#include "CThumbnailWidget.h"

#include <QWidget>
#include <QBoxLayout>
#include <QPalette>
#include <QLabel>
#include <QDebug>
#include <QMouseEvent>
#include <QPoint>
#include <CProjectManager.h>
#include <QGraphicsColorizeEffect>
#include <QGraphicsDropShadowEffect>

CThumbnailWidget::CThumbnailWidget(QWidget *parent) : QFrame(parent),
    m_bIsHover(false),
    m_bIsPress(false)
{

}

CThumbnailWidget::CThumbnailWidget(QString a_sSceneID, int a_iPlayerID, QString a_sSyncedID,QWidget *parent) : QFrame(parent),
    m_sSceneID(a_sSceneID),
    m_sSyncedID(a_sSyncedID),
    m_bIsHover(false),
    m_bIsPress(false),
    m_iPlayerID(a_iPlayerID)
{
    this->setFrameShape(QFrame::Box);
//    this->setMinimumWidth(250);
//    this->setMaximumWidth(250);
    this->setMinimumHeight(90);
    this->setMaximumHeight(90);
    this->setMinimumWidth(150);
    this->setMaximumWidth(150);


    QVBoxLayout *vLayout = new QVBoxLayout();
    this->setLayout(vLayout);

    Unselect();

    FillThumbnail();

    //    m_pAddSceneButton = new QPushButton(parent);
    //    m_pAddSceneButton->setMaximumHeight(50);
    //    m_pAddSceneButton->setMaximumWidth(50);
    //    m_pAddSceneButton->setText("+");
    //    m_pAddSceneButton->move((a_iIndex * 255) - 10, (int)(parent->height() / 2));
    //    m_pAddSceneButton->raise();
    //    m_pAddSceneButton->show();
    //m_pSynchroSceneButton = new QPushButton(this);

    //    QPixmap bkgnd();
    //    bkgnd = bkgnd.scaled(this->size(), Qt::KeepAspectRatioByExpanding);
    //    QPalette palette;
    //    palette.setBrush(QPalette::Background, bkgnd);
    //    this->setPalette(palette);
}

void CThumbnailWidget::UpdateThumbnail()
{
    //    QLayoutItem *child;
    //    while ((child = this->layout()->takeAt(0)) != 0) {
    //        delete child->widget();
    //        delete child;
    //    }
    //    FillThumbnail();
    QLabel* label = this->findChild<QLabel*>("image", Qt::FindDirectChildrenOnly);
    if (label)
    {
        QPixmap pic(CProjectManager::Instance()->QGetAbsoluteWritablePath() + m_sSceneID+".png");
        if (!pic.isNull())
        {
//            pic = pic.scaledToWidth(this->width(), Qt::SmoothTransformation);
            pic = pic.scaledToHeight(label->height(), Qt::SmoothTransformation);
            label->setPixmap(pic);
        }
    }
}

void CThumbnailWidget::FillThumbnail()
{
    QLayout* layout = this->layout();
    QLabel *label = new QLabel(this);
    label->setObjectName("image");
    label->setAlignment(Qt::AlignHCenter);
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QLabel* labelID = new QLabel(m_sSceneID, this);
    labelID->setObjectName("text");
    labelID->setAlignment(Qt::AlignHCenter);
    labelID->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    layout->addWidget(labelID);
    layout->addWidget(label);
}

QString CThumbnailWidget::GetSceneID()
{
    return this->m_sSceneID;
}

QString CThumbnailWidget::GetSyncedID()
{
    return this->m_sSyncedID;
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
    this->layout()->setContentsMargins(5,5,5,5);
    this->setLineWidth(0);

    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(100,100,100));
    pal.setColor(QPalette::Foreground, QColor(0, 85, 255));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    QGraphicsColorizeEffect *cEffect = new QGraphicsColorizeEffect;
    cEffect->setStrength(1);
    cEffect->setColor(Qt::black);

    this->setGraphicsEffect(cEffect);
}

void CThumbnailWidget::Select()
{
    this->layout()->setContentsMargins(2,2,2,2);
    this->setLineWidth(3);

    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(250,250,250));
    pal.setColor(QPalette::Foreground, QColor(0, 85, 255));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    this->setGraphicsEffect(0);
}

void CThumbnailWidget::LastActive()
{
    this->layout()->setContentsMargins(3,3,3,3);
    this->setLineWidth(2);

    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(175,175,175));
    pal.setColor(QPalette::Foreground, QColor(0, 85, 255));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    this->setGraphicsEffect(0);
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
