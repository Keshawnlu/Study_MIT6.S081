#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"


int main(int argc, char *argv[]) {

    if(argc < 2){

        printf("参数错误\n");
        exit(1);
    }

    char* cmd = argv[1];    //输入的命令
    char argv_list[MAXARG][100];    //10不能太大，100就报错
    char* new_argv[MAXARG];
    char buf; 

    while(1){

        memset(argv_list, 0, MAXARG * 100); //重要

        for(int i = 1; i < argc; ++i){

            strcpy(argv_list[i - 1], argv[i]); //传入参数
        }

         
        int index = argc - 1;
        int n = 0;
        int offset = 0;

        while((n = read(0, &buf, 1)) > 0){

            if(buf == ' '){

                ++index;
                offset = 0;
                continue;
            }

            if(buf == '\n')
                break;  //一行读完

            argv_list[index][offset++] = buf;
        }

        if(n <= 0)  //否则会无限循环打印
            break;

        for(int i = 0; i <= index; ++i)
            new_argv[i] = argv_list[i];

        if(fork() == 0){

            exec(cmd, new_argv);

        }
        else{

            wait(0);
        }
        
    }

    exit(0);
}