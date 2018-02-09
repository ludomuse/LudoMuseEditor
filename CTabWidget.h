#ifndef CTABWIDGET_H
#define CTABWIDGET_H
#include <QTabWidget>


class TabWidget : public QTabWidget
{
    Q_OBJECT
public:
    TabWidget();

public slots:
    void reogarniseTabs(int from,int to);
};

#endif // CTABWIDGET_H
