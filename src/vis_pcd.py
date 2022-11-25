import open3d as o3d
import numpy as np
import os
import glob

# datas = './cam2liar_test_data/pcd'
# data = np.fromfile(dir, dtype=np.float32).reshape(-1, 4)

datas = glob.glob('0002.pcd')
print(datas)
for data in datas:
    print(data)
    pcd = o3d.io.read_point_cloud(data)
    print(np.array(pcd))
    o3d.visualization.draw_geometries([pcd])
