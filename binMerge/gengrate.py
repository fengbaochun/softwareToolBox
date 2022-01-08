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
        'version':'0.0', 
    },    
    'bin1':{
        'path':'./app.bin',         #固件所在路径
        'startAddr':0x00,           #固件起始地址
        'maxSize':16*1024,          #固件对齐大小
        'isWriteBinInfo': False,      #是否在固件开始写入固件信息
        'version':'0.0', 
    },
    'bin2':{
        'path':'./drive.bin',       
        'startAddr':0x00,      
        'maxSize':16*1024,         
        'isWriteBinInfo':True,     
        'version':'1.2', 
    },
    'bin3':{            
        'path':'./force.bin',       
        'startAddr':0x00,      
        'maxSize':32*1024,         
        'isWriteBinInfo':True,      
        'version':'1.2', 
    },
}

newBinPath = './aibus200.bin'


def writeBinInfo(newBin,binSize,mainVer,subVer):
    # 将驱动大小写入到主控末尾 (共16个字节，中间两个字节为驱动固件大小，其余字节为0x00)
    data = struct.pack('B',0x00)
    for i in range(7):
        newBin.write(data)
    
    # 固件大小
    data = struct.pack('B',binSize&0xff)
    newBin.write(data)
    data = struct.pack('B',(binSize>>8)&0xff)
    newBin.write(data)
    
    # 版本号
    data = struct.pack('B',mainVer)
    newBin.write(data)
    data = struct.pack('B',subVer)
    newBin.write(data)

    data = struct.pack('B',0x00)
    for i in range(5):
        newBin.write(data)          

    newBin.flush()

    return newBin

def creatBin(newBinPath,offset,info):
    if info['maxSize'] is not 0:
        print('-------------------')
        newBin = open(newBinPath,'rb+')
        newBin.seek(offset,0)          

        size = os.path.getsize(info['path'])    
        diff = info['maxSize']-size                     #计算需要对齐的数量

        if info['isWriteBinInfo']:                      #根据字典判断是否需要写入
            newBin = writeBinInfo(newBin,size,1,10)     #写入固件信息（包含固件固件大小 及 版本）
            diff = diff - 16
            pass

        print(newBin.tell())

        binData = open(info['path'], 'rb')              
        newBin.write(binData.read())                    #写入固件
        
        if diff > 0:
            for i in range(diff):   
                data = struct.pack('B',0xFF)            #写FF
                newBin.write(data)  

        newBin.flush()  
        print(newBin.tell())
    pass


def main():   
    open(newBinPath,'wb+')                              #删除并创建源文件

    creatBin(newBinPath,0,binInfo['bin1'])
    creatBin(newBinPath,binInfo['bin1']['maxSize'],binInfo['bin2'])
    creatBin(newBinPath,binInfo['bin1']['maxSize'] + binInfo['bin2']['maxSize'],binInfo['bin3'])
    pass
    

if __name__ == '__main__':
    main()
    

