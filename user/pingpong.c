#include "kernel/types.h"
#include "user/user.h"


int main(int argc, char *argv[]) {

    int parent_fd[2];   //0为读，1为写
    int child_fd[2];    //0为读，1为写

    pipe(parent_fd);    //创建管道
    pipe(child_fd);

    char buf_ping[4];
    char buf_pong[4];

    int pid = fork();
    if(pid == 0){

        //子进程
        close(parent_fd[1]);    //关闭父进程的写端
        close(child_fd[0]);     //关闭自己的读端
        
        if(read(parent_fd[0], buf_ping, sizeof(buf_ping)) == 4){    //收到4个字节

            printf("Child_pid = %d, 收到ping\n", getpid());
        }

        write(child_fd[1], "pong", 4);  //往管道写

        close(parent_fd[0]);
        close(child_fd[1]);
        exit(0);    //关闭，正常退出
    }
    else if(pid > 0){

        //父进程
        close(parent_fd[0]);    //关闭自己的读端
        close(child_fd[1]);     //关闭子进程的写端

        write(parent_fd[1], "ping", 4); //先写

        wait((int*)0);  //等待子进程返回

        if(read(child_fd[0], buf_pong, sizeof(buf_pong)) == 4){

            printf("Parent_pid = %d, 收到pong\n", getpid());
        }

        close(parent_fd[1]);
        close(child_fd[0]);
        exit(0);    //关闭，正常退出
    }
    else{

        printf("fork error\n");
        close(parent_fd[0]);
        close(parent_fd[1]);
        close(child_fd[0]);
        close(child_fd[1]);
        exit(1);
    }
}