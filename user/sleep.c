#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char *argv[]) {

    if(argc != 2){

        //忘记传递参数
        printf("%s", "请输入睡眠时间\n");
        exit(1);    //异常退出
    }

    int time = atoi(argv[1]);
    sleep(time);

    exit(0);
}