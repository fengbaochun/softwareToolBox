import sys
import os
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtCore import QThread, pyqtSignal
from PyQt5.QtWidgets import QApplication, QMainWindow, QMessageBox,QInputDialog,QFileDialog
from PyQt5.QtCore import QTimer
from PyQt5.QtGui import QIcon

from Ui_update_UI import Ui_MainWindow
from update import *
from read import *
from ctypes import *
import binascii
import time

class MyMainWindow(QMainWindow, Ui_MainWindow):
    progressFinashSignal = pyqtSignal()
    logoSingal = pyqtSignal(str)
    def __init__(self,parent = None):
        super(MyMainWindow, self).__init__(parent)
        self.setupUi(self)

        self.setWindowTitle("bin tool")   
        # 设置驱动板ID  
        self.devID = ["A1", "A2", "A3","B1", "B2", "B3","B8","A8"]
        self.devNum.addItems(self.devID)
        self.devNum.setCurrentIndex(6)

        # 显示驱动板软件版本
        self.verNum.append(str(1.0))

        self.openBtn.clicked.connect(self.openDevCallback)
        self.choseBinBtn.clicked.connect(self.choseBinCallback)
        self.minPosBtn.clicked.connect(self.minPosCallback)
        self.maxPosBtn.clicked.connect(self.maxPosCallback)
        self.updateStart.clicked.connect(self.startUpdateBinCallBack)

        # 信号及槽函数
        revTh.progressSignal.connect(self.progressCallback)
        revTh.logoSingal.connect(self.logoCallback)
        revTh.revAnswerSingal.connect(updateTh.updateBinCallBack)
        self.logoSingal.connect(self.logoCallback)
        updateTh.logoSingal.connect(self.logoCallback)

        self.progressFinashSignal.connect(self.updateFinashCallBack)

    def openDevCallback(self):
        if self.openBtn.text() == "打开":
            try:
                index = self.devNum.currentIndex()
                print(self.binPath)
                # 初始化CAN设备
                canControl.opendevice()
                canControl.initdevice()
                canControl.startcan()
                canControl.clearcan()
                # 启动接收数据线程
                revTh.start()
                # 获取id & 路径
                updateTh.getBoardInfo(index, self.binPath)
                self.logoSingal.emit("dev opened")
                self.openBtn.setText("关闭")
                pass
            except:
                self.openBtn.setText("打开")
                QMessageBox.critical(self, "info error", "Please select the correct info!!!", QMessageBox.Yes , QMessageBox.Yes)
                return 

        elif self.openBtn.text() == "关闭":
            self.logoSingal.emit("dev closed")
            canControl.closedevice()
            self.openBtn.setText("打开")
        pass

    def choseBinCallback(self):
        print("choseBinCallback")
        # https://www.cnblogs.com/linyfeng/p/11223711.html
        # get_directory_path = QFileDialog.getExistingDirectory(self,
        #                             "Chose bin",
        #                             "./")      
        self.binPath, ok = QFileDialog.getOpenFileName(self,
                                    "Chose bin",
                                   "./",
                                    "Bin Files (*.bin)")   

        if ok:
            self.directory_text.setText(str(self.binPath))                                                      
            updateTh.getBinSize(self.binPath)   

            if os.path.getsize(self.binPath) <= (1024*30):
                self.logText.append("chose bin ok !!!")
                self.logText.append("binPath :"+str(self.binPath))    
            else:
                QMessageBox.critical(self, "Bin size error", "Please select the correct firmware!!!", QMessageBox.Yes , QMessageBox.Yes)
                # https://blog.csdn.net/jia666666/article/details/81540785

        pass

    def upodateCallback(self):
        print("upodateCallback")
        pass

    def minPosCallback(self):
        updateTh.setFun('上报使能',1)
        print("minPosCallback")
        pass
    
    def maxPosCallback(self):
        updateTh.setFun('上报使能',0)
        print("maxPosCallback")
        pass
    
    # 加载logo
    def logoCallback(self,logoData):
        self.logText.append(logoData)
        pass

    # 更新完成
    def updateFinashCallBack(self):
        QMessageBox.information(self, "tips", "update finashed!!!", QMessageBox.Yes , QMessageBox.Yes)
        pass

    # 更新进度条
    def progressCallback(self,num):
        self.progressBar.setValue(int((num+1)*(100.0/float(updateTh.totalPackNum))))
        if num is updateTh.totalPackNum:
            self.logoSingal.emit("update succeed!!!")
            self.progressFinashSignal.emit()
            pass
        pass
    
    # 启动更新线程
    def startUpdateBinCallBack(self):
        updateTh.start()
        print("startUpdateBinCallBack")
        pass

def main():
    app = QApplication(sys.argv)
    myWin = MyMainWindow()
    myWin.setFixedSize(myWin.width(), myWin.height())
    myWin.show()
    sys.exit(app.exec_())
    pass

if __name__ == "__main__":
    main()
