#include "main.h"

int main()
{
    int sock_fd, conn_fd;
    struct sockaddr_in server_addr{}, client{};
    int port = atoi(PORT);  //获取端口
    //设置服务端的 sockaddr_in
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //创建socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
    {
        printf("socket error\n");
        return 1;
    }
    //绑定
    int ret = bind(sock_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (ret < 0)
    {
        printf("bind error\n");
        return 1;
    }
    //监听
    ret = listen(sock_fd, 10);
    if (ret < 0)
    {
        printf("listen error\n");
        return 1;
    }
    //创建线程池
    threadpool<task> pool(20);
    //线程池开始运行
    pool.start();

    while (true)
    {
        socklen_t len = sizeof(client);
        //接受连接
        conn_fd = accept(sock_fd, (struct sockaddr *) &client, &len);
        printf("一个连接进入...\n");
        if (conn_fd == 0)
        {
            printf("accept error...\n");
            break;
        }
        task *ta = new task(conn_fd);
        //向线程池添加任务
        pool.append_task(ta);
    }
    return 0;
}