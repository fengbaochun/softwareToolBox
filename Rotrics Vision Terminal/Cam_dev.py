import cv2
import yaml
import numpy as np


class Cam_dev():

    port_list = []  

    status = False

    '''
    # 640*480
    cameraMatrix = np.array([
                            [ 7.9641507015667764e+02, 0., 3.1577913194699374e+02], 
                            [0.,7.9661307355876215e+02, 2.1453452136833957e+02], 
                            [0., 0., 1. ]
                            ])

    distCoeffs = np.array([
                        [ -1.1949335317713690e+00,
                        1.8078010700662486e+00,
                        4.9410258870084744e-03, 
                        2.8036176641915598e-03,
                        -2.0575845684235938e+00]
                        ])  
    '''
    # 800*600
    cameraMatrix = np.array([
                            [ 7.7147312644612566e+02, 0., 4.1076305384540058e+02], 
                            [0.,7.7196085036305726e+02, 2.7072648925224809e+02], 
                            [0., 0., 1. ]                            
                            ])

    distCoeffs = np.array([ -7.5184623537659290e-01, 
                            9.0484565422186369e-01,
                            3.5328031670409693e-03, 
                            -9.5538846153888600e-03,
                            -9.0276848603826498e-01 ])  


    def __init__(self):

        self.scan()

        pass
    

    def open(self,dev_id,cap_w,cap_h):
        self.cap = cv2.VideoCapture(dev_id)

        if self.cap.isOpened():

            self.cap.set(cv2.CAP_PROP_FRAME_WIDTH, cap_w)
            self.cap.set(cv2.CAP_PROP_FRAME_HEIGHT, cap_h)
            self.status = True

        else:
            print("open fail")  
        pass



    def scan(self):
        self.dev_list = []
        num = 0
        for i in range(10):
            try:
                temp_cap = cv2.VideoCapture(i)
                if temp_cap.isOpened():
                    self.dev_list.append(str(num))
                    num = num + 1
                    temp_cap.release()
            except:
                pass

        print("dev"+str(self.dev_list))
        return self.dev_list
        pass


    def close(self):
        self.cap.release()
        self.status = False
     
        pass

    ''' img_type : 0  '''
    ''' img_type : 1  '''
    def get_img(self,img_type):
        if img_type==0:
            ret, img = self.cap.read()
        elif img_type==1:
            ret, img = self.cap.read()
            img = cv2.undistort(img, self.cameraMatrix, self.distCoeffs, None)
        return img
    
    pass

video = Cam_dev()