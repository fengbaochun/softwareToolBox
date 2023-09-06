#ifndef CTL_H
#define CTL_H

#include "uProtocol.h"

#define FUN_BASE_ADDR			0x30	//参数表的基地址
typedef enum{
    CTL_MODE    =   FUN_BASE_ADDR,      //控制模式
    TAR_POS,		                    //位置
    CUR_POS,
    POS_RANGE,

    TAR_SPEED,                      	//速度
    CUR_SPEED,
    SPEED_RANGE,

    TAR_CURRENT,                    	//电流
    CUR_CURRENT,
    CURRENT_RANGE,

    POLE_PAIRS,                     	//极对数
    PHASE_RES,							//相电阻
    PHASE_IND,							//相电感

    CURRENT_P,							//电流环参数
    CURRENT_I,
    CURRENT_D,

    SPEED_P,							//速度环参数
    SPEED_I,
    SPEED_D,

    POS_P,								//位置环参数
    POS_I,
    POS_D,

    REPORT_FPS,                         //上报帧率
    CLEAR_ERROR,                    	//清除错误
    CLEAR_PARAM,                    	//清除参数

    SAVE_PARAM,                     	//保存参数
    SYNC_PARAM,                     	//参数同步

    MAX_ADDR

}funsionType;

typedef enum{
    CAL_MODE,				//标定模式
    FREE_MODE,				//空闲模式 (空闲模式可以切换到其他模式)

    CURRENT_MODE,			//电流模式
    SPEED_MODE,				//速度模式
    POS_MODE,				//位置模式

    TRAJ_POS_MODE,			//轨迹位置模式

    FAULT_MODE,				//故障模式
}ctlModeTypedef;

const QMap<QString, uint8_t> ctlFun = {
    std::map<QString, uint8_t>::value_type("mode",          CTL_MODE),

    std::map<QString, uint8_t>::value_type("tarPos",        TAR_POS),
    std::map<QString, uint8_t>::value_type("curPos",        CUR_POS),
    std::map<QString, uint8_t>::value_type("posRange",      POS_RANGE),

    std::map<QString, uint8_t>::value_type("posP",          POS_P),
    std::map<QString, uint8_t>::value_type("posI",          POS_I),
    std::map<QString, uint8_t>::value_type("posD",          POS_D),

    std::map<QString, uint8_t>::value_type("tarSpeed",      TAR_SPEED),
    std::map<QString, uint8_t>::value_type("curSpeed",      CUR_SPEED),
    std::map<QString, uint8_t>::value_type("speedRange",    SPEED_RANGE),

    std::map<QString, uint8_t>::value_type("speedP",          SPEED_P),
    std::map<QString, uint8_t>::value_type("speedI",          SPEED_I),
    std::map<QString, uint8_t>::value_type("speedD",          SPEED_D),

    std::map<QString, uint8_t>::value_type("tarCurrent",    TAR_CURRENT),
    std::map<QString, uint8_t>::value_type("curCurrent",    CUR_CURRENT),
    std::map<QString, uint8_t>::value_type("currentRange",  CURRENT_RANGE),

    std::map<QString, uint8_t>::value_type("currentP",          CURRENT_P),
    std::map<QString, uint8_t>::value_type("currentI",          CURRENT_I),
    std::map<QString, uint8_t>::value_type("currentD",          CURRENT_D),

    std::map<QString, uint8_t>::value_type("polePairs",     POLE_PAIRS),
    std::map<QString, uint8_t>::value_type("phaseRes",      PHASE_RES),
    std::map<QString, uint8_t>::value_type("phaseInd",      PHASE_IND),

    std::map<QString, uint8_t>::value_type("fps",      REPORT_FPS),
    std::map<QString, uint8_t>::value_type("saveParam",      SAVE_PARAM),
};



class ctl : public uProtocol
{
    Q_OBJECT
public:
    explicit ctl(QObject *parent = nullptr);
    static ctl* instance();

    bool setFps(int timeout);                                       //设置帧率

    typedef struct{
        ctlModeTypedef mode;

        float curPos,tarPos;
        float curSpeed,tarSpeed;
        float curIq,tarIq;
        float maxPos,minPos;
        float maxSpeed,minSpeed;
        float maxIq,minIq;

        float posP, posI, posD;
        float speedP, speedI, speedD;
        float currentP, currentI, currentD;

        float temp, busVoltage;

    }motorType;

    motorType motor;
    ctlModeTypedef getMode();
    void setMode(ctlModeTypedef m);
    bool setTarPos(float val);
    bool setTarSpeed(float val);
    bool setTarCurrent(float val);

    bool setPosRange(float minVal, float maxVal);
    bool setSpeedRange(float minVal, float maxVal);
    bool setCurrentRange(float minVal, float maxVal);

    bool getPosRange(float *minVal, float *maxVal);
    bool getSpeedRange(float *minVal, float *maxVal);
    bool getCurrentRange(float *minVal, float *maxVal);

    bool setPosP(float val);
    bool setPosI(float val);
    bool setPosD(float val);

    bool setSpeedP(float val);
    bool setSpeedI(float val);
    bool setSpeedD(float val);

    bool setCurrentP(float val);
    bool setCurrentI(float val);
    bool setCurrentD(float val);

    bool paramSync();
    bool paramSave();

private slots:
    void reportCallBack(uint8_t fun, QByteArray qb);

private:
    static ctl* INSTANCE;
    QTimer *tim;

};

#endif // CTL_H
