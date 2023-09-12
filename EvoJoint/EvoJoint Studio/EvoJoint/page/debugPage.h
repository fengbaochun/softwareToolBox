#ifndef DEBUGPAGE_H
#define DEBUGPAGE_H

#include <QWidget>

namespace Ui {
class debugPage;
}

class debugPage : public QWidget
{
    Q_OBJECT

public:
    explicit debugPage(QWidget *parent = nullptr);
    ~debugPage();

private slots:
    void logSlot(const QString &message, int level);

private:
    Ui::debugPage *ui;
};

#endif // DEBUGPAGE_H
