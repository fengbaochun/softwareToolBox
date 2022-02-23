import sys
import os
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtCore import QThread, pyqtBoundSignal, pyqtSignal
from PyQt5.QtCore import QTimer
from can import CANalyser
from read import *
from ctypes import *

import binascii
import time
from agreement import *

canControl = CANalyser()

class updateThreadClass(QThread):
    logoSingal = pyqtSignal(str)
    def __init__(self):
        super(updateThreadClass, self).__init__()
        # 需要更新的ID
        self.ID = 0xB8
        self.idList = [0xA1,0xA2,0xA3,0xB1,0xB2,0xB3,0xB8,0xA8]
        self.path = "./forceAcqu.bin"
        self.finashPkgNum = 0                   #发送完成的包数
        self.perPkgSize = 1024
    # 获取固件size
    def getBinSize(self,path):
        self.binSize = os.path.getsize(path)
        self.packNum = self.binSize // 1024
        self.lastPackSize = self.binSize % 1024
        
        # 计算总包数，用于进度更新
        if self.binSize%1024 > 0:
            self.totalPackNum = self.binSize//1024 + 1
        else:
            self.totalPackNum = self.binSize//1024
        
        print("固件大小:"+str(self.binSize))
        print("总 包 数:"+str(self.packNum))
        print("余字节数:"+str(self.lastPackSize))   

        pass

    # 获取板子ID和固件路径等信息
    def getBoardInfo(self,index,path):
        self.ID = self.idList[index]
        self.path = path
        print("id:" + str(self.ID))
        print("path:" + str(self.path))
        pass

    def readBin(self,path):
        self.binfile = open(path, 'rb') #打开二进制文件
        size = 8
        index = 1
        # 整包
        smallPkgNum = self.binSize // 8         #计算共有多少can帧（每帧8字节）
        for i in range((smallPkgNum)):      
            revData = list(self.binfile.read(size)) 
            # print(i+1)
            data = (c_ubyte * 8)(revData[0],revData[1],revData[2],revData[3],revData[4],revData[5],revData[6],revData[7])
            canControl.transmit(id=self.ID, send_type=1, len=8, InputData=data)  
            
            if i>1:
                if (((i+1)*size % 1024) is 0):
                    print("index : "+str(index))
                    while self.finashPkgNum is not index:           #每1KB都要等待应答
                        
                        pass                                        
                    index = index+1
                    # time.sleep(0.08)                 

        # time.sleep(0.05)
        
        # 余包
        if self.lastPackSize>0:
            revData = list(self.binfile.read(4)) 
            data = (c_ubyte * 8)(revData[0],revData[1],revData[2],revData[3],0x00,0x00,0x00,0x00)
            canControl.transmit(id=self.ID, send_type=1, len=8, InputData=data)  
            while self.finashPkgNum is not index:                   #每1KB都要等待应答

                pass   
            
            # time.sleep(0.08) 


        print("固件大小:"+str(self.binSize))
        print("包    数:"+str(self.packNum))
        print("余字节数:"+str(self.lastPackSize))   
        
        # 清除标志，支持二次升级
        self.binfile.seek(0)

        revTh.enterBootStatus = False
        revTh.binSizeStatus = False
        revTh.binDataStatus = False
            
        pass

    # 功能设置
    def setFun(self,key,val):
        data = (c_ubyte * 8)(WERITE_CMD,funDict['功能'][key],(val>>8)&0xff,val&0xff,0x00,0x00,0x00,0x00)
        canControl.transmit(id=self.ID, send_type=1, len=8, InputData=data)
        pass
    
    # 发送固件大小
    def sendBinSize(self):
        
        data = (c_ubyte * 8)(WERITE_CMD,funDict['功能']['固件大小'],(self.binSize>>8)&0xff,self.binSize&0xff,0x00,0x00,0x00,0x00)
        canControl.transmit(id=self.ID, send_type=1, len=8, InputData=data)
        
        print("data[2]："+str(self.binSize&0xff))
        print("data[3]："+str((self.binSize>>8)&0xff))
        
        print("driveApp bin size : "+str(self.binSize))
        print(list(data))
        
        while not revTh.binSizeStatus:
            pass
        
        pass

    # 进入bootloader
    def enterBootloader(self,status):
        if status is True:
            data = (c_ubyte * 8)(WERITE_CMD,funDict['功能']['升级'],0xFF,0xFF,0x00,0x00,0x00,0x00)
            canControl.transmit(id=self.ID, send_type=1, len=8, InputData=data)
            
            while not revTh.enterBootStatus:
                pass

            pass
        else:
            print("Please enable enterBootloader")
            self.logoSingal.emit("Please enable enterBootloader")
        pass
            
    # 固件更新回调
    def updateBinCallBack(self,pkgIndex):
        self.finashPkgNum = pkgIndex
        pass
    
    def run(self):
        self.setFun('上报使能',0)
        time.sleep(1)
        # 进入boot
        self.enterBootloader(True)

        # 发送固件size
        self.sendBinSize()
        # 读取固件内容并打印
        self.readBin(self.path)

        while True:
            pass
 
        pass

updateTh = updateThreadClass()

    

