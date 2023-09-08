#ifndef DEVCFGPAGE_H
#define DEVCFGPAGE_H

#include <QWidget>

namespace Ui {
class devCfgPage;
}

class devCfgPage : public QWidget
{
    Q_OBJECT

public:
    explicit devCfgPage(QWidget *parent = nullptr);
    ~devCfgPage();

    typedef struct{
        int index;
        QString name;
        QString port;
    }devInfoType;

    QList<devInfoType> dev;

private slots:
    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void on_pushButton_clicked(bool checked);
    void on_devList_activated(int index);
    void on_comboBox_activated(int index);

private:
    Ui::devCfgPage *ui;
};

#endif // DEVCFGPAGE_H
