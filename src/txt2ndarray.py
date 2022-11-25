# -*- coding: utf-8 -*-
import numpy as np
import pandas as pd

# file = np.loadtxt("test1.txt",dtype=str,delimiter=',')
# with open("test1.txt") as file:
#     for line in file:
    #     pass # do something
    #     input_list=line.strip()
    #     print(type(line))

# my_data = file.shape

def ReadTxtName(rootdir):
    lines = []
    with open(rootdir, 'r') as file_to_read:
        while True:
            line = file_to_read.readline()
            if not line:
                break
            line = line.strip('\n')
            lines.append(line)
    return lines
inputlist = ReadTxtName('test1.txt')

print(type(inputlist[0]))
# print(len(inputlist))
for i in range(len(inputlist)):
    input_data = np.array(inputlist[i].split()).reshape(-1,3)
    print(type(input_data[0][1]))
    np_shape = (input_data.shape[0],1)
    data_np = np.zeros(np_shape, dtype=np.float)
    result = np.concatenate([input_data, data_np], axis=1)
    result_data = np.float64(result)
    print(type(result_data[0][1]))
    # print(np.float32(result))


