# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'h:\014-QT相关\PyQt-send-mail\send gmail\mainwindow.ui'
#
# Created by: PyQt5 UI code generator 5.15.1
#
# WARNING: Any manual changes made to this file will be lost when pyuic5 is
# run again.  Do not edit this file unless you know what you are doing.


from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(811, 782)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.tabWidget_2 = QtWidgets.QTabWidget(self.centralwidget)
        self.tabWidget_2.setGeometry(QtCore.QRect(30, 20, 753, 706))
        self.tabWidget_2.setObjectName("tabWidget_2")
        self.tab_5 = QtWidgets.QWidget()
        self.tab_5.setObjectName("tab_5")
        self.groupBox_3 = QtWidgets.QGroupBox(self.tab_5)
        self.groupBox_3.setGeometry(QtCore.QRect(10, 10, 261, 141))
        self.groupBox_3.setObjectName("groupBox_3")
        self.verticalLayout_9 = QtWidgets.QVBoxLayout(self.groupBox_3)
        self.verticalLayout_9.setObjectName("verticalLayout_9")
        self.verticalLayout_7 = QtWidgets.QVBoxLayout()
        self.verticalLayout_7.setObjectName("verticalLayout_7")
        self.formLayout_2 = QtWidgets.QFormLayout()
        self.formLayout_2.setObjectName("formLayout_2")
        self.label_6 = QtWidgets.QLabel(self.groupBox_3)
        self.label_6.setObjectName("label_6")
        self.formLayout_2.setWidget(0, QtWidgets.QFormLayout.LabelRole, self.label_6)
        self.label_7 = QtWidgets.QLabel(self.groupBox_3)
        self.label_7.setObjectName("label_7")
        self.formLayout_2.setWidget(1, QtWidgets.QFormLayout.LabelRole, self.label_7)
        self.label_9 = QtWidgets.QLabel(self.groupBox_3)
        self.label_9.setObjectName("label_9")
        self.formLayout_2.setWidget(2, QtWidgets.QFormLayout.LabelRole, self.label_9)
        self.Box_server_2 = QtWidgets.QComboBox(self.groupBox_3)
        self.Box_server_2.setObjectName("Box_server_2")
        self.formLayout_2.setWidget(2, QtWidgets.QFormLayout.FieldRole, self.Box_server_2)
        self.textEdit_account_2 = QtWidgets.QTextEdit(self.groupBox_3)
        self.textEdit_account_2.setMaximumSize(QtCore.QSize(16777215, 20))
        self.textEdit_account_2.setObjectName("textEdit_account_2")
        self.formLayout_2.setWidget(0, QtWidgets.QFormLayout.FieldRole, self.textEdit_account_2)
        self.textEdit_password_2 = QtWidgets.QTextEdit(self.groupBox_3)
        self.textEdit_password_2.setMaximumSize(QtCore.QSize(16777215, 20))
        self.textEdit_password_2.setObjectName("textEdit_password_2")
        self.formLayout_2.setWidget(1, QtWidgets.QFormLayout.FieldRole, self.textEdit_password_2)
        self.verticalLayout_7.addLayout(self.formLayout_2)
        self.pushButton_login = QtWidgets.QPushButton(self.groupBox_3)
        self.pushButton_login.setObjectName("pushButton_login")
        self.verticalLayout_7.addWidget(self.pushButton_login)
        self.verticalLayout_9.addLayout(self.verticalLayout_7)
        self.tabWidget_3 = QtWidgets.QTabWidget(self.tab_5)
        self.tabWidget_3.setGeometry(QtCore.QRect(10, 160, 731, 481))
        self.tabWidget_3.setObjectName("tabWidget_3")
        self.tab_6 = QtWidgets.QWidget()
        self.tab_6.setObjectName("tab_6")
        self.verticalLayout_10 = QtWidgets.QVBoxLayout(self.tab_6)
        self.verticalLayout_10.setObjectName("verticalLayout_10")
        self.log_text = QtWidgets.QTextEdit(self.tab_6)
        self.log_text.setObjectName("log_text")
        self.verticalLayout_10.addWidget(self.log_text)
        self.tabWidget_3.addTab(self.tab_6, "")
        self.tab_7 = QtWidgets.QWidget()
        self.tab_7.setObjectName("tab_7")
        self.textBrowser_sendfail = QtWidgets.QTextBrowser(self.tab_7)
        self.textBrowser_sendfail.setGeometry(QtCore.QRect(10, 10, 701, 431))
        self.textBrowser_sendfail.setObjectName("textBrowser_sendfail")
        self.tabWidget_3.addTab(self.tab_7, "")
        self.progressBar_2 = QtWidgets.QProgressBar(self.tab_5)
        self.progressBar_2.setGeometry(QtCore.QRect(10, 650, 731, 21))
        self.progressBar_2.setProperty("value", 24)
        self.progressBar_2.setObjectName("progressBar_2")
        self.pushButton_check = QtWidgets.QPushButton(self.tab_5)
        self.pushButton_check.setGeometry(QtCore.QRect(310, 130, 75, 23))
        self.pushButton_check.setObjectName("pushButton_check")
        self.formLayoutWidget_6 = QtWidgets.QWidget(self.tab_5)
        self.formLayoutWidget_6.setGeometry(QtCore.QRect(310, 30, 341, 22))
        self.formLayoutWidget_6.setObjectName("formLayoutWidget_6")
        self.formLayout_6 = QtWidgets.QFormLayout(self.formLayoutWidget_6)
        self.formLayout_6.setContentsMargins(0, 0, 0, 0)
        self.formLayout_6.setVerticalSpacing(6)
        self.formLayout_6.setObjectName("formLayout_6")
        self.label_10 = QtWidgets.QLabel(self.formLayoutWidget_6)
        self.label_10.setObjectName("label_10")
        self.formLayout_6.setWidget(0, QtWidgets.QFormLayout.LabelRole, self.label_10)
        self.directory_text = QtWidgets.QTextBrowser(self.formLayoutWidget_6)
        self.directory_text.setMaximumSize(QtCore.QSize(16777215, 20))
        self.directory_text.setObjectName("directory_text")
        self.formLayout_6.setWidget(0, QtWidgets.QFormLayout.FieldRole, self.directory_text)
        self.Button_chose = QtWidgets.QPushButton(self.tab_5)
        self.Button_chose.setGeometry(QtCore.QRect(660, 30, 61, 23))
        self.Button_chose.setObjectName("Button_chose")
        self.pushButton_send_2 = QtWidgets.QPushButton(self.tab_5)
        self.pushButton_send_2.setGeometry(QtCore.QRect(540, 130, 75, 23))
        self.pushButton_send_2.setObjectName("pushButton_send_2")
        self.tabWidget_2.addTab(self.tab_5, "")
        self.tab_8 = QtWidgets.QWidget()
        self.tab_8.setObjectName("tab_8")
        self.verticalLayout_11 = QtWidgets.QVBoxLayout(self.tab_8)
        self.verticalLayout_11.setObjectName("verticalLayout_11")
        self.groupBox_4 = QtWidgets.QGroupBox(self.tab_8)
        self.groupBox_4.setObjectName("groupBox_4")
        self.verticalLayout_12 = QtWidgets.QVBoxLayout(self.groupBox_4)
        self.verticalLayout_12.setObjectName("verticalLayout_12")
        self.verticalLayout_13 = QtWidgets.QVBoxLayout()
        self.verticalLayout_13.setObjectName("verticalLayout_13")
        self.formLayout_7 = QtWidgets.QFormLayout()
        self.formLayout_7.setContentsMargins(-1, 0, -1, 0)
        self.formLayout_7.setVerticalSpacing(6)
        self.formLayout_7.setObjectName("formLayout_7")
        self.label_11 = QtWidgets.QLabel(self.groupBox_4)
        self.label_11.setObjectName("label_11")
        self.formLayout_7.setWidget(0, QtWidgets.QFormLayout.LabelRole, self.label_11)
        self.textEdit_title = QtWidgets.QTextEdit(self.groupBox_4)
        self.textEdit_title.setMaximumSize(QtCore.QSize(700, 20))
        self.textEdit_title.setObjectName("textEdit_title")
        self.formLayout_7.setWidget(0, QtWidgets.QFormLayout.FieldRole, self.textEdit_title)
        self.verticalLayout_13.addLayout(self.formLayout_7)
        self.verticalLayout_14 = QtWidgets.QVBoxLayout()
        self.verticalLayout_14.setObjectName("verticalLayout_14")
        self.label_12 = QtWidgets.QLabel(self.groupBox_4)
        self.label_12.setObjectName("label_12")
        self.verticalLayout_14.addWidget(self.label_12)
        self.plainTextEdit_gmail = QtWidgets.QPlainTextEdit(self.groupBox_4)
        self.plainTextEdit_gmail.setMinimumSize(QtCore.QSize(400, 580))
        self.plainTextEdit_gmail.setObjectName("plainTextEdit_gmail")
        self.verticalLayout_14.addWidget(self.plainTextEdit_gmail)
        self.verticalLayout_13.addLayout(self.verticalLayout_14)
        self.verticalLayout_12.addLayout(self.verticalLayout_13)
        self.verticalLayout_11.addWidget(self.groupBox_4)
        self.tabWidget_2.addTab(self.tab_8, "")
        MainWindow.setCentralWidget(self.centralwidget)

        self.retranslateUi(MainWindow)
        self.tabWidget_2.setCurrentIndex(0)
        self.tabWidget_3.setCurrentIndex(0)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))
        self.groupBox_3.setTitle(_translate("MainWindow", "登录设置"))
        self.label_6.setText(_translate("MainWindow", "账号"))
        self.label_7.setText(_translate("MainWindow", "密码"))
        self.label_9.setText(_translate("MainWindow", "服务器"))
        self.pushButton_login.setText(_translate("MainWindow", "登录"))
        self.tabWidget_3.setTabText(self.tabWidget_3.indexOf(self.tab_6), _translate("MainWindow", "日志"))
        self.tabWidget_3.setTabText(self.tabWidget_3.indexOf(self.tab_7), _translate("MainWindow", "发送失败"))
        self.pushButton_check.setText(_translate("MainWindow", "校检测试"))
        self.label_10.setText(_translate("MainWindow", "路径"))
        self.directory_text.setHtml(_translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'SimSun\'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>"))
        self.Button_chose.setText(_translate("MainWindow", "文件选择"))
        self.pushButton_send_2.setText(_translate("MainWindow", "开始发送"))
        self.tabWidget_2.setTabText(self.tabWidget_2.indexOf(self.tab_5), _translate("MainWindow", "设置"))
        self.groupBox_4.setTitle(_translate("MainWindow", "邮件编辑"))
        self.label_11.setText(_translate("MainWindow", "标题"))
        self.label_12.setText(_translate("MainWindow", "内容"))
        self.tabWidget_2.setTabText(self.tabWidget_2.indexOf(self.tab_8), _translate("MainWindow", "邮件"))
