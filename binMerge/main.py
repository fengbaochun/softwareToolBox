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
from gengrate import *

class MyMainWindow(QMainWindow, Ui_MainWindow):
    def __init__(self,parent = None):
        super(MyMainWindow, self).__init__(parent)
        self.setupUi(self)

        self.setWindowTitle("固件合成 V2.0")   
        self.alignBinText.setText(binInfo['bin0']['path'])

        self.creatBinBtn.clicked.connect(self.creatBinCallBack)
        self.driveBinChoseBtn.clicked.connect(self.driveBinChoseCallBack)
        self.mainBinChoseBtn.clicked.connect(self.mainBinChoseCallBack)
        self.forceBinChoseBtn.clicked.connect(self.forceBinChoseCallBack)

        self.mainBinMaxSizeText.setText("400")
        self.mainBinVerText.setText("0.0")
        self.driveBinMaxSizeText.setText("40")
        self.driveBinVerText.setText("1.8")
        self.forceBinMaxSizeText.setText("30")
        self.forceBinVerText.setText("1.0")
        
        pass

    def binInfoUpdate(self):

        binInfo["bin1"]["path"] = self.mainBinPath
        binInfo["bin1"]["maxSize"] = int(self.mainBinMaxSizeText.toPlainText())*1024
        binInfo["bin1"]["version"] = self.mainBinVerText.toPlainText()
        print(binInfo["bin1"])

        binInfo["bin2"]["path"] = self.driveBinPath
        binInfo["bin2"]["maxSize"] = int(self.driveBinMaxSizeText.toPlainText())*1024
        binInfo["bin2"]["version"] = self.driveBinVerText.toPlainText()
        print(binInfo["bin2"])

        binInfo["bin3"]["path"] = self.forceBinPath
        binInfo["bin3"]["maxSize"] = int(self.forceBinMaxSizeText.toPlainText())*1024
        binInfo["bin3"]["version"] = self.forceBinVerText.toPlainText()
        print(binInfo["bin3"])

        binInfo['bin0']['path'] = self.alignBinText.toPlainText()
        pass

    def forceBinChoseCallBack(self):
        self.forceBinPath, ok = QFileDialog.getOpenFileName(self,
                                    "Chose bin",
                                   "./",
                                    "Bin Files (*.bin)")   
        if ok:
            self.forceBinPathText.setText(str(self.forceBinPath))                                                       
        pass

    def driveBinChoseCallBack(self):
        self.driveBinPath, ok = QFileDialog.getOpenFileName(self,
                                    "Chose bin",
                                   "./",
                                    "Bin Files (*.bin)")   
        if ok:
            self.driveBinPathText.setText(str(self.driveBinPath))                                                       
        pass

    def mainBinChoseCallBack(self):
        self.mainBinPath, ok = QFileDialog.getOpenFileName(self,
                                    "Chose bin",
                                   "./",
                                    "Bin Files (*.bin)")   
        if ok:
            self.mainBinPathText.setText(str(self.mainBinPath))                                                          
        pass        
        
    def creatBinCallBack(self):
        print("creatBinCallBack")
        try:
            # 允许更新条件
            if True:
                self.binInfoUpdate()
                gengrate()
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



