#include "CTabPage.h"
#include "ui_CTabPage.h"

CTabPage::CTabPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CTabPage)
{
    ui->setupUi(this);
}

CTabPage::~CTabPage()
{
    delete ui;
}

QWidget* CTabPage::GetTimeline(){
    return ui->timelineContainer;
}
