#include "config.h"
#include <QTextCodec>
config* config::instance = NULL;

config::config()
{
    configFile = new QSettings("./config/configFile.ini",QSettings::IniFormat);
    configFile->setIniCodec(QTextCodec::codecForName("UTF-8"));
    //参数初始化
    if (configFile->value("VIEW/debugView").isNull())
        configFile->setValue("VIEW/debugView", false);
    if (configFile->value("VIEW/operateView").isNull())
        configFile->setValue("VIEW/operateView", false);
    if (configFile->value("VIEW/test").isNull())
        configFile->setValue("VIEW/test", false);
}

config::~config()
{
    delete configFile;
}

config &config::getInstance()
{
    if(instance == NULL)
        instance = new config();
    return *instance;
}

void config::writeConfig(const QString group, const QString key, QVariant value)
{
    QString str("%1/%2");
    configFile->setValue(str.arg(group,key), value);
}

QVariant config::readConfig(const QString group, const QString key)
{
    QString str("%1/%2");
    QVariant qvar = configFile->value(str.arg(group, key));
    return qvar;
}

void config::removeKey(const QString key)
{
    configFile->remove(key);
}

QStringList config::readAllKeys(const QString group)
{
    QStringList result;
    configFile->beginGroup(group);
    result = configFile->childKeys();
    configFile->endGroup();
    return  result;
}

QStringList config::readAllGroups()
{
    return  configFile->childGroups();
}

//根据配置加载UI
void config::loadWidgetCfg(const QString group, const QString key, QStackedWidget *ui){
    QVariant res = readConfig(group, key);
    if(res == true){
        ui->show();
    }else{
        ui->hide();
    }
}

void config::updateWidgetCfg(const QString group, const QString key, QStackedWidget *ui)
{
    QVariant val = ui->isHidden() ? false : true;
    writeConfig(group, key, val);
}





