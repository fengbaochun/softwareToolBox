#include "debugPage.h"
#include "ui_debugPage.h"

debugPage::debugPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::debugPage)
{
    ui->setupUi(this);
}

debugPage::~debugPage()
{
    delete ui;
}
