#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void sieve(int read_fd){
    int prime;
    if (read(read_fd, &prime, 4) <= 0){
        close(read_fd);
        exit(1);
    }
     printf("prime %d\n", prime);
     int p[2];
     pipe(p);
     int num;
     if (fork() == 0){
        close(p[1]);
        sieve(p[0]);
     }else{
        close(p[0]);
        while(read(read_fd, &num, 4) > 0){
            if (num % prime != 0){
                write(p[1], &num, 4);
            }
        }
        close(read_fd);
        close(p[1]);
        wait(0);
     }
     exit(0);
    
}

int main(){
    int p[2];
    pipe(p);
    if (fork() == 0){
        close(p[1]);
        sieve(p[0]);
    }else{
        for (int i = 2; i < 36; i++){
        write(p[1], &i, 4);
        }
        close(p[0]);
        close(p[1]);
        wait(0);
    }
    exit(0);
}