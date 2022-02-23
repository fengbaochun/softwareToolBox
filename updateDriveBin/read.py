from PyQt5.QtCore import QThread, pyqtSignal
from ctypes import *
import time
from update import *
from agreement import *

class _VCI_INIT_CONFIG(Structure):
    _fields_ = [('AccCode', c_ulong),
                ('AccMask', c_ulong),
                ('Reserved', c_ulong),
                ('Filter', c_ubyte),
                ('Timing0', c_ubyte),
                ('Timing1', c_ubyte),
                ('Mode', c_ubyte)]

class _VCI_CAN_OBJ(Structure):
    _fields_ = [('ID', c_uint),
                ('TimeStamp', c_uint),
                ('TimeFlag', c_ubyte),
                ('SendType', c_ubyte),
                ('RemoteFlag', c_ubyte),
                ('ExternFlag', c_ubyte),
                ('DataLen', c_ubyte),
                ('Data', c_ubyte*8),
                ('Reserved', c_ubyte*3)]

class revDataThread(QThread):
    # 创建信号
    revDataSignal = pyqtSignal(int,list)
    progressSignal = pyqtSignal(int)
    logoSingal = pyqtSignal(str)
    revAnswerSingal = pyqtSignal(int)
    def __init__(self):
        super(revDataThread, self).__init__()
        self.canConfig()
        # 连接到指定的槽函数
        self.revDataSignal.connect(self.revDataSlot)
        self.enterBootStatus = False
        self.binSizeStatus = False
        self.binDataStatus = False
        self.binAnswer = False
        pass
    
    #CAN总线配置 
    def canConfig(self):
        self.canLib = windll.LoadLibrary('./ECanVci64.dll')
        self.vic = _VCI_INIT_CONFIG()
        self.vic.AccCode = 0x00000000
        self.vic.AccMask = 0xffffffff
        self.vic.Filter = 0
        self.vic.Timing0 = 0x00
        self.vic.Timing1 = 0x14   #波特率1000K
        self.vic.Mode = 0
        self.vco = _VCI_CAN_OBJ()

        self.frame = []
        self.id = 0
        pass

    # 接收数据信号槽函数
    def revDataSlot(self,id,frameData):
        otherStyleTime = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(time.time()))        
        print(otherStyleTime+" REV - ID: "+hex(id)+" Data: "+str(frameData))
        self.logoSingal.emit(otherStyleTime+" REV - ID: "+hex(id)+" Data: "+str(frameData))
        # bootloader 应答成功
        if (frameData[0] is ANSWER_CMD) and (frameData[1] is funDict['功能']['升级']) and (frameData[2] is 0xFF)and (frameData[3] is 0xFF):
            self.enterBootStatus = True
            self.logoSingal.emit("boot start successful!!!")
            print("boot start successful!!!")
        # bin size 应答成功
        elif (frameData[0] is ANSWER_CMD) and (frameData[1] is funDict['功能']['固件大小']):
            self.binSizeStatus = True
            self.logoSingal.emit("send bin size successful!!!")
            print("send bin size successful!!!")
            pass
        # 发送1KB数据 应答成功
        elif (frameData[0] is ANSWER_CMD) and (frameData[1] is funDict['功能']['固件数据']):
            self.binDataStatus = True
            self.revAnswerSingal.emit(frameData[3])         #接收到应答后发射信号
            self.progressSignal.emit(frameData[3])
            pass        
        else:
            # print(frameData)
            pass
        pass


    # 接收CAN数据线程
    def run(self):
        while True:
            num = self.canLib.GetReceiveNum(3, 0, 0)
            if num:
                flag = self.canLib.Receive(3, 0, 0, pointer(self.vco), 1, 0)
                if flag <= 0:
                    print('调用 VCI_Receive 出错\r\n')
                elif flag > 0:
                    # 信号发射
                    self.revDataSignal.emit(self.vco.ID,list(self.vco.Data))                    

revTh = revDataThread()  


