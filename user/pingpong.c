#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int
main(int argc, char *argv[])
{
    int p1[2];
    pipe(p1);
    int p2[2];
    pipe(p2);
    char s = 'b';
    write(p1[1],&s,1);
    if (fork() == 0){
        char *buf =  malloc(sizeof(char));
        read(p1[0], buf, 1);
        printf("%d: received ping\n", getpid());
        write(p2[1],buf,1);
        exit(0);
    }else{
        read(p2[0],0,1);
        printf("%d: received pong\n", getpid());
        wait(0);
    }
    exit(0);
}
