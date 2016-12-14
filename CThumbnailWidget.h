#ifndef CTHUMBNAILWIDGET_H
#define CTHUMBNAILWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QString>
#include <QPushButton>

class CThumbnailWidget : public QFrame
{
    Q_OBJECT
public:
    explicit CThumbnailWidget(QWidget *parent = 0);
    CThumbnailWidget(QString a_sId, int a_iPlayerID = 0, QString a_sSyncedId = "",QWidget *parent=Q_NULLPTR);
    bool IsSceneID(const QString& a_rSceneID, int a_iPlayerID);
    void Unselect();
    void Select();
    void LastActive();
    int GetPlayerID();
    QString GetSceneID();
    QString GetSyncedID();
    void UpdateThumbnail();
    void FillThumbnail();

protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void leaveEvent(QEvent * event);
    void enterEvent(QEvent * event);

signals:
    void onClick(CThumbnailWidget* a_pClickedWidget);



private:
    QString m_sSceneID;
    QString m_sSyncedID;
    int m_iPlayerID;
    bool m_bIsHover;
    bool m_bIsPress;
    QPushButton* m_pAddSceneButton;
};

#endif // CTHUMBNAILWIDGET_H
