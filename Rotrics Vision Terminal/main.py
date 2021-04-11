import sys
from PyQt5.QtWidgets import QApplication,QMainWindow,QMessageBox
from PyQt5.QtCore import QTimer
from PyQt5.QtGui import QIcon

from Ui_mainwindow import Ui_MainWindow  
from find_color_block import Find_color_block  
from Tool_box.Serial_tool import *
from Cam_dev import *
from find_color_block import *
from PyQt5 import sip



class MyMainWindow(QMainWindow, Ui_MainWindow):
    def __init__(self,parent = None):
        super(MyMainWindow, self).__init__(parent)
        self.setupUi(self)

        self.setWindowTitle("Rotrics Vision Terminal")

        self.com = [] 
        for name in Com_dev.port_list:
            self.com.append(str(name).split("-")[0])
        
        self.Box_com.addItems(self.com)

        bps = ["9600", "115200"]
        self.Box_bps.addItems(bps)
        self.Box_bps.setCurrentIndex(1)

        stop_bit = ["1","1.5","2"]
        self.Box_stop.addItems(stop_bit)
        self.Box_stop.setCurrentIndex(0)

        cam_num = video.scan()
        print("camera status"+str(video.status))

        self.Box_cam.addItems(cam_num)
        self.Box_cam.setCurrentIndex(0)

        self.Button_opencom.clicked.connect(self.on_open_com)
        self.Button_refresh.clicked.connect(self.refresh_port)

        self.Button_open_cam.clicked.connect(self.on_open_cam)        
        self.Button_cam_refresh.clicked.connect(self.cam_refresh_port) 

        table_1 = Find_color_block() 
        self.tabWidget.insertTab(0,table_1,"Color block recognition")
        self.tabWidget.setCurrentWidget(table_1)

    
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
                
                Com_dev.send(gcode.init())
                Com_dev.send(gcode.home())                
                
                if status == False:
                    self.Button_opencom.setText("Open")
                    QMessageBox.question(self, "Open error", "The serial port is occupied or does not exist!!!", QMessageBox.Yes , QMessageBox.Yes)                                    
                    print("open fail")
                    return 
                
                # Com_dev.send("M1111\r\n")   
                pass
            except:
                self.Button_opencom.setText("Open")
                QMessageBox.question(self, "Open error", "The serial port is occupied or does not exist!!!", QMessageBox.Yes , QMessageBox.Yes)                
                print("open fail")
                return 
        else:
            # Com_dev.send("G0X280Y0Z0\r\n")  
            Com_dev.close()
            self.Button_opencom.setText("Open")
        pass

    def on_open_cam(self):
        if self.Button_open_cam.text() == "Open":

            cam_x = str(self.Box_cam.currentText()) 

            try:
                # video.open(int(cam_x),640,480)
                video.open(int(cam_x),800,600)
                print("camera already opened")
                self.Button_open_cam.setText("Close")
                pass
            except:
                QMessageBox.question(self, "Open error", "Please insert the camera", QMessageBox.Yes , QMessageBox.Yes)                
                print("open fail")
                pass             
        else:
            video.close()
            print("camera already closed")
            print(Cam_dev.status)
            self.Button_open_cam.setText("Open")
        pass


    def refresh_port(self):
        Com_dev.scan()
        self.Box_com.clear()
        for i in range(0,len(Com_dev.port_list)):
            # print(str(Com_dev.port_list[i]).split("-")[0])
            self.Box_com.insertItem(i,str(Com_dev.port_list[i]).split("-")[0])
        pass

    def cam_refresh_port(self):
        video.scan()
        self.Box_cam.clear()
        for i in range(0,len(video.dev_list)):
            self.Box_cam.insertItem(i,str(video.dev_list[i]))
        pass


    pass


if __name__ =='__main__':
    app = QApplication(sys.argv)
    myWin = MyMainWindow()
    myWin.setWindowIcon(QIcon('./img/logo.ico'))
    myWin.setFixedSize(myWin.width(), myWin.height())    
    myWin.show()
    sys.exit(app.exec_())
