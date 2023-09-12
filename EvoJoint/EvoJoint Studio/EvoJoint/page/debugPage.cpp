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

void debugPage::logSlot(const QString &message, int level)
{
    if(message.contains("FATAL")){
        const QString ss="<font color=\"#FF0000\">";
        ui->textBrowser->append(ss + qPrintable(message) + "</font> ");//显示红色的字体
    }else{
        ui->textBrowser->append(qUtf8Printable(message));
    }
}
