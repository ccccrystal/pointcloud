#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import socket
import numpy

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
ip_port = ('127.0.0.1',9999)
s.connect(ip_port)

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
print(inputlist)
print(type(inputlist[0]))

for data in inputlist:
    # lidar_data = bytes(data,encoding ='utf-8')
    # s.sendto(lidar_data, ('127.0.0.1', 9999))
    s.send(data.encode('utf-8'))

    # print(s.recv(1024).decode('utf-8'))

s.close()