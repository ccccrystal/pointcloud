import open3d as o3d
import numpy as np
import os


def kitti():
    dir = "/home/today/KITTI/velodyne/training/velodyne/"
    files = os.listdir(dir)
    files.sort(key=lambda x: int(x[:-4]))
    for file in files:
        lidar = np.fromfile(dir + file, dtype=np.float32).reshape(-1, 4)
        # print(lidar[0])
        pcd = o3d.geometry.PointCloud()
        pcd.points = o3d.utility.Vector3dVector(lidar[:, :3])
        o3d.visualization.draw_geometries([pcd])


def my_datas():
    i = 0
    datas = np.asarray([[0, 0, 0, 0]])
    for line in open('/home/today/Lidar/208.txt', "r"):
        line = line.split()
        line = list((map(float, line)))
        i += 1
        if len(line) == 1:
            try:
                pcd = o3d.geometry.PointCloud()
                pcd.points = o3d.utility.Vector3dVector(datas[:, :3])
                o3d.visualization.draw_geometries([pcd])
                datas.tofile('000000.bin')
            except:
                continue
            datas = np.asarray([[0, 0, 0, 0]])
            continue
        data = np.asarray(line)
        datas = np.concatenate(([data], datas))


def my_data():

    lidar = np.fromfile('./000010.bin')
    # lidar = np.fromfile(fi_d).reshape(-1, 4)
    print(lidar)
    # pcd = o3d.geometry.PointCloud()
    # pcd.points = o3d.utility.Vector3dVector(lidar[:, :3])
    # o3d.visualization.draw_geometries([pcd])
    # o3d.io.write_point_cloud("0002.pcd", pcd)
    # o3d.io.write_point_cloud("0002.ply", pcd)


if __name__ == "__main__":
    # kitti()
    # my_datas()
    my_data()
