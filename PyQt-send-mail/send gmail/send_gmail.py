
import sys
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QApplication, QMainWindow, QMessageBox,QInputDialog,QFileDialog
from PyQt5.QtCore import QTimer
from PyQt5.QtGui import QIcon

from Ui_mainwindow import Ui_MainWindow
from struct import *
import os
import yagmail
import xlrd
import time

# 模板

title_str_temple = "Rotrics ****  **** Tracking Number"
str_info_temple=    '''
    Hey, + name +，<p>

    Good day. Thanks so much for supporting our Rotrics Arm!<p>

    We have shipped out your rewards with the rest of European orders via "Eurasian railway +local UPS" delivery. The train will arrive at destination stations (Poland and Germany) before the end of July, and customs clearance processing might take 7-10 business days. Local UPS delivery will take 3-5 business days. <strong>So you can expect the courier knock at your door from August 13th.</strong>

    <br>Tracking website: <strong>https://www.ups.com/track</strong><br>

    <strong>Kindly notice that the tracking data won't update until your package starts local delivery.</strong>

    <br>Besides, the UPS has canceled Signature Confirmation Service because of COVID-19, so the courier might not knock your door or require your signature to confirm the delivery. Please do pay attention to the delivery status and make sure you get the package.<br><p>

    For those who have purchased our laser module, we want to apologize again that we have canceled the enclosure from the rewards because of the limited budget. We have replaced it with a pair of safety goggles, if you still need the safety enclosure, please purchase from our website: <strong>https://bit.ly/2AKP2L7</strong><br>

    <br>Thanks again for your support and patience! Looking forward to your feedback!<br>

    <p>Yours Sincerely,<p>
    <p>Rotrics Team。<p>
                    
    '''



