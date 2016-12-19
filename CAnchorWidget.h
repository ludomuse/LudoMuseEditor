#ifndef CANCHORWIDGET_H
#define CANCHORWIDGET_H

#include <QWidget>

class CAnchorWidget : public QWidget
{
    Q_OBJECT
public:
    CAnchorWidget(int a_iFirstAnchor, QWidget* a_pParent = Q_NULLPTR);

private slots:
    void changeAnchor(int a_iAnchorID);

signals:
    void anchorChanged(int a_iAnchorID);
};
#endif // CANCHORWIDGET_H
