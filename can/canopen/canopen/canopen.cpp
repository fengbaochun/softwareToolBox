#include "canopen.h"
#include <stdio.h>
#include "TestMaser.h"

CO_Data *masterData = NULL;
canopen::canopen(QObject *parent)
{
    (void)parent;
    //主站字典切换
//    masterData = &master_Data;
    masterData = &TestMaser_Data;
}

void InitNodes(CO_Data* d, UNS32 id)
{
    (void)d;
    (void)id;
    setNodeId(masterData,0x00);
    setState(masterData, Initialisation);
    setState(masterData, Operational);
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
    while(true){
        Message m{0};
        int ret = usbCanReceive(0, &m);
        if(ret>0){
            canDispatch(masterData, &m);
            QString str = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss.zzz");
            qDebug()<<str<<" id:"<<m.cob_id<<" rtr:"<<m.rtr<<" len:"<<m.len<<" data:"<<m.data[0]<<m.data[1]<<m.data[2]<<m.data[3]<<m.data[4]<<m.data[5]<<m.data[6]<<m.data[7];
        }
    }
}
