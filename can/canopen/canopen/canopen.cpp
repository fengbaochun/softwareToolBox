#include "canopen.h"
#include <stdio.h>
#include <QString>
#include <QTimer>
#include "TestMaser.h"

#include "objacces.h"

CO_Data *masterData = NULL;
canopen::canopen(QObject *parent)
{
    (void)parent;
    //主站字典切换
//    masterData = &master_Data;
    masterData = &_Data;
}

void InitNodes(CO_Data* d, UNS32 id)
{
    (void)d;
    (void)id;
    setNodeId(masterData,0x00);                 // 把节点号设置为 0x00,即主节点
    setState(masterData, Initialisation);       // 总线进入初始化模式
    setState(masterData,Pre_operational);       // 总线进入预操作模式
    setState(masterData, Operational);          // 总线进入操作模式,(执行后,会发送 PDO)

    masterSendNMTstateChange (masterData, 0x02, NMT_Start_Node);

//    UNS8 count = 1;
//    masterData->ConsumerHeartbeatCount = &count;					//节点数量
//    int16_t time = 1000;											//1000ms一次
//    masterData->ConsumerHeartbeatEntries[0] = time;                //变量的0-16位为时间检测间隔:1000ms
//    masterData->ConsumerHeartbeatEntries[0] |= 0x02 << 16; 			//变量的17-20位为ID 这里假设节点id为0x01

//    QTimer timer;
//    QObject::connect(&timer,&QTimer::timeout,[](){

//        if (masterData->NMTable[0x02] == Operational)
//            printf("节点在工作\r\n");
//        if (masterData->NMTable[0x02] == Disconnected)
//            printf("节点已经离线\r\n");
//       printf("当前状态%x\r\n", masterData->NMTable[0x01]);

//    });
//    timer.start();
}

void Exit(CO_Data* d, UNS32 id)
{
    (void)d;
    (void)id;
//    masterSendNMTstateChange (&master_Data, 0x01, NMT_Stop_Node);
    setState(masterData, Stopped);
}

void canopen::init()
{
    TimerInit();
    StartTimerLoop(&InitNodes);
}

void canopen::readDevType()
{
    UNS32 data = 0;

    writeNetworkDict(masterData,0x02,0x1000,0x00,0,uint32,&data,0);
    qDebug()<<"data "<<data;


//    getODentry(masterData,0x1280,0x03,&data,(UNS32 *)sizeof(data),(UNS8 * )uint32,0);
//    qDebug()<<"data "<<data;

//    if(ret == OD_SUCCESSFUL){
//        qDebug()<<"read success :data 0x%04X"<<data;
//    }else{
//        qDebug()<<"read failed : ret 0x%04X"<<ret;
//    }

//    unsigned char get_test_datasend[8]={0x40,0x00,0x20,0x00,0x00,0x00,0x00,0x00};
//    sendSDO(masterData,SDO_CLIENT,0,get_test_datasend);
}

void canopen::restNode()
{
    masterSendNMTstateChange(masterData, 0x02, NMT_Reset_Node);
}

#if 0
//void canopen::start()
//{
//    TimerInit();
//    StartTimerLoop(&InitNodes);
//}

//void canopen::stop()
//{
//    StopTimerLoop(&Exit);
//}

#endif

void canopen::run()
{
    if(canOpen((s_BOARD * )1, masterData) != NULL){
        qDebug()<<"open success !!!";
    }
    init();
    while(true){
        Message m{0};
        int ret = usbCanReceive(0, &m);
        if(ret>0){
            canDispatch(masterData, &m);
            QString str = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss.zzz");
            qDebug()<<str<<" id:"<<QString().sprintf("0x%04X",m.cob_id)<<" rtr:"<<m.rtr<<" len:"<<m.len<<" data:"<<m.data[0]<<m.data[1]<<m.data[2]<<m.data[3]<<m.data[4]<<m.data[5]<<m.data[6]<<m.data[7];
        }
    }
}
