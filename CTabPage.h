#ifndef CTABPAGE_H
#define CTABPAGE_H

#include <QWidget>

namespace Ui {
class CTabPage;
}

class CTabPage : public QWidget
{
    Q_OBJECT

public:
    explicit CTabPage(QWidget *parent = 0);
    ~CTabPage();
    QWidget* GetTimeline();

private:
    Ui::CTabPage *ui;
};

#endif // CTABPAGE_H
