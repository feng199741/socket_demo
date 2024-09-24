#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>   // 用于htons()和inet_pton()函数
#include <unistd.h>
#include <chrono>
#include <cstdlib>
#include <thread>

struct Point
{
    float x, y, z;
};

std::vector<Point> generatePointCloud(int num_points)
{
    std::vector<Point> points;
    for (int i = 0; i < num_points; i++)
    {
        Point p = {static_cast<float>(rand() % 100) / 10.0f,
                    static_cast<float>(rand() % 100) / 10.0f,
                    static_cast<float>(rand() % 100) / 10.0f};
        points.push_back(p);       
    }
    return points;
}

//通过socket发送点云数据
void sendPointCloud(int sockfd, std::vector<Point>& points)
{
    for (const auto& p : points)
    {
        send(sockfd, &p, sizeof(Point), 0);  //发送数据
    }
}

int main()
{
    int sockfd;
    struct sockaddr_in serv_addr;
    
    //创建socket
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cerr << "Socket creation error !!!" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;   // 使用 IPv4 协议
    serv_addr.sin_port = htons(8080); // 设置端口号为8080，并转换为网络字节序
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        std::cerr << "Invaild address / Address not supported !!! " << std::endl;
    }

    //连接到Python端服务器
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << "Connection failed !!! " << std::endl;
        return -1;
    }

    int num_points = 100; //每组点云的点数
    int num_iterations = 10; // 发送10组不同的点云
    for (int i = 0; i < num_iterations; i++)
    {
        std::vector<Point> points = generatePointCloud(num_points);
        sendPointCloud(sockfd, points);
        std::cout << " Sent point cloud group: " << i + 1 <<std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    close(sockfd);
    return 0;

}