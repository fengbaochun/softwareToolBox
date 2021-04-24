import sys
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QApplication, QMainWindow, QMessageBox,QInputDialog,QFileDialog
from PyQt5.QtCore import QTimer
from PyQt5.QtGui import QIcon

from Ui_mainwindow import Ui_MainWindow  
from Serial_tool import *

from struct import *
import binascii
import os

class MyMainWindow(QMainWindow, Ui_MainWindow):
    def __init__(self,parent = None):
        super(MyMainWindow, self).__init__(parent)
        self.setupUi(self)

        self.setWindowTitle("Update Tool")

        self.com = [] 
        for name in Com_dev.port_list:
            self.com.append(str(name).split("-")[0])
        
        self.Box_com.addItems(self.com)

        bps = ["9600", "115200"]
        self.Box_bps.addItems(bps)
        self.Box_bps.setCurrentIndex(1)

        way = ["only_uart","dex_arm"]
        self.way_box.addItems(way)
        self.way_box.setCurrentIndex(1)

        self.cur_progressBar = 0
        self.pack_size = 500

        self.Button_opencom.clicked.connect(self.on_open_com)
        self.Button_refresh.clicked.connect(self.refresh_port)
        self.chose_bin_Button.clicked.connect(self.Chose_model_bin)
        self.Button_start.clicked.connect(self.Start_update)

    def on_open_com(self):
        global Com_dev
        if self.Button_opencom.text() == "Open":
            self.Button_opencom.setText("Close")
            com_x = str(self.Box_com.currentText()) 
            bps_x = str(self.Box_bps.currentText())
 
            Com_dev.set_com(com_x)
            Com_dev.set_bps(bps_x)

            try:
                status = Com_dev.open()           
                
                if status == False:
                    self.Button_opencom.setText("Open")
                    QMessageBox.question(self, "Open error", "The serial port is occupied or does not exist!!!", QMessageBox.Yes , QMessageBox.Yes)                                    
                    print("open fail")
                    
                    self.log_text.append("open fail")
                    return 
                else:
                    self.log_text.append("open success")
                    
                
                pass
            except:
                self.Button_opencom.setText("Open")
                QMessageBox.question(self, "Open error", "The serial port is occupied or does not exist!!!", QMessageBox.Yes , QMessageBox.Yes)                
                print("open fail")
                self.log_text.append("open fail")
                return 
        else:
            Com_dev.close()
            self.Button_opencom.setText("Open")
            self.log_text.append("open close")
        pass


    def refresh_port(self):
        Com_dev.scan()
        self.Box_com.clear()
        for i in range(0,len(Com_dev.port_list)):
            # print(str(Com_dev.port_list[i]).split("-")[0])
            self.Box_com.insertItem(i,str(Com_dev.port_list[i]).split("-")[0])
        pass

    def Chose_model_bin(self):
        # https://www.cnblogs.com/linyfeng/p/11223711.html
        # get_directory_path = QFileDialog.getExistingDirectory(self,
        #                             "Chose bin",
        #                             "./")      

        self.bin_path, ok = QFileDialog.getOpenFileName(self,
                                    "Chose bin",
                                   "./",
                                    "All Files (*);;Text Files (*.bin)")   
        if ok:
            self.directory_text.setText(str(self.bin_path))                                                      
            print(str(self.bin_path))       

            if os.path.getsize(self.bin_path) <= (1024*17):
                self.log_text.append("chose bin ok !!!")
                self.log_text.append("bin_path :"+str(self.bin_path))    
            else:
                QMessageBox.question(self, "Bin size error", "Please select the correct firmware!!!", QMessageBox.Yes , QMessageBox.Yes)


        
        pass

    def usart_update_bin(self):
        pack_size = self.pack_size
        file = open(self.bin_path, 'rb')
        bin_size = os.path.getsize(self.bin_path)

        pack_num = bin_size  // pack_size
        last_num = bin_size  % pack_size

        self.cur_progressBar = pack_num + 1

        print("固件大小:"+str(bin_size))
        print("单 包 数:"+str(pack_size))
        print("包    数:"+str(pack_num))
        print("余字节数:"+str(last_num))
        self.log_text.append("bin size :"+str(bin_size))
        # 进入boot
        enter_cmd_str = "FFFFEFFF"
        print(enter_cmd_str+" 已发送")
        self.log_text.append("enter boot")
        Com_dev.only_send_hex(enter_cmd_str)
        time.sleep(0.5)

        # 向设备发送固件大小
        bin_size_str = "<"+str(bin_size)+">"
        print(bin_size_str+" 已发送")
        
        Com_dev.send(bin_size_str)
        self.log_text.append("send bin size")
        self.log_text.append("start send bin")
        # 整包
        for i in range(pack_num):    
            bin_data = file.read(pack_size)
            bin_data_16 = str(binascii.b2a_hex(bin_data))[2:-1]
            print("pack "+str(i)+": "+bin_data_16+" \n")

            Com_dev.send_hex(bin_data_16)
            self.progressBar.setValue((i*100)/self.cur_progressBar)
            self.log_text.append("send data ...")

        # 余包
        if last_num>0:
            bin_data = file.read(last_num)    
            bin_data_16 = str(binascii.b2a_hex(bin_data))[2:-1]
            print("pack "+str(i)+": "+bin_data_16+" \n")

            Com_dev.send_hex(bin_data_16)   
            self.log_text.append("send data ...")

            self.progressBar.setValue(100)

        # 余包
        # file.close()
        print("update finash")
        self.log_text.append("update finash")

        pass

    # 机械臂 升级 旋转模块
    def dexarm_update_ratation_model(self):

        one_buf_size = 50

        file = open(self.bin_path, 'rb')
        bin_size = os.path.getsize(self.bin_path)
        
        pack_num = bin_size // one_buf_size
        last_num = bin_size % one_buf_size

        self.cur_progressBar = pack_num + 1

        # 初始化
        # ser.write("M2100\r\n".encode())
        # read_dev_answer() 
        Com_dev.send("M2100\r\n")
        # 进入boot
        # ser.write("M2102U1\r\n".encode())
        # read_dev_answer() 
        Com_dev.send("M2102U1\r\n")

        # 发送固件大小
        bin_size_str_cmd = "M2102U2S"+str(bin_size)+"\r\n"
        # ser.write(bin_size_str_cmd.encode())
        # read_dev_answer() 
        Com_dev.send(bin_size_str_cmd)


        # 整包
        for i in range(pack_num):
            # 读取
            bin_data = file.read(one_buf_size)
            # 转换+.
            bin_data_16 = str(binascii.b2a_hex(bin_data))[2:-1]
            str1 = "M2102U3 <"+bin_data_16+">\r\n" #数据头为 gcode 指令 
            print("---------SEND---:"+str1)
            # 字符写入
            # ser.write(str1.encode())
            # read_dev_answer() 
            Com_dev.send(str1)

            print("-"+str(i)+"-")
            self.progressBar.setValue((i*100)/self.cur_progressBar)
            self.log_text.append("send data ...")            

        if last_num>0:
            print("last pack")
            # 余包
            # 读取
            bin_data = file.read(last_num)
            # 转换
            bin_data_16 = str(binascii.b2a_hex(bin_data))[2:-1]
            str1 = "M2102U3 <"+bin_data_16+">\r\n" #数据头为 gcode 指令 
            print("---------SEND---:"+str1)
            # 字符写入
            # ser.write(str1.encode())
            # read_dev_answer() 
            Com_dev.send(str1)
            self.log_text.append("send data ...")
            self.progressBar.setValue(100)
            self.log_text.append("update finash")

        print("bin_size = "+str(bin_size+11)+" transmission ok!!!")
        pass

    def Start_update(self):
        if self.Button_opencom.text() == "Open":
            QMessageBox.question(self, "Open error", "The serial port is occupied or does not exist!!!", QMessageBox.Yes , QMessageBox.Yes)
        else:
            if self.way_box.currentText() == "only_uart":
                
                print("start update")
                self.Button_start.setText("update")
                self.usart_update_bin()
                QMessageBox.question(self, "OK", "update successed", QMessageBox.Yes , QMessageBox.Yes)   

            elif self.way_box.currentText() == "dex_arm":
                self.Button_start.setText("update")
                self.dexarm_update_ratation_model()
                QMessageBox.question(self, "OK", "update successed", QMessageBox.Yes , QMessageBox.Yes)   

                pass


            self.progressBar.setValue(0)

        pass

    def update_progressBar(self):
        
        pass


    pass


if __name__ =='__main__':
    app = QApplication(sys.argv)
    myWin = MyMainWindow()
    myWin.setFixedSize(myWin.width(), myWin.height())
    myWin.show()
    sys.exit(app.exec_())

# https://blog.csdn.net/xyisv/article/details/88292870    
