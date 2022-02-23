#include <QCoreApplication>
#include <QMap>
#include <QDebug.h>

#define CMD_BASE_ADDR    0x40	        //命令基地址

typedef enum{
    CMD_BASE = CMD_BASE_ADDR,
    READ_CMD,                           //读
    WRITE_CMD,                          //写
    ANSWER_CMD,                         //应答
    AUTO_REPORT_CMD,                    //主动上报
}cmdType;

#define FUNSION_BASE_ADDR   0x50

typedef enum{
    FUNSION_BASE = FUNSION_BASE_ADDR,   //功能码基地址
    ID_ADDR,                            //ID
    CAL_ZERO_ADDR,                      //标零
    CAL_VAL_ADDR,                       //标重
    PEEL_ADDR,                          //去皮
    CANCLE_PEEL_ADDR,                   //取消去皮
    FPS_ADDR,                           //速度设置
    WEIGHT_ADDR,                        //读重量
    REPORT_ENABLE_ADDR,                 //自动上报地址
    VERSION,							//版本查询地址

    UPDATE_BIN_ADDR,					//固件升级地址
    BIN_SIZE_ADDR,						//固件大小地址
    BIN_DATA_ADDR,						//固件数据应答地址

    SAVE_ADDR,							//保存参数

    ALL_ADDR,                           //读所有（重量、帧率）
}funsionType;

void initCanCmdMap(QMap<QString,funsionType> &cmd){
    cmd.insert("ID",  ID_ADDR);
    cmd.insert("标零",CAL_ZERO_ADDR);
    cmd.insert("标重",CAL_VAL_ADDR);
    cmd.insert("去皮",PEEL_ADDR);
    cmd.insert("取消去皮",CANCLE_PEEL_ADDR);
    cmd.insert("速度",FPS_ADDR);
    cmd.insert("重量",WEIGHT_ADDR);
    cmd.insert("上报",REPORT_ENABLE_ADDR);
    cmd.insert("版本",VERSION);
    cmd.insert("固件升级",UPDATE_BIN_ADDR);
    cmd.insert("固件大小",BIN_SIZE_ADDR);
    cmd.insert("固件应答",BIN_DATA_ADDR);
    cmd.insert("参数保存",SAVE_ADDR);
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    printf("----------------------------\r\n");

    QMap<QString,funsionType> canCmd;
    initCanCmdMap(canCmd);

    auto iter = canCmd.begin();
    while (iter != canCmd.end()){
        qDebug() << "cmd " << iter.key() << ":" << iter.value(); // 迭代器
        iter++;
    }
    printf("----------------------------\r\n");

    return a.exec();
}
