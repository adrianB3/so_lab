#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include<sys/wait.h>

static int receiveEnd = 0;
static int receiveAtSig = 0;

int charCount(char * str){
    int i;
    int charCount = 0;

    for(i=0;str[i];i++){
        if(str[i] != ' '){
            charCount++;
        }
    }

    return charCount;
}

void readTotal(int sig){
    if(sig == SIGUSR2){
        receiveEnd = 1;
    }
}

void readSig1(int sig){
    if(sig == SIGUSR1){
        receiveAtSig = 1;
    }
}

int main(){

    int pfd[2], nr_bytes;

    pid_t pid_fiu;

    char readbuffer[2];
    signal(SIGUSR1, readSig1);
    signal(SIGUSR2, readTotal);

    pipe(pfd);

    if((pid_fiu = fork()) == -1){
        perror("fork");
        exit(1);
    }

    if(pid_fiu == 0){
        // proces fiu
        int chrsRecivedTotal = 0;
        int chrsRecivedAfterSig = 0;

        while(!receiveEnd){
            nr_bytes = read(pfd[0], readbuffer, sizeof(readbuffer));
            if(nr_bytes > 0){
                chrsRecivedTotal += charCount(readbuffer);
                printf("%s", readbuffer);
            }
        }
        
        printf("total: %d", chrsRecivedTotal);
    }
    else{
        // proces parinte
        
        while(SIGALRM){
            write(pfd[1], "a", (strlen("a") + 1));
        }
        kill(pid_fiu, SIGUSR2);
    }

    return 0;
}