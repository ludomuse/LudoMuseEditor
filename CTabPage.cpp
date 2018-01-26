#include "CTabPage.h"
#include "ui_ctabpage.h"

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
