import socket
import struct
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D  # 添加这个3d绘图功能
import numpy as np

#创建socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(('127.0.0.1', 8080)) #绑定与C++相同的地址和端口
server_socket.listen(1)

print(" Waitint for Connectinon ...")
conn, addr = server_socket.accept()
print(f"Connected by {addr}")

#创建Matplotlib图形窗口
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

point_size = struct.calcsize('fff')

#每次接收到的点云组
num_points_per_group = 100

while True:
    points = []
    for _ in range(num_points_per_group):
        data = conn.recv(point_size)
        if not data:
            break
        x, y, z = struct.unpack('fff', data)
        points.append([x,y,z])
    
    if len(points) == 0:
        break

    points = np.array(points)

    #清除前一组点云
    ax.cla()

    #更新并绘制新的点云
    ax.scatter(points[:, 0], points[:, 1], points[:, 2])
    ax.set_xlabel("x Label")
    ax.set_ylabel("Y Label")
    ax.set_zlabel("Z Label")

    #显示新的点云
    plt.pause(0.5)  #暂停一小段时间， 允许图形更新

# 关闭连接
conn.close()
plt.show()