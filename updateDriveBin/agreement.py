from enum import Enum, EnumMeta

cmdBaseAddr = 0x40
READ_CMD = cmdBaseAddr + 1
WERITE_CMD = cmdBaseAddr + 2
ANSWER_CMD = cmdBaseAddr + 3
AUTO_ANSWER_CMD = cmdBaseAddr + 4

funBaseAddr = 0x50
n = ['id','标零','标重','去皮','取消去皮','帧率','重量','上报使能','版本查询','升级','固件大小','固件数据']
fun = Enum('功能',(n[0],n[1],n[2],n[3],n[4],n[5],n[6],n[7],n[8],n[9],n[10],n[11]))

print(len(n))
funDict = {'功能':{}}

print(funDict)

for i in range(len(n)):
    funDict['功能'][n[i]] = funBaseAddr + fun[n[i]].value

print(funDict)

# #################################################################################

# totalSize = 1024*1024
# startAddr = 0x08000000

# bootSize = 256*1024
# appAddr = startAddr + bootSize 
# appSize = 400*1024
# driveAddr = startAddr + bootSize + appSize
# driveSize = 30*1024
# forceAddr = startAddr + bootSize + appSize + driveSize
# forceSize = 30*1024

# print("芯片起始地址 :" + str(hex(startAddr)) + "  size:" + str(totalSize))
# print("boot起始地址 :" + str(hex(startAddr)) + "  size:" + str(bootSize))
# print("app起始地址  :" + str(hex(appAddr)) + "  size:" + str(appSize))
# print("drive起始地址:" + str(hex(driveAddr)) + "  size:" + str(driveSize))
# print("force起始地址:" + str(hex(forceAddr)) + "  size:" + str(forceSize))

