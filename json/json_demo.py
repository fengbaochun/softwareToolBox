import json
import os

'''json 操作类'''
class Json_c():
    '''类初始化'''
    def __init__(self,temp_path):
        self.path = temp_path
        try:
            self.read_all()
        except :
            print("json : "+str(self.path)+"文件不存在")
            pass
        pass
    
    '''读'''
    def read_all(self):

        with open(self.path,'r') as load_f:
            self.json_data=json.load(load_f)
        
        return self.json_data
    
    '''读一级键对应的内容'''
    def read_level1(self,key):
        # print(json.dumps(self.json_data[key],indent=1))
        return self.json_data[key]
    
    '''读二级键对应的内容'''
    def read_level2(self,key1,key2):
        # print(json.dumps(self.json_data[key1][key2],indent=1))
        return self.json_data[key1][key2]

    '''增'''
    def add(self,key,data):
        temp = {key:data}
        self.json_data.update(temp)
        pass

    '''删'''
    def dele(self,key):
        self.json_data.pop(key)
        pass

    '''更新键值'''
    def update_data1(self,key,value):
        self.json_data[key] = value
        # json 转 字符串
        json_str = json.dumps(self.json_data, ensure_ascii=False, indent=4)
        # 写入字符串到json
        with open(self.path, 'w') as json_file:
	        json_file.write(json_str)

        pass

    '''更新键值'''
    def update_data2(self,key1,key2,value):
        self.json_data[key1][key2] = value
        # json 转 字符串
        json_str = json.dumps(self.json_data, ensure_ascii=False, indent=4)
        # 写入字符串到json
        with open(self.path, 'w') as json_file:
	        json_file.write(json_str)

        pass


    '''打印'''
    def print_json(self):
        print(json.dumps(self.json_data,indent=1))
        pass

    pass

global Json_g
Json_g = Json_c('config.json')

'''主函数'''
def main():
    test = Json_c('config.json')
    test.read_level1("place_pos")
    test.read_level2("place_pos","red")
    test.print_json()
    test.update_data1("red_hsv",[108, 190, 59, 255, 70, 255])
    test.update_data2("place_pos","red", [-180,280,0])
    test.print_json()
    # test.add("name","789456123456789456")
    # test.print_json()

    pass

if __name__ == "__main__":
    main()
    pass

