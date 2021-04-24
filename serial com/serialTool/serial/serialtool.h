#ifndef SERIALTOOL_H
#define SERIALTOOL_H

#include <QWidget>

namespace Ui {
class serialTool;
}

class serialTool : public QWidget
{
    Q_OBJECT

public:
    explicit serialTool(QWidget *parent = nullptr);
    ~serialTool();

private:
    Ui::serialTool *ui;
};

#endif // SERIALTOOL_H
