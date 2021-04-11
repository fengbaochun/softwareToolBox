import sys
import cv2
import time
import serial
import serial.tools.list_ports
import numpy as np
import math
from PyQt5 import QtWidgets
from PyQt5.QtWidgets import QMessageBox,QApplication,QSpinBox
from PyQt5.QtCore import QTimer
from PyQt5.QtWidgets import QCheckBox
from PyQt5.QtCore import Qt
from PyQt5.QtCore import pyqtSlot
from PyQt5.QtGui import QImage, QPixmap
from Tool_box.Serial_tool import *

from Ui_find_color_block import Ui_find_color_block

from Color_recognition.Color_block_recogn import Color_block_recogn

from Cam_dev import *
from config import *




color_dict ={   'red':      [127,197,188,60,197,255],
                'blue':     [50,197,188,80,197,255],
                'yellow':   [60,197,188,100,197,255],
              }   


global red_hsv
global bule_hsv
global yellow_hsv


# red_hsv = [108, 190, 59, 255, 70, 255]
# blue_hsv = [74, 131, 107, 241, 146, 255]
# yellow_hsv = [30, 83, 125, 209, 143, 255]

red_hsv = Json_g.read_level1("red_hsv")
blue_hsv = Json_g.read_level1("blue_hsv")
yellow_hsv = Json_g.read_level1("yellow_hsv")

feature_param=[50,50,100,250]

rgb_param=[0,0,255]

obj_all_info = {
            "red":{},
            "blue":{},
            "yellow":{}
            }

# obj_fact_size = 30 # mm
# obj_img_size = 69 # 

obj_fact_size = 29 # mm
obj_img_size = 69 # 

# obj_p = float(obj_img_size)/float(obj_fact_size) 
# print(obj_p)
# Z = -21


global obj_y
global obj_x

global obj_y_d
global obj_x_d

global obj_y_m
global obj_x_m

# obj_y_d = 2.50
# obj_x_d = 2.55

# obj_y_m = 2.60
# obj_x_m = 2.75

obj_y_d = Json_g.read_level1("obj_y_d")
obj_x_d = Json_g.read_level1("obj_x_d")

obj_y_m = Json_g.read_level1("obj_y_m")
obj_x_m = Json_g.read_level1("obj_x_m")


obj_y = 2.50
obj_x = 2.55



# red_place = [-180,280,0]
# blue_place = [-180,200,0]
# yellow_place = [-180,120,0]

# place_pos = {   "red":Json_g.read_level2("place_pos","red"),
#                 "blue":Json_g.read_level2("place_pos","blue"),
#                 "yellow":Json_g.read_level2("place_pos","yellow")
#             }

place_pos = Json_g.read_level1("place_pos")
print(place_pos)

MAX_HIGH=[0,295,167]

VIEW_CENTER=[0,215,-45]

