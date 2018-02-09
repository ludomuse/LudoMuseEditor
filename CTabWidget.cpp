#include "CTabWidget.h"

TabWidget::TabWidget()
{
    setMovable(true);
    connect(tabBar(),SIGNAL(tabMoved(int,int)),this,SLOT(reogarniseTabs(int,int)));
}

void TabWidget::reogarniseTabs(int from, int to){
    qDebug() << "HELLO REORGANISE " << from << " TO " << to;
}
