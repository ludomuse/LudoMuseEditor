#include "CTimelineWidget.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <CMainWindow.h>

CTimelineWidget::CTimelineWidget(QWidget *parent):
    QWidget(parent),
    m_pLoader(new CThumbnailsLoaderThread()),
    m_pThumbnailList(new QList<CThumbnailWidget*>),
    m_iActivePlayer(0)
{
    connect(m_pLoader, SIGNAL(finished()), m_pLoader, SLOT(deleteLater()));

    m_pLoader->start();
    QHBoxLayout* pHBLayout = new QHBoxLayout();
    pHBLayout->setSpacing(10);
    pHBLayout->setContentsMargins(5, 5, 5, 5);
    setLayout(pHBLayout);
}

void CTimelineWidget::UpdateTimeline()
{
    QLayoutItem *child;
    if(layout() != Q_NULLPTR)
    {
        while (layout()->takeAt(0) != 0) {
            //                        delete child->widget();
            delete child;
        }
    }
    int iLayout1 = 0;
    int iLayout2 = 0;
    for (int i = 0; i < m_pThumbnailList->length(); i++)
    {
        if (m_pThumbnailList->at(i)->GetPlayerID() == CMainWindow::PLAYER_1)
        {
            if (iLayout1 >= layout()->count())
            {
                layout()->addWidget(NewColumn());

            }
            AddThumbnail(m_pThumbnailList->at(i), layout()->itemAt(iLayout1)->widget());
            iLayout1++;
        }
        else if (m_pThumbnailList->at(i)->GetPlayerID() == CMainWindow::PLAYER_2)
        {
            if (iLayout2 >= layout()->count())
            {
                layout()->addWidget(NewColumn());
            }
            AddThumbnail(m_pThumbnailList->at(i), layout()->itemAt(iLayout2)->widget());
            iLayout2++;
        }
        else if (m_pThumbnailList->at(i)->GetPlayerID() == CMainWindow::BOTH_PLAYER)
        {
            layout()->addWidget(NewColumn());

            AddThumbnail(m_pThumbnailList->at(i), layout()->itemAt(layout()->count()-1)->widget());
            iLayout1 = layout()->count();
            iLayout2 = layout()->count();
        }
    }
}

QWidget* CTimelineWidget::NewColumn()
{
    QWidget* pNewColumn = new QWidget(this);
    QVBoxLayout* pVBLayout = new QVBoxLayout();
    pVBLayout->setSpacing(10);
    pVBLayout->setContentsMargins(0, 0, 0, 0);
    pNewColumn->setLayout(pVBLayout);
    return pNewColumn;
}