class MyMainWindow(QMainWindow, Ui_MainWindow):
    def __init__(self,parent = None):
        super(MyMainWindow, self).__init__(parent)
        self.setupUi(self)

        self.setWindowTitle("邮件批量发送")

        self.open_xlsx_status = False

        account = ["fulfillment@rotrics.com","fengbaochun@rotrics.com", "115200"]
        self.textEdit_account_2.append(account[0])

        password = ["nY7Kd7gqm9iERevW","Fbc2483@","dex_arm"]
        self.textEdit_password_2.append(password[0])

        servername = 'smtp.exmail.qq.com'
        self.Box_server_2.addItem(servername)

        # self.plainTextEdit_gmail.toPlainText(str_info_temple)
        self.plainTextEdit_gmail.appendPlainText(str_info_temple)
        self.textEdit_title.append(title_str_temple)

        self.cur_progressBar = 0
        self.progressBar_2.setValue(self.cur_progressBar)

        # 槽函数
        self.Button_chose.clicked.connect(self.Chose_xlsx)
        self.pushButton_login.clicked.connect(self.log_in_callback)
        self.pushButton_check.clicked.connect(self.check_xlsx_callback)
        self.pushButton_send_2.clicked.connect(self.send_xlsx_callback)



    def check_xlsx_callback(self):
        
        if self.open_xlsx_status:
            self.log_text.append("开始校检，并打印")  
            self.get_exel_data_and_check(self.Data_sheet)   
            pass
        else:
            QMessageBox.question(self, "校检 错误", "请先选择 *.xlsx 文件后，再点击 此按钮", QMessageBox.Yes , QMessageBox.Yes)                                    
            pass
        pass


    def log_in_callback(self):
        account = str(self.textEdit_account_2.toPlainText()) 
        password = str(self.textEdit_password_2.toPlainText())
        server = str(self.Box_server_2.currentText())        
        if self.pushButton_login.text() == "登录":
            self.pushButton_login.setText("退出")
            print(account,password,server)
            try:
                #登录邮箱
                self.yag = yagmail.SMTP(user=account, password=password, host=server)
                self.log_text.append("账号："+str(account)+"  已登录成功")
            
            except:
                self.log_text.append("网络错误")

                pass
        elif self.pushButton_login.text() == "退出":
            self.pushButton_login.setText("登录")
            self.log_text.append("账号："+str(account)+"  已退出")            
            pass      
        
        pass

    def print_exel(self,data):
        rowNum = data.nrows  # sheet行数
        colNum = data.ncols  # sheet列数
        print(data.name,rowNum,colNum)  # 获取sheet名称

        i = 1
        while i<rowNum: 
            rows = data.row_values(i) 
            # print(rows)
            i = i+1        
        pass

    def get_exel_data_and_check(self,data):
        rowNum = data.nrows  # 获取行数
        i=1
        while i<rowNum:
            rows_data = data.row_values(i) #第i行数据

            SZ_name = rows_data[0]
            to_addr = rows_data[2]
            name = rows_data[1].split(" ",1)    #分隔字符返回列表
            # serial_num = rows_data[3]
            self.log_text.append("-----------------------------------------------------------------------------------")
            # self.log_text.append("序号 :"+str(i)+"\n编号 :"+str(int(SZ_name))+"\n名字 :"+name[0]+"\n地址 :"+to_addr +"\n单号 :"+serial_num )
            self.log_text.append("序号 :"+str(i)+"\n编号 :"+str(int(SZ_name))+"\n名字 :"+name[0]+"\n地址 :"+to_addr )

            i = i+1  
            pass    

    '''
    to_addr: 发送邮件地址
    name: 客户名字
    serial_num：物流编号
    '''
    def send_gmail(self,to_addr,name,title,mail_detail):
        # 字符串拼接匹配
        str_info = mail_detail.split("+ name +")[0] + str(name[0]) + mail_detail.split("+ name +")[1]
        # self.log_text.append("邮件内容：\r\n"+str_info)
        
        #发送邮件，（接收邮箱，邮件标题，邮件内容）
        self.yag.send(to = to_addr, subject = title , contents=str_info)  

        pass

    def send_gmail_to_user(self,data):
        title = self.textEdit_title.toPlainText()
        # if len(title)<10:
        #     pass
        # self.log_text.append("邮件标题："+title)

        # mail_detail = self.textEdit_gmail.toPlainText()
        mail_detail = self.plainTextEdit_gmail.toPlainText()
    
        rowNum = data.nrows  # 获取行数
        i=1
        self.cur_progressBar = rowNum

        while i<rowNum:
            QApplication.processEvents()
            rows_data = data.row_values(i) #第i行数据

            SZ_name = rows_data[0]
            to_addr = rows_data[2]
            name = rows_data[1].split(" ",1)    #分隔字符返回列表
            # serial_num = rows_data[3]
            # user_info = "序号 :"+str(i)+"\n编号 :"+str(int(SZ_name))+"\n名字 :"+name[0]+"\n地址 :"+to_addr +"\n单号 :"+serial_num
            user_info = "序号 :"+str(i)+"\n编号 :"+str(int(SZ_name))+"\n名字 :"+name[0]+"\n地址 :"+to_addr 
            self.log_text.append("-----------------------------------------------------------------------------------")
            self.log_text.append(user_info)

            try:
                # self.send_gmail(to_addr,name,serial_num,title,mail_detail)
                self.send_gmail(to_addr,name,title,mail_detail)
                self.log_text.append("\n发送成功")
            except:
                self.textBrowser_sendfail.append("-----------------------------------------------------------------------------------")
                self.textBrowser_sendfail.append(user_info)
                self.log_text.append("\n发送失败")
                # time.sleep(2)

            self.progressBar_2.setValue((i*100)/self.cur_progressBar)
            i = i+1  

            pass 

        self.progressBar_2.setValue(100)
        QMessageBox.question(self, "发送完成", "在 “发送失败” 中查看未发送成功的客户信息，单独处理", QMessageBox.Yes , QMessageBox.Yes)                                    
        pass

    def send_xlsx_callback(self):

        if self.open_xlsx_status:
            self.log_text.append("开始发送 。。。。")  
            self.send_gmail_to_user(self.Data_sheet)
            pass
        else:
            QMessageBox.question(self, "发送 错误", "请先选择 *.xlsx 文件后并校检，再点击 此按钮", QMessageBox.Yes , QMessageBox.Yes)                                    
            pass
        pass        

        pass

    def Chose_xlsx(self):   
        self.path, ok = QFileDialog.getOpenFileName(self,
                                    "选择xlsx",
                                   "./",
                                    "xlsx Files (*.xlsx);;All Files (*)")   
        if ok:
            self.directory_text.setText(str(self.path))                                                      
            print(str(self.path))       
            self.open_xlsx_status = True

            self.log_text.append("已选择文件路径 :"+str(self.path))   

            # 打开execl
            workbook = xlrd.open_workbook(self.path) 
            # 根据sheet索引或者名称获取sheet内容
            self.Data_sheet = workbook.sheets()[0]  # 通过索引获取      
            self.print_exel(self.Data_sheet) 

        pass

        
    pass


if __name__ =='__main__':
    app = QApplication(sys.argv)
    myWin = MyMainWindow()
    myWin.setFixedSize(myWin.width(), myWin.height())
    myWin.show()
    sys.exit(app.exec_())



# import sys
# from PyQt5 import QtCore, QtGui, QtWidgets
# from PyQt5.QtWidgets import QApplication, QMainWindow, QMessageBox,QInputDialog,QFileDialog
# from PyQt5.QtCore import QTimer
# from PyQt5.QtGui import QIcon

# from Ui_mainwindow import Ui_MainWindow
# from struct import *
# import os

# class MyMainWindow(QMainWindow, Ui_MainWindow):
#     def __init__(self,parent = None):
#         super(MyMainWindow, self).__init__(parent)
#         self.setupUi(self)

#         self.setWindowTitle("邮件批量发送")
#     pass


# if __name__ =='__main__':
#     app = QApplication(sys.argv)
#     myWin = MyMainWindow()
#     myWin.setFixedSize(myWin.width(), myWin.height())
#     myWin.show()
#     sys.exit(app.exec_())
