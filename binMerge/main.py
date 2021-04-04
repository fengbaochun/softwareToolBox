import sys
from Ui_mergeBin import Ui_MainWindow
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from struct import *
import os
import time
import binascii
import struct
import shutil

class MyMainWindow(QMainWindow, Ui_MainWindow):
    def __init__(self,parent = None):
        super(MyMainWindow, self).__init__(parent)
        self.setupUi(self)

        self.setWindowTitle("固件合成")   
        self.alignBinText.setText("400KB - v1.4")
        # 合成的文件路径
        self.newFilePath = "./A9D Plus App.bin"

        self.maxSize = 400*1024

        self.creatBinBtn.clicked.connect(self.creatBinCallBack)
        self.driveBinChoseBtn.clicked.connect(self.driveBinChoseCallBack)
        self.mainBinChoseBtn.clicked.connect(self.mainBinChoseCallBack)
        pass

    def driveBinChoseCallBack(self):
        print("driveBinChoseCallBack")

        self.driveBinPath, ok = QFileDialog.getOpenFileName(self,
                                    "Chose bin",
                                   "./",
                                    "Bin Files (*.bin)")   

        if ok:
            self.driveBinPathText.setText(str(self.driveBinPath))                                                       
            if os.path.getsize(self.driveBinPath) <= (1024*40):
                pass  
            else:
                QMessageBox.critical(self, "Bin size error", "Please select the correct drive firmware!!!", QMessageBox.Yes , QMessageBox.Yes)

        pass

    def mainBinChoseCallBack(self):
        print("mainBinChoseCallBack")

        self.mainBinPath, ok = QFileDialog.getOpenFileName(self,
                                    "Chose bin",
                                   "./",
                                    "Bin Files (*.bin)")   

        if ok:
            self.mainBinPathText.setText(str(self.mainBinPath))                                                          
            if os.path.getsize(self.mainBinPath) <= self.maxSize:  
                pass
            else:
                QMessageBox.critical(self, "Bin size error", "Please select the correct main board firmware!!!", QMessageBox.Yes , QMessageBox.Yes)
        pass        
        

    def creatBinCallBack(self):
        print("creatBinCallBack")

        try:

            self.srcBinSize = os.path.getsize(self.mainBinPath)
            print("主板固件大小:"+str(self.srcBinSize))
            self.driveBinSize = os.path.getsize(self.driveBinPath)
            print("驱动固件大小:"+str(self.driveBinSize))   

            # 获取字符串内容
            binInfo = self.alignBinText.toPlainText()

            # 版本号分割
            self.mainVer = int(binInfo.split(' - v')[1].split('.')[0])
            self.subVer = int(binInfo.split(' - v')[1].split('.')[1])

            print(self.mainVer,self.subVer)
            # 合成固件的最大字节数
            self.maxSize = int(binInfo.split('KB - ')[0])*1024   
            
            print("max size : " + str(self.maxSize))
            print("src bin size : " + str(self.srcBinSize))
            
            # 允许更新条件
            if self.srcBinSize<self.maxSize:
                # 新建bin文件
                newBinFile = open(self.newFilePath,'wb+')
                # 先写入主板固件
                srcBinFile = open(self.mainBinPath, 'rb') 
                newBinFile.write(srcBinFile.read())

                # 补齐主控固件到400KB
                for i in range(self.maxSize-self.srcBinSize):   
                    data = struct.pack('B',0xFF)  #写1
                    newBinFile.write(data)
                
                newBinFile.flush()
                self.newFileSize = os.path.getsize(self.newFilePath)
                print("已补齐")
                print("补齐固件大小:"+str(self.newFileSize))

                # 将驱动大小写入到主控末尾 (共16个字节，中间两个字节为驱动固件大小，其余字节为0x00)
                data = struct.pack('B',0x00)
                for i in range(7):
                    newBinFile.write(data)
                
                # 固件大小
                data = struct.pack('B',self.driveBinSize&0xff)
                newBinFile.write(data)
                data = struct.pack('B',(self.driveBinSize>>8)&0xff)
                newBinFile.write(data)
                
                # 版本号
                data = struct.pack('B',self.mainVer)
                newBinFile.write(data)
                data = struct.pack('B',self.subVer)
                newBinFile.write(data)

                data = struct.pack('B',0x00)
                for i in range(5):
                    newBinFile.write(data)          

                newBinFile.flush()

                # 将驱动固件写入到驱动大小末尾
                driveBin = open(self.driveBinPath,'rb')
                newBinFile.write(driveBin.read())
                newBinFile.flush()

                self.newFileSize = os.path.getsize(self.newFilePath)
                print("合成固件大小："+str(self.newFileSize))
                
                srcBinFile.close()
                newBinFile.close()
                driveBin.close()
                QMessageBox.about(self, "success", "creat ok!!!")
            else:
                QMessageBox.critical(self, "error", "Please select the correct firmware!!!", QMessageBox.Yes , QMessageBox.Yes)
            pass   

        except :
            QMessageBox.critical(self, "Select path error", "Please select the correct path!!!", QMessageBox.Yes , QMessageBox.Yes)
 
    


if __name__ =='__main__':
    app = QApplication(sys.argv)
    myWin = MyMainWindow()
    myWin.setFixedSize(myWin.width(), myWin.height())
    myWin.show()
    sys.exit(app.exec_())



