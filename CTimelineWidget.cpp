#include "CTimelineWidget.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <CMainWindow.h>

CTimelineWidget::CTimelineWidget(QWidget *parent):
    QWidget(parent),
    m_pLoader(new CThumbnailsLoaderThread()),
    m_pThumbnailList(new QList<CThumbnailWidget*>),
    m_iActivePlayer(0),
    m_iCurrentIndex1(-1),
    m_iCurrentIndex2(-1)
{
    connect(m_pLoader, SIGNAL(finished()), m_pLoader, SLOT(deleteLater()));

    m_pLoader->start();
    QHBoxLayout* pHBLayout = new QHBoxLayout();
    pHBLayout->setSpacing(6);
    pHBLayout->setContentsMargins(3, 3, 3, 3);
    setLayout(pHBLayout);
}

void CTimelineWidget::UpdateTimeline()
{
    QLayoutItem *child;
    if(layout() != Q_NULLPTR)
    {
        while (((child = layout()->takeAt(0)) != 0)) {
            //                                    delete child->widget();
            delete child;
        }
    }
    int iLayout1 = 0;
    int iLayout2 = 0;
    for (int i = 0; i < m_pThumbnailList->length(); i++)
    {
        if (m_pThumbnailList->at(i)->GetSyncedID() != QString("")) {
            int j = FindThumbnailIndex(m_pThumbnailList->at(i)->GetSyncedID());
            if (j > i)
            {
                layout()->addWidget(NewColumn(true));
                AddThumbnail(m_pThumbnailList->at(i), layout()->itemAt(layout()->count()-1)->widget());
                AddThumbnail(m_pThumbnailList->at(j), layout()->itemAt(layout()->count()-1)->widget());
                iLayout1 = layout()->count();
                iLayout2 = layout()->count();
            }
        }
        else if (m_pThumbnailList->at(i)->GetPlayerID() == CMainWindow::PLAYER_1)
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

QWidget* CTimelineWidget::NewColumn(bool a_bIsGame)
{
    QWidget* pNewColumn = new QWidget(this);
    QVBoxLayout* pVBLayout = new QVBoxLayout();
    pVBLayout->setSpacing(6);
    pVBLayout->setContentsMargins(3, 3, 3, 3);
    pNewColumn->setLayout(pVBLayout);
    if (a_bIsGame)
    {
        QPalette Pal(palette());
        Pal.setColor(QPalette::Background, QColor(255, 170, 0));
        pNewColumn->setAutoFillBackground(true);
        pNewColumn->setPalette(Pal);
    }
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

void CTimelineWidget::PushScene(const QString &a_sSceneID, int a_iPlayerID, const QString &a_sSyncedSceneID)
{
    CThumbnailWidget* pNewThumbnail = new CThumbnailWidget(a_sSceneID, a_iPlayerID, a_sSyncedSceneID);
    ConnectThumbnail(pNewThumbnail);
    m_pThumbnailList->append(pNewThumbnail);
}

void CTimelineWidget::InsertScene(const QString& a_sPrevSceneID, const QString& a_sSceneID, int a_iPlayerID, const QString &a_sSyncedSceneID) {
    if (a_sPrevSceneID == "") {
        CThumbnailWidget* pNewThumbnail = new CThumbnailWidget(a_sSceneID, a_iPlayerID, a_sSyncedSceneID);
        ConnectThumbnail(pNewThumbnail);
        m_pThumbnailList->insert(0, pNewThumbnail);
    }
    else
    {
        int iPrevIndex = FindThumbnailIndex(a_sPrevSceneID);
        if (iPrevIndex >= 0) {
            CThumbnailWidget* pNewThumbnail = new CThumbnailWidget(a_sSceneID, a_iPlayerID);
            ConnectThumbnail(pNewThumbnail);
            m_pThumbnailList->insert(iPrevIndex+1, pNewThumbnail);
        }
    }
}

void CTimelineWidget::RemoveScene(const QString& a_sSceneID)
{
    int iIndex = FindThumbnailIndex(a_sSceneID);
    if (iIndex >= 0 && iIndex < m_pThumbnailList->length()) {
        DeactivateThumbnails();
        int iPlayerID = m_pThumbnailList->at(iIndex)->GetPlayerID();
        if (iPlayerID == CMainWindow::PLAYER_1 || iPlayerID == CMainWindow::BOTH_PLAYER)
        {
            int currentIndex = FindNextThumbnailIndex(m_iCurrentIndex1, CMainWindow::PLAYER_1);
            if (currentIndex >= m_pThumbnailList->count())
            {
                currentIndex = FindPrevThumbnailIndex(m_iCurrentIndex1, CMainWindow::PLAYER_1);
            }
            m_iCurrentIndex1 = currentIndex;
        }
        if (iPlayerID == CMainWindow::PLAYER_2 || iPlayerID == CMainWindow::BOTH_PLAYER)
        {
            int currentIndex = FindNextThumbnailIndex(m_iCurrentIndex2, CMainWindow::PLAYER_2);
            if (currentIndex >= m_pThumbnailList->count())
            {
                currentIndex = FindPrevThumbnailIndex(m_iCurrentIndex2, CMainWindow::PLAYER_2);
            }
            m_iCurrentIndex2 = currentIndex;
        }
        if (m_iCurrentIndex1 > iIndex) {
            m_iCurrentIndex1--;
        }
        if (m_iCurrentIndex2 > iIndex) {
            m_iCurrentIndex2--;
        }
        CThumbnailWidget* pDeleted = m_pThumbnailList->takeAt(iIndex);
        m_pLoader->RemoveThumbnail(pDeleted);
        pDeleted->deleteLater();
        ActivateThumbnails();
    }
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
    emit saveThumbnail();
    SelectThumbnail(a_pThumbnail);
    emit thumbnailSelected();
}

void CTimelineWidget::SelectThumbnail(CThumbnailWidget *a_pThumbnail)
{
    //    SaveThumbnail();
    DeactivateThumbnails();
    int iPlayerID = a_pThumbnail->GetPlayerID();
    switch (iPlayerID)
    {
    case CMainWindow::PLAYER_1:
        if (m_iActivePlayer == CMainWindow::BOTH_PLAYER) {
            m_iCurrentIndex2 = -1;
        }
        m_iActivePlayer = CMainWindow::PLAYER_1;
        m_iCurrentIndex1 = m_pThumbnailList->indexOf(a_pThumbnail);
        break;
    case CMainWindow::PLAYER_2:
        if (m_iActivePlayer == CMainWindow::BOTH_PLAYER) {
            m_iCurrentIndex1 = -1;
        }
        m_iActivePlayer = CMainWindow::PLAYER_2;
        m_iCurrentIndex2 = m_pThumbnailList->indexOf(a_pThumbnail);
        break;
    case CMainWindow::BOTH_PLAYER:
        m_iActivePlayer = CMainWindow::BOTH_PLAYER;
        m_iCurrentIndex1 = m_pThumbnailList->indexOf(a_pThumbnail);
        m_iCurrentIndex2 = m_iCurrentIndex1;
        break;
    }
    ActivateThumbnails();
}

void CTimelineWidget::SelectThumbnail(const QString& a_sSceneID) {
    int iIndex = FindThumbnailIndex(a_sSceneID);
    if (iIndex >= 0) {
        SelectThumbnail(m_pThumbnailList->at(iIndex));
    }
}

void CTimelineWidget::SelectNextThumbnail()
{
    DeactivateThumbnails();
    int iNextIndex1 = -1;
    int iNextIndex2 = -1;
    switch (m_iActivePlayer) {
    case CMainWindow::PLAYER_1 :
        iNextIndex1 = FindNextThumbnailIndex(m_iCurrentIndex1, CMainWindow::PLAYER_1);
        if (iNextIndex1 >= 0 && m_pThumbnailList->at(iNextIndex1)->GetPlayerID() == CMainWindow::BOTH_PLAYER)
        {
            iNextIndex2 = iNextIndex1;
        }
        break;
    case CMainWindow::PLAYER_2 :
        iNextIndex2 = FindNextThumbnailIndex(m_iCurrentIndex2, CMainWindow::PLAYER_2);
        if (iNextIndex2 >= 0 && m_pThumbnailList->at(iNextIndex2)->GetPlayerID() == CMainWindow::BOTH_PLAYER)
        {
            iNextIndex1 = iNextIndex2;
        }
        break;
    case CMainWindow::BOTH_PLAYER :
        iNextIndex1 = FindNextThumbnailIndex(m_iCurrentIndex1, CMainWindow::PLAYER_1);
        iNextIndex2 = FindNextThumbnailIndex(m_iCurrentIndex2, CMainWindow::PLAYER_2);
        break;
    }
    if (iNextIndex2 >= 0)
    {
        m_iCurrentIndex2 = iNextIndex2;
        m_iActivePlayer = m_pThumbnailList->at(iNextIndex2)->GetPlayerID();
    }
    if (iNextIndex1 >= 0)
    {
        m_iCurrentIndex1 = iNextIndex1;
        m_iActivePlayer = m_pThumbnailList->at(iNextIndex1)->GetPlayerID();
    }
    ActivateThumbnails();
}


void CTimelineWidget::SelectPrevThumbnail()
{
    DeactivateThumbnails();
    int iPrevIndex1 = -1;
    int iPrevIndex2 = -1;
    switch (m_iActivePlayer) {
    case CMainWindow::PLAYER_1 :
        iPrevIndex1 = FindPrevThumbnailIndex(m_iCurrentIndex1, CMainWindow::PLAYER_1);
        if (iPrevIndex1 >= 0 && m_pThumbnailList->at(iPrevIndex1)->GetPlayerID() == CMainWindow::BOTH_PLAYER)
        {
            iPrevIndex2 = iPrevIndex1;
        }
        break;
    case CMainWindow::PLAYER_2 :
        iPrevIndex2 = FindPrevThumbnailIndex(m_iCurrentIndex2, CMainWindow::PLAYER_2);
        if (iPrevIndex2 >= 0 && m_pThumbnailList->at(iPrevIndex2)->GetPlayerID() == CMainWindow::BOTH_PLAYER)
        {
            iPrevIndex1 = iPrevIndex2;
        }
        break;
    case CMainWindow::BOTH_PLAYER :
        iPrevIndex1 = FindPrevThumbnailIndex(m_iCurrentIndex1, CMainWindow::PLAYER_1);
        iPrevIndex2 = FindPrevThumbnailIndex(m_iCurrentIndex2, CMainWindow::PLAYER_2);
        break;
    }
    if (iPrevIndex2 >= 0)
    {
        m_iCurrentIndex2 = iPrevIndex2;
        m_iActivePlayer = m_pThumbnailList->at(iPrevIndex2)->GetPlayerID();
    }
    if (iPrevIndex1 >= 0)
    {
        m_iCurrentIndex1 = iPrevIndex1;
        m_iActivePlayer = m_pThumbnailList->at(iPrevIndex1)->GetPlayerID();
    }
    ActivateThumbnails();
}

int CTimelineWidget::FindNextThumbnailIndex(int a_iStartIndex, int a_iPlayerID)
{
    int iNextIndex = a_iStartIndex;
    do
    {
        iNextIndex++;
    }
    while (iNextIndex < m_pThumbnailList->count() &&
           m_pThumbnailList->at(iNextIndex)->GetPlayerID() != a_iPlayerID &&
           m_pThumbnailList->at(iNextIndex)->GetPlayerID() != CMainWindow::BOTH_PLAYER);
    return (iNextIndex < m_pThumbnailList->length() ? iNextIndex : -1);
}

int CTimelineWidget::FindPrevThumbnailIndex(int a_iStartIndex, int a_iPlayerID)
{
    int iPrevIndex = a_iStartIndex;
    do
    {
        iPrevIndex--;
    }
    while (iPrevIndex >= 0 &&
           m_pThumbnailList->at(iPrevIndex)->GetPlayerID() != a_iPlayerID &&
           m_pThumbnailList->at(iPrevIndex)->GetPlayerID() != CMainWindow::BOTH_PLAYER);
    return iPrevIndex;
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
    int iIndex = FindThumbnailIndex(a_sSceneID);
    if (iIndex != -1) {
        m_pLoader->RemoveThumbnail(m_pThumbnailList->at(iIndex));
        m_pLoader->AddThumbnail(m_pThumbnailList->at(iIndex));
    }
}

int CTimelineWidget::FindThumbnailIndex(const QString& a_sSceneId)
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

QString CTimelineWidget::GetCurrentSceneID()
{
    return GetPlayerSceneID(m_iActivePlayer);
}

QString CTimelineWidget::GetPlayerSceneID(int a_iPlayerID)
{
    if (a_iPlayerID == CMainWindow::PLAYER_2)
    {
        if (m_iCurrentIndex2 < 0 || m_iCurrentIndex2 >= m_pThumbnailList->length()) {
            return "";
        } else {
            return m_pThumbnailList->at(m_iCurrentIndex2)->GetSceneID();
        }
    }
    else
    {
        if (m_iCurrentIndex1 < 0 || m_iCurrentIndex1 >= m_pThumbnailList->length()) {
            return "";
        } else {
            return m_pThumbnailList->at(m_iCurrentIndex1)->GetSceneID();
        }
    }
}

int CTimelineWidget::GetCurrentScenePlayer()
{
    if (m_iActivePlayer == CMainWindow::PLAYER_2)
    {
        if (m_iCurrentIndex2 < 0 || m_iCurrentIndex2 >= m_pThumbnailList->length()) {
            return -1;
        } else {
            return m_pThumbnailList->at(m_iCurrentIndex2)->GetPlayerID();
        }
    }
    else
    {
        if (m_iCurrentIndex1 < 0 || m_iCurrentIndex1 >= m_pThumbnailList->length()) {
            return -1;
        } else {
            return m_pThumbnailList->at(m_iCurrentIndex1)->GetPlayerID();
        }
    }
}

int CTimelineWidget::GetCurrentPlayer()
{
    return m_iActivePlayer;
}

void CTimelineWidget::SetCurrentPlayer(int a_iPlayerID)
{
    m_iActivePlayer = a_iPlayerID;
}

int CTimelineWidget::GetCurrentThumbnailIndex(int a_iPlayerID)
{
    if (a_iPlayerID == CMainWindow::PLAYER_2)
    {
        return m_iCurrentIndex2;
    }
    else
    {
        return m_iCurrentIndex1;
    }
}

void CTimelineWidget::SetCurrentThumbnailIndex(int a_iPlayerID, int a_iIndex)
{
    if (a_iPlayerID == CMainWindow::PLAYER_2)
    {
        m_iCurrentIndex2 = a_iIndex;
    }
    else
    {
        m_iCurrentIndex1 = a_iIndex;
    }
}
