#ifndef CTHUMBNAILWIDGET_H
#define CTHUMBNAILWIDGET_H

#include <QWidget>

class CThumbnailWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CThumbnailWidget(QWidget *parent = 0);
    CThumbnailWidget(QString, QWidget *parent=Q_NULLPTR);

protected:
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void leaveEvent(QEvent * event);
    void enterEvent(QEvent * event);

signals:
    void onClick(const QString&);



private:
    QString m_sSceneID;
    bool m_bIsHover;
    bool m_bIsPress;
};

#endif // CTHUMBNAILWIDGET_H