class Find_color_block(QtWidgets.QWidget, Ui_find_color_block):

    hsv=[]
    num=0
    flag = True     
    data_status = True
    demo_index = 1  
    '''init'''
    def __init__(self):
        super(Find_color_block, self).__init__()
        self.setupUi(self)

        self.H_Slider_max.setRange(0,255)
        self.H_Slider_min.setRange(0,255)
        self.S_Slider_max.setRange(0,255)
        self.S_Slider_min.setRange(0,255)
        self.V_Slider_max.setRange(0,255)
        self.V_Slider_min.setRange(0,255)


        self.checkBox_Red.setCheckState(Qt.Checked)   


        self.hsv=red_hsv
        self.H_Slider_max.setValue(self.hsv[0])
        self.H_Slider_min.setValue(self.hsv[1])
        self.S_Slider_max.setValue(self.hsv[2])
        self.S_Slider_min.setValue(self.hsv[3])
        self.V_Slider_max.setValue(self.hsv[4])
        self.V_Slider_min.setValue(self.hsv[5])
        self.TextEdit_hsv.insertPlainText(str(red_hsv))

        self.H_Slider_max.valueChanged.connect(self.Slider_change)   
        self.H_Slider_min.valueChanged.connect(self.Slider_change)   
        self.S_Slider_max.valueChanged.connect(self.Slider_change)   
        self.S_Slider_min.valueChanged.connect(self.Slider_change)   
        self.V_Slider_max.valueChanged.connect(self.Slider_change)   
        self.V_Slider_min.valueChanged.connect(self.Slider_change)   

      
        self.Button_arm_start.clicked.connect(self.Arm_work)  

        self.checkBox_Red.clicked.connect(self.on_red_click) 
        self.checkBox_Yellow.clicked.connect(self.on_yellow_click) 
        self.checkBox_Blue.clicked.connect(self.on_blue_click) 

        self.show_red_place.clear()
        self.show_red_place.insertPlainText(str(place_pos["red"]))
        self.show_blue_place.insertPlainText(str(place_pos["blue"]))
        self.show_yellow_place.insertPlainText(str(place_pos["yellow"]))
 
        self.spinBox.setValue(10)

        self.Button_x_add.clicked.connect(self.change_ARM_tar_pos) 
        self.Button_x_less.clicked.connect(self.change_ARM_tar_pos) 
        self.Button_y_add.clicked.connect(self.change_ARM_tar_pos) 
        self.Button_y_less.clicked.connect(self.change_ARM_tar_pos) 
        self.Button_z_add.clicked.connect(self.change_ARM_tar_pos) 
        self.Button_z_less.clicked.connect(self.change_ARM_tar_pos) 
        self.Button_rest.clicked.connect(self.change_ARM_tar_pos)
        self.Button_home.clicked.connect(self.change_ARM_tar_pos)


        self.demo = ["None","Block", "Beans"]
        self.Boxdemo.addItems(self.demo)
        self.Boxdemo.setCurrentIndex(0)     
        self.Boxdemo.currentIndexChanged.connect(self.change_demo)          

        self.revogn = Color_block_recogn(red_hsv,feature_param,rgb_param)

        self.timer = QTimer()  
        self.timer.timeout.connect(self.get_data_result)
        self.timer.start(10) 
        
        self.G = Gcode()

        self.Box_x.setRange(200,1000)
        self.Box_y.setRange(200,1000)

        self.Box_x.setValue(int(obj_x*100))
        self.Box_y.setValue(int(obj_y*100))

        self.Button_xy_ok.clicked.connect(self.set_xy_multiple)

        print("init ok")
 
        pass

    def set_xy_multiple(self):

        global obj_y
        global obj_x

        global obj_y_d
        global obj_x_d

        global obj_y_m
        global obj_x_m        


        print("X:"+self.Box_x.text())
        print("Y:"+self.Box_y.text())

        obj_x = float(self.Box_x.text())/float(100)
        obj_y = float(self.Box_y.text())/float(100)

        print("obj_x:"+str(float(obj_x)))
        print("obj_y:"+str(float(obj_y)))    

        current_demo = str(self.Boxdemo.currentText())
        print(str(self.Boxdemo.currentIndex()))

        self.demo_index = self.Boxdemo.currentIndex()

        if current_demo == self.demo[1]:

            print("X:"+self.Box_x.text())
            print("Y:"+self.Box_y.text())


            obj_x = float(self.Box_x.text())/float(100)
            obj_y = float(self.Box_y.text())/float(100)

            obj_y_m = obj_y
            obj_x_m = obj_x

            print("obj_x:"+str(float(obj_x)))
            print("obj_y:"+str(float(obj_y)))   

            Json_g.update_data1("obj_y_m",obj_y_m)
            Json_g.update_data1("obj_x_m",obj_x_m)

            pass

        elif current_demo == self.demo[2]:

            print("X:"+self.Box_x.text())
            print("Y:"+self.Box_y.text())


            obj_x = float(self.Box_x.text())/float(100)
            obj_y = float(self.Box_y.text())/float(100)

            obj_y_d = obj_y
            obj_x_d = obj_x

            print("obj_x:"+str(float(obj_x)))
            print("obj_y:"+str(float(obj_y)))   

            Json_g.update_data1("obj_y_d",obj_y_d)
            Json_g.update_data1("obj_x_d",obj_x_d)

            pass        
        pass   

        pass

    

    def change_demo(self):
        current_demo = str(self.Boxdemo.currentText())
        print(str(self.Boxdemo.currentIndex()))
   
        self.demo_index = self.Boxdemo.currentIndex()
    
        if current_demo == self.demo[1]:
            print(str(self.demo[1]))
            self.Box_x.setValue(int(obj_x_m*100))
            self.Box_y.setValue(int(obj_y_m*100))
            pass
   
        elif current_demo == self.demo[2]:
            print(str(self.demo[2]))
            self.Box_x.setValue(int(obj_x_d*100))
            self.Box_y.setValue(int(obj_y_d*100))
            pass        
        pass

    
  
    def fill_data_to_Slider(self,data=[]):
        self.flag = False
       
    
        self.H_Slider_max.setValue(int(data[0]))
        self.H_Slider_min.setValue(int(data[1]))
        self.S_Slider_max.setValue(int(data[2]))
        self.S_Slider_min.setValue(int(data[3]))
        self.V_Slider_max.setValue(int(data[4]))
        self.V_Slider_min.setValue(int(data[5]))

        temp=[]
        temp.append(self.H_Slider_max.value())
        temp.append(self.H_Slider_min.value())
        temp.append(self.S_Slider_max.value())
        temp.append(self.S_Slider_min.value())
        temp.append(self.V_Slider_max.value())
        temp.append(self.V_Slider_min.value())


        self.flag = True
        

        self.TextEdit_hsv.clear() 
    
        self.TextEdit_hsv.insertPlainText(str(data))
        pass


    def get_ARM_pos(self,x,y):

        x = VIEW_CENTER[0] + math.ceil(float((x-400)*1000) / float(obj_x) / float(1000))   
        y = VIEW_CENTER[1] - math.ceil(float((y-300)*1000) / float(obj_y) / float(1000))
        print("obj_x"+str(obj_x))
        print("obj_y"+str(obj_y))
        return [x,y]


    def move_obj(self):

        for dict_name in obj_all_info:
            try:
                for i in range(len(obj_all_info[dict_name]["center"])):
                    x = obj_all_info[dict_name]["center"][i][0]
                    y = obj_all_info[dict_name]["center"][i][1]     
                    temp = self.get_ARM_pos(x,y)
                    print("img"+str([x,y]))
                    print("ARM"+str(temp))
                    
              
                    Com_dev.send(self.G.XYZ(int(temp[0]),int(temp[1]),0))
                    Com_dev.read()
                    
               
                    Com_dev.send(self.G.Z(-20))
                    Com_dev.read()
                  
                    Com_dev.send(self.G.M100x(0))
                    sleep(0.1)
                    # Com_dev.read()
              
                    Com_dev.send(self.G.Z(20))
                    Com_dev.read()
                  
                    Com_dev.send(self.G.XYZ(place_pos[dict_name][0],place_pos[dict_name][1],-5+30*i+35))
                    Com_dev.read()
           
                    Com_dev.send(self.G.Z(-12+30*i))
                    Com_dev.read()

                    Com_dev.send(self.G.M100x(2))
                    # Com_dev.read()
                    sleep(0.1)

                    Com_dev.send(self.G.Z(-5+30*i+35))
                    Com_dev.read()
                    '''
                    '''

            except:
                print(dict_name+"none")
                pass
        pass


    def move_min_obj(self):
        for dict_name in obj_all_info:
            try:
                for i in range(len(obj_all_info[dict_name]["center"])):
                    x = obj_all_info[dict_name]["center"][i][0]
                    y = obj_all_info[dict_name]["center"][i][1]     
                    temp = self.get_ARM_pos(x,y)
                    print(temp)
                    
                    
                    # Com_dev.send(self.G.XYZ(int(225-temp[0]),int(0-temp[1]),0))
                    Com_dev.send(self.G.XYZ(int(temp[0]),int(temp[1]),0))
                    Com_dev.read()
                    
                 
                    # Com_dev.send(self.G.Z(-21))
                    Com_dev.send(self.G.Z(-40))
                    Com_dev.read()
                    
                    
                    # Com_dev.send(self.G.Z(20))
                    # Com_dev.read()
                    # sleep(1)
                    
               
                    Com_dev.send(self.G.M100x(0))
                    sleep(0.1)
                    # Com_dev.read()
               
                    Com_dev.send(self.G.Z(20))
                    Com_dev.read()
                 
                    # Com_dev.send(self.G.XYZ(place_pos[dict_name][0],place_pos[dict_name][1],-5+30*i+40))
                    Com_dev.send(self.G.XYZ(place_pos[dict_name][0],place_pos[dict_name][1],-5+30*2+40))
                    Com_dev.read()
             
                    Com_dev.send(self.G.Z(-5+30*2))
                    Com_dev.read()
   
                    Com_dev.send(self.G.M100x(2))
                    # Com_dev.read()
                    sleep(0.1)

                    # Com_dev.send(self.G.Z(-5+30*i+40))
                    Com_dev.send(self.G.Z(-5+30*2+40))
                    Com_dev.read()
                    '''
                    '''
                    # send_gcode_Z( Gcode_Z + 2 + Z_val*index + 10)        

            except:
                print(dict_name+"none")
                pass
        pass


    def get_current_ARM_pos(self):
        "G0X280Y0Z0"
        return [0,300,0]
        pass
    

    def get_change_val(self,sender,index):
        
        val = self.spinBox.value()
        if Com_dev.status == False:
            QMessageBox.question(self, "Open error", "Please open the serial port before operation!!!", QMessageBox.Yes , QMessageBox.Yes)  
        else:
            if sender == "X+":
                place_pos[index][0] = place_pos[index][0] + val            
            elif sender == "X-":
                place_pos[index][0] = place_pos[index][0] - val     
            elif sender == "Y+":
                place_pos[index][1] = place_pos[index][1] + val 
            elif sender == "Y-":
                place_pos[index][1] = place_pos[index][1] - val  
            elif sender == "Z+":
                place_pos[index][2] = place_pos[index][2] + val 
            elif sender == "Z-":
                place_pos[index][2] = place_pos[index][2] - val 
            elif sender == "REST":
                place_pos["red"] = [-180,280,0]
                place_pos["blue"] = [-180,200,0]
                place_pos["yellow"] = [-180,120,0]
                Json_g.update_data2("place_pos","red", [-180,280,0])
                Json_g.update_data2("place_pos","blue", [-180,200,0])
                Json_g.update_data2("place_pos","yellow", [-180,120,0])
            # update json file 
            Json_g.update_data2("place_pos",index, place_pos[index])

            print(place_pos[index])
        

        self.show_red_place.clear()
        self.show_red_place.insertPlainText(str(place_pos["red"]))
        self.show_blue_place.clear()
        self.show_blue_place.insertPlainText(str(place_pos["blue"]))
        self.show_yellow_place.clear()
        self.show_yellow_place.insertPlainText(str(place_pos["yellow"]))
        pass
    

    def change_ARM_tar_pos(self):
        sender = self.sender().text()

        if sender == "HOME":
            try:
                Com_dev.send(self.G.home())
                Com_dev.read()                    
            except :
                QMessageBox.question(self, "Open error", "Please open the serial port before operation!!!", QMessageBox.Yes , QMessageBox.Yes)    
        else:

            try:
                if self.checkBox_Red.isChecked():
                    self.get_change_val(sender,"red")
                    Com_dev.send(self.G.XYZ(place_pos["red"][0],place_pos["red"][1],place_pos["red"][2]))
                    Com_dev.read()
                elif self.checkBox_Blue.isChecked():
                    self.get_change_val(sender,"blue")
                    Com_dev.send(self.G.XYZ(place_pos["blue"][0],place_pos["blue"][1],place_pos["blue"][2]))
                    Com_dev.read()
                elif self.checkBox_Yellow.isChecked():
                    self.get_change_val(sender,"yellow")
                    Com_dev.send(self.G.XYZ(place_pos["yellow"][0],place_pos["yellow"][1],place_pos["yellow"][2]))
                    Com_dev.read()      
            except:              

                place_pos["red"] = [-180,280,0]
                place_pos["blue"] = [-180,200,0]
                place_pos["yellow"] = [-180,120,0]
                QMessageBox.question(self, "Open error", "Please open the serial port before operation!!!", QMessageBox.Yes , QMessageBox.Yes)              
                print("open fail")
                pass   
            print(place_pos)

        pass

   
    ''' chose demo '''
    def chose_demo(self,index):

        if index == 1:
            self.move_obj()
            pass

        elif index == 2:
            self.move_min_obj()
            pass
        pass
   
    def Arm_work(self):
        
        if Com_dev.status == True:
            if self.Button_arm_start.text() == "Start":

                self.data_status = False
                print(obj_all_info)
                

                self.chose_demo(self.Boxdemo.currentIndex())
                
                for i in obj_all_info:
                    obj_all_info[i].clear()

                print(obj_all_info)

                Com_dev.send(self.G.home())
                self.data_status = True
                print("work over")
            else:
                # Com_dev.send(self.G.init())
                Com_dev.send(self.G.home())
                print("over")
                self.Button_arm_start.setText("Start")

                self.data_status = True
        else:
            QMessageBox.question(self, 'warning', 'Please open the serial port before operation', QMessageBox.Yes, QMessageBox.Yes)        
        pass


    @pyqtSlot()
    def Slider_change(self):
        global red_hsv
        global bule_hsv
        global yellow_hsv

        if  self.flag == True:
            temp_hsv=[]

            temp_hsv.append(self.H_Slider_max.value())
            temp_hsv.append(self.H_Slider_min.value())
            temp_hsv.append(self.S_Slider_max.value())
            temp_hsv.append(self.S_Slider_min.value())
            temp_hsv.append(self.V_Slider_max.value())
            temp_hsv.append(self.V_Slider_min.value())
            
            print("VAL "+str(temp_hsv))
            print("R"+str(self.checkBox_Red.isChecked()))
            print("B"+str(self.checkBox_Blue.isChecked()))
            print("Y"+str(self.checkBox_Yellow.isChecked()))

            if self.checkBox_Red.isChecked():
                red_hsv[0]=temp_hsv[0]
                red_hsv[1]=temp_hsv[1]
                red_hsv[2]=temp_hsv[2]
                red_hsv[3]=temp_hsv[3]
                red_hsv[4]=temp_hsv[4]
                red_hsv[5]=temp_hsv[5]
                Json_g.update_data1("red_hsv",temp_hsv)
            elif self.checkBox_Blue.isChecked():
                blue_hsv[0]=temp_hsv[0]
                blue_hsv[1]=temp_hsv[1]
                blue_hsv[2]=temp_hsv[2]
                blue_hsv[3]=temp_hsv[3]
                blue_hsv[4]=temp_hsv[4]
                blue_hsv[5]=temp_hsv[5]
                Json_g.update_data1("blue_hsv",temp_hsv)
            elif self.checkBox_Yellow.isChecked():
                yellow_hsv[0]=temp_hsv[0]
                yellow_hsv[1]=temp_hsv[1]
                yellow_hsv[2]=temp_hsv[2]
                yellow_hsv[3]=temp_hsv[3]
                yellow_hsv[4]=temp_hsv[4]
                yellow_hsv[5]=temp_hsv[5]
                Json_g.update_data1("yellow_hsv",temp_hsv)

            self.TextEdit_hsv.clear()
            self.TextEdit_hsv.insertPlainText(str(temp_hsv))
            # temp_hsv.clear()

        pass
    

    @pyqtSlot()
    def on_red_click(self):
        self.checkBox_Yellow.setCheckState(Qt.Unchecked)
        self.checkBox_Blue.setCheckState(Qt.Unchecked)
        self.fill_data_to_Slider(red_hsv)
        self.revogn.set_hsv(red_hsv)
        self.revogn.set_rect_rgb([0,0,255])
        
        print("on_red_click")
        pass

    @pyqtSlot()
    def on_blue_click(self):

        self.checkBox_Yellow.setCheckState(Qt.Unchecked)
        self.checkBox_Red.setCheckState(Qt.Unchecked)
        self.fill_data_to_Slider(blue_hsv)

        self.revogn.set_hsv(blue_hsv)
        self.revogn.set_rect_rgb([255,0,0])

        print("on_blue_click")
        pass

    @pyqtSlot()
    def on_yellow_click(self):
        self.checkBox_Blue.setCheckState(Qt.Unchecked)
        self.checkBox_Red.setCheckState(Qt.Unchecked)
        self.fill_data_to_Slider(yellow_hsv)
        self.revogn.set_hsv(yellow_hsv)
        self.revogn.set_rect_rgb([0,255,0])

        print("on_yellow_click")
        pass
    

    def save_obj_info(self):
        if self.checkBox_Red.isChecked():
            obj_all_info["red"].update(self.revogn.tar_info)
        elif self.checkBox_Blue.isChecked():
            obj_all_info["blue"].update(self.revogn.tar_info)
        elif self.checkBox_Yellow.isChecked():
            obj_all_info["yellow"].update(self.revogn.tar_info)
        # print(obj_all_info)
        pass


    def draw_pos(self,img):
        size = 15
        line_w = 2
        img_half_w = 400
        img_half_h = 300


        cv2.line(img,(img_half_w-size,img_half_h), (img_half_w+size,img_half_h), (0, 0, 0), line_w)
        cv2.line(img,(img_half_w,img_half_h-size), (img_half_w,img_half_h+size), (0, 0, 0), line_w)


        
        diff=[220,220]
        cv2.line(img,(img_half_w-size-diff[0],img_half_h), (img_half_w+size-diff[0],img_half_h), (0, 0, 0), line_w)
        cv2.line(img,(img_half_w-diff[1],img_half_h-size), (img_half_w-diff[1],img_half_h+size), (0, 0, 0), line_w)    

        diff=[-220,-220]
        cv2.line(img,(img_half_w-size-diff[0],img_half_h), (img_half_w+size-diff[0],img_half_h), (0, 0, 0), line_w)
        cv2.line(img,(img_half_w-diff[1],img_half_h-size), (img_half_w-diff[1],img_half_h+size), (0, 0, 0), line_w)    

        diff=[220,220]
        cv2.line(img,(img_half_w-size,img_half_h-diff[0]), (img_half_w+size,img_half_h-diff[0]), (0, 0, 0), line_w)
        cv2.line(img,(img_half_w,img_half_h-size-diff[1]), (img_half_w,img_half_h+size-diff[1]), (0, 0, 0), line_w)    

        diff=[-220,-220]
        cv2.line(img,(img_half_w-size,img_half_h-diff[0]), (img_half_w+size,img_half_h-diff[0]), (0, 0, 0), line_w)
        cv2.line(img,(img_half_w,img_half_h-size-diff[1]), (img_half_w,img_half_h+size-diff[1]), (0, 0, 0), line_w)    
   

        pass
        

    def get_data_result(self):

        # print("video.status"+str(video.status))
        # print("self.data_status"+str(self.data_status))
        if video.status == True:

            if self.data_status == True:

                try:
                    img_src , inrange_img = self.revogn.get_target_img(video.get_img(1),self.demo_index)
                except :
                    img_src , inrange_img = self.revogn.get_target_img(video.get_img(0),self.demo_index)
                    pass
                img = cv2.cvtColor(img_src, cv2.COLOR_BGR2RGB) 
                
   
                self.draw_pos(img)

    
                rows, cols, channels=img.shape
                bytesPerLine = channels * cols
                QImg = QImage(img.data, cols, rows, bytesPerLine, QImage.Format_RGB888)

                self.label_img.setPixmap(QPixmap.fromImage(QImg).scaled(
                    self.label_img.size(), Qt.KeepAspectRatio, Qt.SmoothTransformation))


                rows, cols=inrange_img.shape
                QImg = QImage(inrange_img.data, cols, rows,  QImage.Format_Grayscale8)
                self.label_img_gray.setPixmap(QPixmap.fromImage(QImg).scaled(
                    self.label_img_gray.size(), Qt.KeepAspectRatio, Qt.SmoothTransformation))
                

                self.show_recong_result.clear()
            
                pos = []
                for i in range(self.revogn.tar_info["num"]):
                    pos.insert(i,str(np.array(self.revogn.tar_info["center"])[i]))

                angle = []
                for i in range(self.revogn.tar_info["num"]):
                    angle.insert(i,str(np.array(self.revogn.tar_info["angle"])[i]))

                temp = "num:"+str(self.revogn.tar_info["num"])+"\npos:"+str(pos)+"\nangle:"+str(angle)

                self.show_recong_result.insertPlainText(str(temp))
        

                self.save_obj_info()

            cv2.waitKey(1)
        pass


    pass

'''
gcode 
'''
class Gcode(object):

    def __init__(self):
        pass

    def init(self):
        return "M1111\r\n"
    
    def home(self):
        return self.XYZ(MAX_HIGH[0],MAX_HIGH[1],MAX_HIGH[2])
    
    def Z(self,z):
        temp = "G0"+"Z"+str(z)+"\r\n"
        return temp        
        pass

    def X(self):
        pass

    def Y(self):
        pass

    def XYZ(self,x,y,z):
        temp = "G0"+"X"+str(x)+"Y"+str(y)+"Z"+str(z)+"F5000"+"\r\n"
        return temp

    def XY(self,x,y):
        temp = "G0"+"X"+str(x)+"Y"+str(y)+"\r\n"
        return temp

    def M100x(self,x):
        return "M100"+str(x)+"\r\n"
    
    def speed(self,val):
        return "G0F"+str(val)+"\r\n"

    pass

gcode = Gcode()
