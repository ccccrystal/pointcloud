import os
from plyfile import PlyData, PlyElement
import numpy as np
import pandas as pd

file_dir = r"C:/Users/ljj94/Documents/5D/Lidar/pointcloud/ZED/000000.ply"  # 文件的路径
plydata = PlyData.read(file_dir)  # 读取文件
data = plydata.elements[0].data  # 读取数据
data_pd = pd.DataFrame(data)  # 转换成DataFrame, 因为DataFrame可以解析结构化的数据
print(data_pd.shape)
print(data_pd)
np_shape = (data_pd.shape[0],data_pd.shape[1]-2)
# data_np = np.zeros(data_pd.shape, dtype=np.float)  # 初始化储存数据的array
data_np = np.zeros(np_shape, dtype=np.float)  # 初始化储存数据的array

property_names = data[0].dtype.names[:3]  # 读取property的名字
for i, name in enumerate(property_names):  # 按property读取数据，这样可以保证读出的数据是同样的数据类型。
    data_np[:, i] = data_pd[name]
print(data_np)
print(type(data_np[0][1]))

# def read_ply(filename):
#     """ read XYZ point cloud from filename PLY file """
#     plydata = PlyData.read(filename)
#     # pc = plydata['vertex'].data
#     pc = plydata.elements[0].data
#     pc_array = np.array([[x, y, z] for x,y,z in pc])
#     return pc_array
#
# if __name__ == "__main__":
#     read_ply(file_dir)
