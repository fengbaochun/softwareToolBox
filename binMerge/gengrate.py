import sys
import os
import time
import binascii
import struct
import shutil


binInfo = {
    'bin0':{
        'path':'./aibus200.bin',       
        'startAddr':0x00,      
        'maxSize':0,         
        'isWriteBinInfo':False,      
    },    
    'bin1':{
        'path':'./app.bin',         #固件所在路径
        'startAddr':0x00,           #固件起始地址
        'maxSize':16*1024,          #固件对齐大小
        'isWriteBinInfo':True,      #是否在固件开始写入固件信息
    },
    'bin2':{
        'path':'./drive.bin',       
        'startAddr':0x00,      
        'maxSize':0*1024,         
        'isWriteBinInfo':True,      
    },
    'bin3':{            
        'path':'./force.bin',       
        'startAddr':0x00,      
        'maxSize':16*1024,         
        'isWriteBinInfo':True,      
    },
}

newBinPath = './aibus200.bin'

'''

'''
def creatBin(newBinPath,offset,info):
    if info['maxSize'] is not 0:
        print('-------------------')
        newBin = open(newBinPath,'rb+')
        newBin.seek(offset,0)                   
        print(newBin.tell())
        size = os.path.getsize(info['path'])    
        binData = open(info['path'], 'rb')
        newBin.write(binData.read())

        diff = info['maxSize']-size
        if diff > 0:
            for i in range(diff):   
                data = struct.pack('B',0xFF)  #写1
                newBin.write(data)  

        newBin.flush()  
        print(newBin.tell())
    pass


def main():   
    open(newBinPath,'wb+')              #删除并创建源文件

    # creatBin(binInfo['bin0'])
    creatBin(newBinPath,0,binInfo['bin1'])
    creatBin(newBinPath,binInfo['bin1']['maxSize'],binInfo['bin2'])
    creatBin(newBinPath,binInfo['bin1']['maxSize'] + binInfo['bin2']['maxSize'],binInfo['bin3'])
    pass
    

if __name__ == '__main__':
    main()
    
