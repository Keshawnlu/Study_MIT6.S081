#include "kernel/types.h"
#include "user/user.h"



/**
 * @brief 读取左邻居的数据，将不能被first整除的写入右邻居
 * @param lpipe 左邻居的管道符
 * @param rpipe 右邻居的管道符
 * @param first 左邻居的第一个数据
 */
void transmit_data(int* lpipe, int* rpipe, int first){

    int data;
    while(read(lpipe[0], &data, sizeof(data)) == sizeof(data)){

        if(data % first != 0)
            write(rpipe[1], &data, sizeof(data));
    }

    close(lpipe[0]);
    close(rpipe[1]);
}

void prime(int* lpipe){

    close(lpipe[1]);    //关闭左邻居写通道

    int num;

    //读左邻居的第一个数
    if(read(lpipe[0], &num, sizeof(num)) == 0){

        //无数据
        close(lpipe[0]);
        exit(0);
    }
    else{

        //读到数据，输出
        printf("prime %d\n", num);
    }

    int rpipe[2];  
    pipe(rpipe);    //建立右邻居管道

    //读取左邻居的数据，将不能被num整除的写入右邻居
    transmit_data(lpipe, rpipe, num);

    if(fork() == 0){

        prime(rpipe);   //递归
    }
    else{

        //父进程
        close(rpipe[0]);    //关闭右邻居读通道
        wait(0);    //等待子进程结束
    }

    exit(0);
}

int main(int argc, char *argv[]) {

    int lpipe[2];
    pipe(lpipe);

    for(int i = 2; i <= 35; ++i){

        write(lpipe[1], &i, sizeof(i)); //将初始数据写入左邻居
    }

    if(fork() == 0){

        prime(lpipe);
    }
    else{

        close(lpipe[0]);
        close(lpipe[1]);
        wait(0);
    }

    exit(0);
}