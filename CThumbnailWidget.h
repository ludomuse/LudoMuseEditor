#ifndef CTHUMBNAILWIDGET_H
#define CTHUMBNAILWIDGET_H

#include <QWidget>
#include <QString>

class CThumbnailWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CThumbnailWidget(QWidget *parent = 0);
    CThumbnailWidget(QString a_sId, int a_iPlayerID = 0, QWidget *parent=Q_NULLPTR);
    bool isSceneID(const QString& a_rSceneID, int a_iPlayerID);
    QString getSceneID();
    void unselect();
    void LastActive();

protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void leaveEvent(QEvent * event);
    void enterEvent(QEvent * event);

signals:
    void onClick(const QString&, int a_iPlayerID, CThumbnailWidget* a_pClickedWidget);



private:
    QString m_sSceneID;
    int m_iPlayerID;
    bool m_bIsHover;
    bool m_bIsPress;
};

#endif // CTHUMBNAILWIDGET_H
