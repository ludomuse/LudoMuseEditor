#include "CThumbnailWidget.h"

#include <QWidget>
#include <QBoxLayout>
#include <QPalette>
#include <QLabel>
#include <QDebug>

CThumbnailWidget::CThumbnailWidget(QWidget *parent) : QWidget(parent),
    m_bIsHover(false),
    m_bIsPress(false)
{

}

CThumbnailWidget::CThumbnailWidget(QString a_sceneID, QWidget *parent) : QWidget(parent),
    m_sSceneID(a_sceneID),
    m_bIsHover(false),
    m_bIsPress(false)
{
    QHBoxLayout *hLayout = new QHBoxLayout();

    QPalette pal(palette());
    pal.setColor(QPalette::Background, QColor(150,150,150));

    this->setLayout(hLayout);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    this->layout()->addWidget(new QLabel(m_sSceneID, this));

    this->setMinimumWidth(250);
}

void CThumbnailWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(this->m_bIsHover && this->m_bIsPress){
        emit onClick(this->m_sSceneID);
    }
}

void CThumbnailWidget::mousePressEvent(QMouseEvent *event)
{
    this->m_bIsPress = true;
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
