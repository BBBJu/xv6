#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"
int readLine(char *current_argv[MAXARG], int cur){
    char buf[1024];
    int n = 0;
    while(read(0, buf+n, 1)){
        if (buf[n] == '\n'){
            break;
        }
        n += 1;
    }
    if (n >= sizeof(buf) - 1) { // 防止缓冲区溢出
            fprintf(2, "line too long\n");
            exit(1);
    }
    buf[n] = 0;
    if (n == 0){
        return 0;
    }
   
    int offset = 0;
    while(offset < n){
         current_argv[cur++] = buf + offset;
        while (buf[offset] != ' ' && offset < n){
            offset += 1;
        }
        while (buf[offset] == ' ' && offset < n){
            buf[offset] = 0;
            offset += 1;
        }

    }

    
    return cur;
}
int main(int argc, char *argv[]) {
    if (argc <= 1){
        fprintf(2, "usage: xargs <command> <arg>\n");
        exit(1);
    }
    char *current_argv[MAXARG];
    for (int i = 1; i < argc; i++){
        current_argv[i-1] = argv[i];
    }
    int cur;
    while ((cur = readLine(current_argv, argc-1)) != 0){
        current_argv[cur] = 0;
        if(fork() == 0){
			exec(argv[1], current_argv);
			fprintf(2, "exec failed\n");
			exit(1);
		}
        wait(0);
    }
    exit(0);
}

