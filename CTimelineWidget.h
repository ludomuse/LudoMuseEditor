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
    void PushScene(const QString& a_sSceneID, int a_iPlayerID);
    //    void InsertScene(a_sSceneID, a_iPlayerID, a_sPrevSceneID);
    //    void RemoveScene(a_sSceneID);
    void SelectThumbnail(CThumbnailWidget* a_pThumbnail);
    void SelectNextThumbnail();
    void SelectPrevThumbnail();
    void DeactivateThumbnails();
    void ActivateThumbnails();

    bool AddThumbnail(CThumbnailWidget *a_pThumbnail, QWidget* a_pColumn);
    void LoadPreviews();
    void LoadPreview(const QString& a_sSceneID);
    //    CThumbnailWidget* RemoveThumbnail(a_sSceneID);
    const QString& GetCurrentSceneID();

private slots:
    void thumbnailClicked(CThumbnailWidget* a_pThumbnail);

signals:
    void thumbnailSelected(const QString& a_sSceneID);

private:
    QList<CThumbnailWidget*>* m_pThumbnailList;
    int m_iCurrentIndex1;
    int m_iCurrentIndex2;
    int m_iActivePlayer;
    CThumbnailsLoaderThread *m_pLoader;

    void ConnectThumbnail(CThumbnailWidget* a_pThumbnail);
    QWidget* NewColumn();
    int FindThumbnailIndexByID(const QString& a_sStringID);

    //    void AddOneThumbnail(CThumbnailWidget* a_pThumbnail);
    //    void AddTwoThumbnails(CThumbnailWidget* a_pThumbnail1, CThumbnailWidget* a_pThumbnail2);
};

#endif // CTIMELINEWIDGET_H
