#ifndef CTIMELINEWIDGET_H
#define CTIMELINEWIDGET_H

#include <QWidget>
#include <QList>

#include "CThumbnailWidget.h"
#include "CThumbnailsLoaderThread.h"
#include "LudoMuse_src/Classes/Engine/Include/CNode.h"

class CTimelineWidget : public QWidget
{
    Q_OBJECT
public:
    CTimelineWidget(QWidget* parent = Q_NULLPTR);

    void UpdateTimeline();
    void PushScene(const QString& a_sSceneID, int a_iPlayerID, const QString &a_sSyncedScene = "");
    void InsertScene(const QString& a_sPrevSceneID, const QString& a_sSceneID, int a_iPlayerID, const QString &a_sSyncedScene = "");
    void InsertSharedScene(const QString& a_sPrevSceneID1, const QString& a_sPrevSceneID2, const QString& a_sSceneID);
    void RemoveScene(const QString& a_sSceneID);
    //    void InsertScene(a_sSceneID, a_iPlayerID, a_sPrevSceneID);
    //    void RemoveScene(a_sSceneID);
    void SelectThumbnail(CThumbnailWidget* a_pThumbnail);
    void SelectThumbnail(const QString& a_sSceneID);
    void SelectNextThumbnail();
    void SelectPrevThumbnail();
    void DeactivateThumbnails();
    void ActivateThumbnails();

    bool AddThumbnail(CThumbnailWidget *a_pThumbnail, QWidget* a_pColumn);
    void LoadPreviews();
    void LoadPreview(const QString& a_sSceneID);
    //    CThumbnailWidget* RemoveThumbnail(a_sSceneID);
    QString GetCurrentSceneID();
    QString GetPlayerSceneID(int a_iPlayerID);
    int GetCurrentPlayer();
    int GetCurrentScenePlayer();
    void SetCurrentPlayer(int a_iPlayerID);

private slots:
    void thumbnailClicked(CThumbnailWidget* a_pThumbnail);

signals:
    void thumbnailSelected();
    void saveThumbnail();

private:
    QList<CThumbnailWidget*>* m_pThumbnailList;
    int m_iCurrentIndex1;
    int m_iCurrentIndex2;
    int m_iActivePlayer;
    CThumbnailsLoaderThread *m_pLoader;

    void ConnectThumbnail(CThumbnailWidget* a_pThumbnail);
    QWidget* NewColumn(bool a_bIsGame = false);
    int FindThumbnailIndex(const QString& a_sStringID);
    int GetCurrentThumbnailIndex(int a_iPlayerID);
    void SetCurrentThumbnailIndex(int a_iPlayerID, int a_iIndex);
    int FindNextThumbnailIndex(int a_iStartIndex, int a_iPlayerID);
    int FindPrevThumbnailIndex(int a_iStartIndex, int a_iPlayerID);

    //    void AddOneThumbnail(CThumbnailWidget* a_pThumbnail);
    //    void AddTwoThumbnails(CThumbnailWidget* a_pThumbnail1, CThumbnailWidget* a_pThumbnail2);
};

#endif // CTIMELINEWIDGET_H
