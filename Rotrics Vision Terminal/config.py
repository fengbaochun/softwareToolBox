
import sys
import json
import os


defult_dist = { 
    "red_hsv": [  108,  190,  59,  255,  70,  255 ],
    "blue_hsv": [  74,  130,  114,  241,  136,  255 ],
    "yellow_hsv": [  30,  83,  81,  209,  143,  255 ],
    "obj_y_d": 2.5,
    "obj_x_d": 2.55,
    "obj_y_m": 2.6,
    "obj_x_m": 2.75,
    "place_pos": {
        "red": [   -180,   280,   0  ],
        "blue": [   -180,   200,   0  ],
        "yellow": [   -180,   120,   0  ]
    }
}

'''json '''
class Json_c():

    '''init'''
    def __init__(self,temp_path):
        self.path = temp_path
        self.create_file()
        print(defult_dist)

        try:
            self.read_all()
        except :
            print("json : "+str(self.path)+" no existed")
            pass
        pass

    '''creat file'''
    def create_file(self):
        path = self.path[0:self.path.rfind("/")]
        # if not os.path.isdir(path): 
        #     os.makedirs(path)
        if not os.path.isfile(self.path): 
            fd = open(self.path, mode="w", encoding="utf-8")
            fd.close()

            json_str = json.dumps(defult_dist,indent=1)

            with open(self.path, 'w') as json_file:
                json_file.write(json_str)

        else:
            print(str(self.path)+" existed")
            pass    
    
    '''read all '''
    def read_all(self):

        with open(self.path,'r') as load_f:
            self.json_data=json.load(load_f)
        
        return self.json_data
    
    '''read'''
    def read_level1(self,key):
        # print(json.dumps(self.json_data[key],indent=1))
        return self.json_data[key]
    
    '''read'''
    def read_level2(self,key1,key2):
        # print(json.dumps(self.json_data[key1][key2],indent=1))
        return self.json_data[key1][key2]

    '''add'''
    def add(self,key,data):
        temp = {key:data}
        self.json_data.update(temp)
        pass

    '''del'''
    def dele(self,key):
        self.json_data.pop(key)
        pass

    '''update key value'''
    def update_data1(self,key,value):
        self.json_data[key] = value
        # json to  str
        # json_str = json.dumps(self.json_data, ensure_ascii=False, indent=4)
        json_str = json.dumps(self.json_data,indent=1)
        # write to json
        with open(self.path, 'w') as json_file:
	        json_file.write(json_str)

        pass

    '''update key value'''
    def update_data2(self,key1,key2,value):
        self.json_data[key1][key2] = value
        # json to  str
        json_str = json.dumps(self.json_data,indent=1)
        # write to json
        with open(self.path, 'w') as json_file:
	        json_file.write(json_str)

        pass


    '''print'''
    def print_json(self):
        print(json.dumps(self.json_data,indent=1))
        pass

    pass

global Json_g
Json_g = Json_c('C:config.json')


# def main():
#     test = Json_c('C:config.json')
#     test.read_level1("place_pos")
#     test.read_level2("place_pos","red")
#     test.print_json()
#     test.update_data1("red_hsv",[108, 190, 59, 255, 70, 255])
#     test.print_json()
#     # test.add("name","789456123456789456")
#     # test.print_json()

#     pass

# if __name__ == "__main__":
#     main()
#     pass


