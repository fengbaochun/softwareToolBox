#include "canopen.h"
#include <stdio.h>
#include "TestMaser.h"

canopen::canopen(QObject *parent)
{

}

void InitNodes(CO_Data* d, UNS32 id)
{
    setNodeId(&master_Data,0x00);
    setState(&master_Data, Initialisation);
    setState(&master_Data, Operational);
}

void Exit(CO_Data* d, UNS32 id)
{
//    masterSendNMTstateChange (&master_Data, 0x01, NMT_Stop_Node);
    setState(&master_Data, Stopped);
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
    if(canOpen((s_BOARD * )1, &master_Data) != NULL){
        qDebug()<<"open success !!!";
    }
    while(true){
        Message m{0};
        int ret = usbCanReceive(0, &m);
        if(ret>0){
            canDispatch(&master_Data, &m);
            QString str = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss.zzz");
            qDebug()<<str<<" id:"<<m.cob_id<<" rtr:"<<m.rtr<<" len:"<<m.len<<" data:"<<m.data[0]<<m.data[1]<<m.data[2]<<m.data[3]<<m.data[4]<<m.data[5]<<m.data[6]<<m.data[7];
        }
    }
}
