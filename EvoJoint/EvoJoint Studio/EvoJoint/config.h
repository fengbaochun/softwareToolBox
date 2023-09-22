#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QSettings>

class config
{
public:
    ~config();
    config& operator = (const config&) = delete;
    static config &getInstance(void);
    QSettings *configFile;
    void writeConfig(const QString group, const QString key, QVariant value);
    QVariant readConfig(const QString group, const QString key);
    void removeKey(const QString key);
    QStringList readAllKeys(const QString group);        //读取指定结点下的所有键
    QStringList readAllGroups();        //读取所有结点
private:
    config();
    static config * instance;

};

#endif // CONFIG_H
