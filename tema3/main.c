#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

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

    int pfd[2], nr_bytes, pfd2[2];

    pid_t pid_fiu;

    char readbuffer[2];
    signal(SIGUSR1, readSig1);
    signal(SIGUSR2, readTotal);

    pipe(pfd);
    pipe(pfd2);

    if((pid_fiu = fork()) == -1){
        perror("fork");
        exit(1);
    }

    if(pid_fiu == 0){
        // proces fiu
        int chrsRecivedTotal = 0;
        int chrsRecivedAfterSig = 0;
        int charAtSigs[5];
        int j = 0;
        while(!receiveEnd){
            nr_bytes = read(pfd[0], readbuffer, sizeof(readbuffer));
            if(nr_bytes > 0){
                chrsRecivedTotal += charCount(readbuffer);
                //printf("%s", readbuffer);
                if(receiveAtSig){
                    charAtSigs[j] = chrsRecivedTotal;
                    printf("Child Recv after %d sec: %d\n", j+1, charAtSigs[j]);
                    receiveAtSig = 0;
                    j++;
                }
            } 
        }     
        charAtSigs[4] = chrsRecivedTotal;  
        printf("Child recv total: %d\n\n", chrsRecivedTotal);
       write(pfd2[1], &chrsRecivedTotal, sizeof(chrsRecivedTotal));
       write(pfd2[1], &charAtSigs, sizeof(charAtSigs));
    }
    else{
        // proces parinte
        int n;
        int charAtSigs[5];
        time_t currentTime;
        time_t t1 = time(0) + 5;
        time_t t2 = time(0) + 1;
        while(time(0) < t1){
            currentTime = time(0);
            write(pfd[1], "a", (strlen("a") + 1));
            if(currentTime == t2){
                kill(pid_fiu, SIGUSR1);
                t2 = currentTime + 1;
            }       
        }
        kill(pid_fiu, SIGUSR2);
        nr_bytes = read(pfd2[0], &n, sizeof(n));
        if(nr_bytes > 0){
            printf("Parent recv total: %d\n", n);
        }
        nr_bytes = read(pfd2[0], &charAtSigs, sizeof(charAtSigs));
        int i = 0;
        for(i=0; i<5; i++){
            printf("Parent Recv after %d sec: %d\n", i+1, charAtSigs[i]);
        }
    }

    return 0;
}