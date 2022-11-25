# -*- coding: utf-8 -*-
"""
@ author: XiNG
@ datetime: 2020/7/28 16:41
@ software: PyCharm
"""
import os
path = r"C:/Users/ljj94/Documents/5D/Lidar/pointcloud/ZED/"

filename_list = os.listdir(path)
a = 0
for index,i in enumerate(filename_list):
    used_name = path + filename_list[a]
    new_name = path + '{:0>6}'.format(index) + '.ply'
    os.rename(used_name,new_name)
    print("文件%s重命名成功,新的文件名为%s" %(used_name,new_name))
    a += 1