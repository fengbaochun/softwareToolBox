import serial,time
import serial.tools.list_ports
import threading
from time import sleep
from PyQt5.QtWidgets import QApplication,QMainWindow,QMessageBox


class Serial_dev(object):

    status = False
    bps = 115200
    def __init__(self):
        self.scan()
        pass
    
    def scan(self):
        self.port_list = list(serial.tools.list_ports.comports())
        if len(self.port_list) == 0:
            pass
        else:
            print("dev:")
            for i in range(0,len(self.port_list)):
                print(self.port_list[i])
        pass
    
    def open(self):
        try:
            self.ser_v = serial.Serial(self.port,self.bps,timeout=3000)
            if self.ser_v.is_open:
                self.status = True
            else:
                self.status = False
            pass
        except:
            self.status = False
            pass

        return self.status
        


    def close(self):
        self.ser_v.close()
        self.status = False
        print(str(self.port)+"close")
        pass



    def send(self,data):
        status = True

        if self.status:
            
            self.ser_v.write(data.encode("gbk"))
        
            while status:
                data = self.ser_v.read_until()
                print(data)
                if "ok\\" in str(data):
                    print(str(data))
                    return          
                elif "Unknown command" in str(data):
                    print(str(data)+"send error")
                    self.ser_v.write(data.encode("gbk"))
                    pass
        pass
    
    def send_hex(self,data):
        status = True

        if self.status:

            self.ser_v.write(bytes().fromhex(data))
        
            while status:
                data = self.ser_v.read_until()
                print(data)
                if "ok\\" in str(data):
                    print(str(data))
                    return          
                elif "Unknown command" in str(data):
                    print(str(data)+"send error")
                    self.ser_v.write(data.encode("gbk"))
                    pass
        pass    

    def only_send_hex(self,data):
        status = True
        if self.status:
            self.ser_v.write(bytes().fromhex(data))


    def read(self):
        # status = False
        # while status:
        #     data = self.ser_v.read_until()
        #     print(data)
        #     if "ok\\" in str(data):
        #         print(str(data))
        #         return 
        pass    
    

    def set_bps(self,val):
        self.bps = val
        pass

    def set_com(self,num):
        self.port = num
        pass


    pass

global Com_dev
Com_dev = Serial_dev()



def main():
    Gcode1="M1111\r\n"
    Gcode2="G0X255Y0Z180\r\n"

    dev = Serial_dev()
    dev.set_com("COM6")
    dev.set_bps(115200)
    dev.open()
    dev.send(Gcode2)
    dev.read()
    dev.close()
    # dev.scan()
    pass

if __name__ == "__main__":
    main()    
    pass

