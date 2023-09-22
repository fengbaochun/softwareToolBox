#include "config.h"
#include <QTextCodec>
config* config::instance = NULL;

config::config()
{
    configFile = new QSettings("./config/configFile.ini",QSettings::IniFormat);
    configFile->setIniCodec(QTextCodec::codecForName("UTF-8"));

    //可以自行添加一些默认配置
    if (configFile->value("Set/length").isNull())
        configFile->setValue("Set/length", 2);
    if (configFile->value("Set/width").isNull())
        configFile->setValue("Set/width", 2.5);
    if (configFile->value("Set/showLog").isNull())
        configFile->setValue("Set/showLog", true);
    if (configFile->value("Net/ip").isNull())
        configFile->setValue("Net/ip", "127.0.0.1");
    if (configFile->value("Net/port").isNull())
        configFile->setValue("Net/port", 8888);

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
