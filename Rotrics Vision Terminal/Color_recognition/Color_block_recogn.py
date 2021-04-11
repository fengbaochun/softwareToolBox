import cv2
import numpy as np
import math

from Cam_dev import *

tar_color = 'green'
color_dict = {'red': {'Lower': np.array([127, 60, 171]), 'Upper': np.array([188, 197, 255])},
              'blue': {'Lower': np.array([100, 80, 46]), 'Upper': np.array([124, 255, 255])},
              'green': {'Lower': np.array([35, 43, 35]), 'Upper': np.array([90, 255, 255])},
              }              

red_hsv = [108, 190, 120, 255, 163, 223]
blue_hsv = [74, 131, 107, 241, 146, 255]
yellow_hsv = [30, 83, 60, 209, 156, 255]

feature_param=[40,40,60,250,20]

rgb_param=[0,0,255]


class Color_block_recogn():
    tar_num = 0

    tar_info = {"num":0,
            "center":[],
            "angle":[]}
    

    def __init__(self,color_list=[],fea_list=[],rect_rgb_list=[]):
        self.hsv = color_list
        self.fea_p = fea_list
        self.rect_rgb = rect_rgb_list
        print("Color hsv"+str(self.hsv))
        print("Target characteristics"+str(self.fea_p))
        print("Rectangular border"+str(self.rect_rgb))
        pass
    
    def set_hsv(self,tar_list=[]):
        self.hsv = tar_list
        pass

    def set_fea(self,tar_list=[]):
        self.fea_p = tar_list
        pass

    def set_rect_rgb(self,tar_list=[]):
        self.rect_rgb = tar_list
        pass

    def get_target_img(self,img,condition_index):
        

        temp_tar_info = {"num":0,
        "center":[],
        "angle":[]}
        temp_num = 0


        gs_img = cv2.GaussianBlur(img, (5, 5), 0)                     

        hsv_img = cv2.cvtColor(gs_img, cv2.COLOR_BGR2HSV)

        inRange_hsv = cv2.inRange(hsv_img, np.array([self.hsv[0],self.hsv[2],self.hsv[4]]), np.array([self.hsv[1],self.hsv[3],self.hsv[5]]))

        average_val_img = cv2.blur(inRange_hsv,(3,3))

        canny_img = cv2.Canny(average_val_img,128,255,3)

        _,contours, hierarchy = cv2.findContours(canny_img,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)

        try:
            for i in range(len(contours)):
                # if (len(contours[i])>self.fea_p[2]) and (len(contours[i])<self.fea_p[3]):
                # if (len(contours[i])>self.fea_p[2]):
                if (len(contours[i])>35):
                
                    min_rect = cv2.minAreaRect(contours[i])

  
                    if condition_index == 1:

                        result = (min_rect[1][0]>35 and min_rect[1][1]>35)and(abs(min_rect[1][0]-min_rect[1][1])<25)
                    elif condition_index == 2:
                        result = (min_rect[1][0] * min_rect[1][1]) > 670

                    # if (min_rect[1][0]>35 and min_rect[1][1]>35)and(abs(min_rect[1][0]-min_rect[1][1])<25):
                    # if ((min_rect[1][0] * min_rect[1][1]) > 670):
                    if result:
 
                        box_points = cv2.boxPoints(min_rect)

                        cv2.circle(img,(int(min_rect[0][0]),int(min_rect[0][1])) ,2,(self.rect_rgb[0],self.rect_rgb[1], self.rect_rgb[2]),4)
                        cv2.drawContours(img, [np.int0(box_points)], 0, (self.rect_rgb[0],self.rect_rgb[1], self.rect_rgb[2]), 2)        
                        
               
                        temp_tar_info["center"].insert(temp_num,np.int0(min_rect[0]))
                        temp_tar_info["angle"].insert(temp_num,np.int0(min_rect[2]))  

                        temp_num = temp_num + 1

            temp_tar_info["num"]=temp_num

            self.tar_info.clear()
            self.tar_info = temp_tar_info
            # print(self.tar_info)
        except:
            print("error------------->")

        return img,inRange_hsv


    def get_tar_info(self):
        pass
    
    pass


def recogn_main():
    
    video.open(1,640,480)
    revogn = Color_block_recogn(red_hsv,feature_param,rgb_param)
    while True:
        img,inRange_hsv = revogn.get_target_img(video.get_img(1))
        cv2.imshow('inRange_hsv', inRange_hsv)
        cv2.imshow('src_img', img)

        cv2.waitKey(30)
    pass


if __name__ == "__main__":
    recogn_main()
    pass