bool CTimelineWidget::AddThumbnail(CThumbnailWidget *a_pThumbnail, QWidget* a_pColumn)
{
    QVBoxLayout* pVBLayout = dynamic_cast<QVBoxLayout*>(a_pColumn->layout());
    if (!pVBLayout)
    {
        return false;
    }
    if (pVBLayout->count() == 0)
    {
        pVBLayout->addWidget(a_pThumbnail);
    }
    else if (pVBLayout->count() == 1)
    {
        CThumbnailWidget* pOtherThumbnail = dynamic_cast<CThumbnailWidget*>(pVBLayout->itemAt(0)->widget());
        if (pOtherThumbnail)
        {
            if (a_pThumbnail->GetPlayerID() == CMainWindow::PLAYER_1 && pOtherThumbnail->GetPlayerID() == CMainWindow::PLAYER_2)
            {
                pVBLayout->insertWidget(0, a_pThumbnail);
            }
            else if (a_pThumbnail->GetPlayerID() == CMainWindow::PLAYER_2 && pOtherThumbnail->GetPlayerID() == CMainWindow::PLAYER_1)
            {
                pVBLayout->insertWidget(1, a_pThumbnail);
            }
            else {
                return false;
            }
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
    if (a_pThumbnail->GetPlayerID() == CMainWindow::PLAYER_1)
    {
        pVBLayout->setAlignment(a_pThumbnail, Qt::AlignTop);
    }
    else if (a_pThumbnail->GetPlayerID() == CMainWindow::PLAYER_2)
    {
        pVBLayout->setAlignment(a_pThumbnail, Qt::AlignBottom);
    }
    else if (a_pThumbnail->GetPlayerID() == CMainWindow::BOTH_PLAYER)
    {
        pVBLayout->setAlignment(a_pThumbnail, Qt::AlignCenter);
    }
    return true;
}

void CTimelineWidget::PushScene(const QString &a_sSceneID, int a_iPlayerID)
{
    CThumbnailWidget* pNewThumbnail = new CThumbnailWidget(a_sSceneID, a_iPlayerID);
    ConnectThumbnail(pNewThumbnail);
    m_pThumbnailList->append(pNewThumbnail);
}

void CTimelineWidget::ConnectThumbnail(CThumbnailWidget* a_pThumbnail)
{
    connect(a_pThumbnail,
            SIGNAL(onClick(CThumbnailWidget*)),
            this,
            SLOT(thumbnailClicked(CThumbnailWidget*)));
}

void CTimelineWidget::thumbnailClicked(CThumbnailWidget* a_pThumbnail)
{
    SelectThumbnail(a_pThumbnail);
}

void CTimelineWidget::SelectThumbnail(CThumbnailWidget *a_pThumbnail)
{
    //    SaveThumbnail();
    DeactivateThumbnails();
    int iPlayerID = a_pThumbnail->GetPlayerID();
    switch (iPlayerID)
    {
    case CMainWindow::PLAYER_1:
        m_iActivePlayer = CMainWindow::PLAYER_1;
        m_iCurrentIndex1 = m_pThumbnailList->indexOf(a_pThumbnail);
        break;
    case CMainWindow::PLAYER_2:
        m_iActivePlayer = CMainWindow::PLAYER_2;
        m_iCurrentIndex2 = m_pThumbnailList->indexOf(a_pThumbnail);
        break;
    case CMainWindow::BOTH_PLAYER:
        m_iCurrentIndex1 = m_pThumbnailList->indexOf(a_pThumbnail);
        m_iCurrentIndex2 = m_iCurrentIndex1;
        break;
    }
    ActivateThumbnails();
    //    ShowCurrentScene();
}

void CTimelineWidget::SelectNextThumbnail()
{
    DeactivateThumbnails();
    int iNextIndex;
    if (m_iActivePlayer == CMainWindow::PLAYER_1)
    {
        iNextIndex = m_iCurrentIndex1;
        do
        {
            iNextIndex++;
        }
        while (iNextIndex < m_pThumbnailList->count() &&
               m_pThumbnailList->at(iNextIndex)->GetPlayerID() != CMainWindow::PLAYER_1 &&
               m_pThumbnailList->at(iNextIndex)->GetPlayerID() != CMainWindow::BOTH_PLAYER);
        if (iNextIndex < m_pThumbnailList->count())
        {
            m_iCurrentIndex1 = iNextIndex;
            if (m_pThumbnailList->at(iNextIndex)->GetPlayerID() == CMainWindow::BOTH_PLAYER)
            {
                m_iCurrentIndex2 = m_iCurrentIndex1;
            }
        }
    }
    else
    {
        iNextIndex = m_iCurrentIndex2;
        do
        {
            iNextIndex++;
        }
        while (iNextIndex < m_pThumbnailList->count() &&
               m_pThumbnailList->at(iNextIndex)->GetPlayerID() != CMainWindow::PLAYER_2 &&
               m_pThumbnailList->at(iNextIndex)->GetPlayerID() != CMainWindow::BOTH_PLAYER);
        if (iNextIndex < m_pThumbnailList->count())
        {
            m_iCurrentIndex2 = iNextIndex;
            if (m_pThumbnailList->at(iNextIndex)->GetPlayerID() == CMainWindow::BOTH_PLAYER)
            {
                m_iCurrentIndex1 = m_iCurrentIndex2;
            }
        }
    }
    ActivateThumbnails();
}

void CTimelineWidget::SelectPrevThumbnail()
{
    DeactivateThumbnails();
    int iPrevIndex;
    if (m_iActivePlayer == CMainWindow::PLAYER_1)
    {
        iPrevIndex = m_iCurrentIndex1;
        do
        {
            iPrevIndex--;
        }
        while (iPrevIndex >= 0 &&
               m_pThumbnailList->at(iPrevIndex)->GetPlayerID() != CMainWindow::PLAYER_1 &&
               m_pThumbnailList->at(iPrevIndex)->GetPlayerID() != CMainWindow::BOTH_PLAYER);
        if (iPrevIndex >= 0)
        {
            m_iCurrentIndex1 = iPrevIndex;
            if (m_pThumbnailList->at(iPrevIndex)->GetPlayerID() == CMainWindow::BOTH_PLAYER)
            {
                m_iCurrentIndex2 = m_iCurrentIndex1;
            }
        }
    }
    else
    {
        iPrevIndex = m_iCurrentIndex2;
        do
        {
            iPrevIndex--;
        }
        while (iPrevIndex >= 0 &&
               m_pThumbnailList->at(iPrevIndex)->GetPlayerID() != CMainWindow::PLAYER_2 &&
               m_pThumbnailList->at(iPrevIndex)->GetPlayerID() != CMainWindow::BOTH_PLAYER);
        if (iPrevIndex >= 0)
        {
            m_iCurrentIndex2 = iPrevIndex;
            if (m_pThumbnailList->at(iPrevIndex)->GetPlayerID() == CMainWindow::BOTH_PLAYER)
            {
                m_iCurrentIndex1 = m_iCurrentIndex2;
            }
        }
    }
    ActivateThumbnails();
}

void CTimelineWidget::DeactivateThumbnails()
{
    if (m_iCurrentIndex1 >= 0 && m_iCurrentIndex1 < m_pThumbnailList->count())
    {
        m_pThumbnailList->at(m_iCurrentIndex1)->Unselect();
    }
    if (m_iCurrentIndex2 >= 0 && m_iCurrentIndex2 < m_pThumbnailList->count())
    {
        m_pThumbnailList->at(m_iCurrentIndex2)->Unselect();
    }
}

void CTimelineWidget::ActivateThumbnails()
{
    int iActiveIndex;
    int iOtherIndex;
    if (m_iActivePlayer == CMainWindow::PLAYER_2)
    {
        iActiveIndex = m_iCurrentIndex2;
        iOtherIndex = m_iCurrentIndex1;
    }
    else
    {
        iActiveIndex = m_iCurrentIndex1;
        iOtherIndex = m_iCurrentIndex2;
    }

    if (iActiveIndex >= 0 &&
            m_pThumbnailList->count() > iActiveIndex)
    {
        if (iOtherIndex >= 0 &&
                m_pThumbnailList->count() > iOtherIndex)
        {
            m_pThumbnailList->at(iOtherIndex)->LastActive();
        }
        m_pThumbnailList->at(iActiveIndex)->Select();
    }
    else if (iOtherIndex >= 0 &&
             m_pThumbnailList->count() > iOtherIndex)
    {
        m_pThumbnailList->at(iOtherIndex)->Select();
        m_iActivePlayer = 1 - m_iActivePlayer;
    }
}

void CTimelineWidget::LoadPreviews()
{
    m_pLoader->AddThumbnailList(m_pThumbnailList);
}

void CTimelineWidget::LoadPreview(const QString& a_sSceneID)
{
    int iIndex = FindThumbnailIndexByID(a_sSceneID);
    if (iIndex != -1) {
        m_pLoader->RemoveThumbnail(m_pThumbnailList->at(iIndex));
        m_pLoader->AddThumbnail(m_pThumbnailList->at(iIndex));
    }
}

int CTimelineWidget::FindThumbnailIndexByID(const QString& a_sSceneId)
{
    int iIndex;
    for (iIndex = 0; iIndex < m_pThumbnailList->count(); iIndex++)
    {
        if (m_pThumbnailList->at(iIndex)->GetSceneID() == a_sSceneId)
        {
            return iIndex;
        }
    }
    return -1;
}

const QString& CTimelineWidget::GetCurrentSceneID()
{
    if (m_iActivePlayer == CMainWindow::PLAYER2)
    {
        return m_pThumbnailList->at(m_iCurrentIndex2)->GetSceneID();
    }
    else
    {
        return m_pThumbnailList->at(m_iCurrentIndex1)->GetSceneID();
    }
}
