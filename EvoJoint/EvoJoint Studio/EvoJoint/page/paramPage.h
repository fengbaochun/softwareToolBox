#ifndef PARAMPAGE_H
#define PARAMPAGE_H

#include <QWidget>

namespace Ui {
class paramPage;
}

class paramPage : public QWidget
{
    Q_OBJECT

public:
    explicit paramPage(QWidget *parent = nullptr);
    ~paramPage();

private:
    Ui::paramPage *ui;
};

#endif // PARAMPAGE_H
