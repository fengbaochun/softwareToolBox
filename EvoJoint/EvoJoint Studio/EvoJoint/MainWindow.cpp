#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QSerialPortInfo>
#include <QTimer>
#include <QtEndian>

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDebug>

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QPainter>
#include <QProxyStyle>
#include <QSpinBox>
#include <QStandardItemModel>
#include <QStatusBar>
#include <QStyleOptionTab>

#include "motor/ctl.h"

class CustomTabStyle : public QProxyStyle
{
public:
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
        const QSize &size, const QWidget *widget) const
    {
        QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab) {
            s.transpose();
            s.rwidth() = 90; // 设置每个tabBar中item的大小
            s.rheight() = 60;
        }
        return s;
    }

    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_TabBarTabLabel) {
            if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option)) {
                QRect allRect = tab->rect;

                if (tab->state & QStyle::State_Selected) {
                    painter->save();
                    painter->setPen(0x89cfff);
                    painter->setBrush(QBrush(0x89cfff));
                    painter->drawRect(allRect.adjusted(6, 6, -6, -6));
                    painter->restore();
                }
                QTextOption option;
                option.setAlignment(Qt::AlignCenter);
                if (tab->state & QStyle::State_Selected) {
                    painter->setPen(0xf8fcff);
                }
                else {
                    painter->setPen(0x5d5d5d);
                }

                painter->drawText(allRect, tab->text, option);
                return;
            }
        }

        if (element == CE_TabBarTab) {
            QProxyStyle::drawControl(element, option, painter, widget);
        }
    }
};


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 新建菜单栏
    QMenuBar *menuBr = menuBar();
    setMenuBar(menuBr);
    // 添加菜单
    QMenu *pdev = menuBr->addMenu("设备");
    // 添加菜单项
    QAction *pNew = pdev->addAction("打开设备");
    QAction *pOpen = pdev->addAction("关闭设备");
    pdev->addSeparator();  // 添加分割线

    connect(pNew, &QAction::triggered,[=] (){
        devUi = new devCfgPage();
        devUi->show();
        qDebug() << "Create new file";
    });
    connect(pOpen, &QAction::triggered,[] (){
        qDebug() << "Open file";
    });

    this->setWindowTitle("ToolBOX");

    //添加示波器
    chartUi = new chartPage();
    ui->stackedWidget->insertWidget(0, chartUi);
    //添加参数表
    paramUi = new paramPage();
    ui->stackedWidget->insertWidget(1, paramUi);

    ui->stackedWidget->setCurrentIndex(0);

    //添加 操作页
    operateUi = new operatePage(this);
    ui->operateStacketWidget->insertWidget(0, operateUi);
    ui->operateStacketWidget->setCurrentIndex(0);

    debugUi = new debugPage();
    ui->debugStackedWidget->insertWidget(0, debugUi);
    ui->debugStackedWidget->setCurrentIndex(0);

    //状态栏操作
    QStatusBar *_status=statusBar();
    QLabel *titleLabel = new QLabel;//定义一个label
    titleLabel->setMinimumSize(120, 10);
    titleLabel->setFrameShadow(QFrame::Sunken);
    titleLabel->setText("Qt状态栏（statusbar）的使用!");//label显示内容
    statusBar()->addWidget(titleLabel);//将label加到状态栏上

    //treeview
    treeViewInit();
    ctl::instance()->open("COM35", 921600);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::treeViewInit()
{
    // 设置表头
    ui->treeWidget->setHeaderLabels(QStringList() << "名称" << "值" );
    ui->treeWidget->header()->setVisible(true);
    // 表头文字中间对齐
    QTreeWidgetItem* header = ui->treeWidget->headerItem();
    header->setTextAlignment(0, Qt::AlignLeft);
    header->setTextAlignment(1, Qt::AlignLeft);
    // 设置列宽
    ui->treeWidget->setColumnWidth(0, 100);
    ui->treeWidget->setColumnWidth(1, 60);

    /***************************************************************************/
    //devInfo
    QTreeWidgetItem *devInfo=new QTreeWidgetItem(QStringList()<<"devInfo");
    devInfo->setIcon(0,QIcon("://images/setting2.png"));
    ui->treeWidget->insertTopLevelItem(0, devInfo);

    QTreeWidgetItem* FixedFrame = new QTreeWidgetItem(QStringList()<<"type");
    devInfo->addChild(FixedFrame);

    //添加combox控件
    QComboBox *Type=new QComboBox();
    Type->addItem("type 1");
    Type->addItem("type 2");
    Type->addItem("type 3");
    Type->setMaximumWidth(60);
    ui->treeWidget->setItemWidget(FixedFrame, 1, Type);

    QTreeWidgetItem* devIdItem=new QTreeWidgetItem(QStringList()<<"devId");
    devInfo->addChild(devIdItem);
    //添加QSpinBox控件
    QSpinBox  *DevId=new QSpinBox();
    DevId->setRange(0x00, 0xFF);
    DevId->setMaximumWidth(60);
    ui->treeWidget->setItemWidget(devIdItem, 1, DevId);

    /***************************************************************************/
    /* paramTab */
    QTreeWidgetItem *paramTab=new QTreeWidgetItem(QStringList()<<"paramTab");
    paramTab->setIcon(0,QIcon("://images/setting2.png"));
    ui->treeWidget->insertTopLevelItem(1, paramTab);

    //max Iq
    QTreeWidgetItem* maxIqItem=new QTreeWidgetItem(QStringList()<<"maxIq");
    paramTab->addChild(maxIqItem);
    QDoubleSpinBox  *maxIqQSpinBox=new QDoubleSpinBox();        //添加控件
    maxIqQSpinBox->setDecimals(4);                              //4位小数点
    maxIqQSpinBox->setRange(0, 20);
    maxIqQSpinBox->setMaximumWidth(60);
    ui->treeWidget->setItemWidget(maxIqItem, 1, maxIqQSpinBox);

    //min Iq
    QTreeWidgetItem* minIqItem=new QTreeWidgetItem(QStringList()<<"minIq");
    paramTab->addChild(minIqItem);
    QDoubleSpinBox  *minIqQSpinBox=new QDoubleSpinBox();        //添加控件
    minIqQSpinBox->setDecimals(4);                              //4位小数点
    minIqQSpinBox->setRange(-20, 0);
    minIqQSpinBox->setMaximumWidth(60);
    ui->treeWidget->setItemWidget(minIqItem, 1, minIqQSpinBox);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() != Qt::RightButton)
        return;

    static bool isOperateView = true;
    static bool isDebugView = true;
    QMenu *menu = new QMenu(this);
    QStringList list;
    list << ((isDebugView) ? "隐藏debug页面":"显示debug页面")
         << ((isOperateView) ? "隐藏操作页面":"显示操作页面");
    QList<QAction *> actList;
    for(int i = 0; i < list.count(); i ++){
        actList.append(new QAction(list.at(i)));
    }
    connect(actList.at(0),&QAction::triggered,this,[=](){       //暂停/开始
        isDebugView = !isDebugView;
        (isDebugView) ? (ui->debugStackedWidget->show()) : (ui->debugStackedWidget->hide());
    });
    connect(actList.at(1),&QAction::triggered,this,[=]{         //隐藏、显示操作页面
        isOperateView = !isOperateView;
        (isOperateView) ? (ui->operateStacketWidget->show()) : (ui->operateStacketWidget->hide());
    });
    menu->addActions(actList);
    menu->exec(QCursor::pos());
}

//https://blog.csdn.net/qq_38441692/article/details/105290933
//http://www.dedeyun.com/it/c/103182.html
//https://blog.csdn.net/qq_32348883/article/details/126390913
//https://blog.csdn.net/fittec/article/details/130244036



