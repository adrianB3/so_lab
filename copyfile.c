#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>

void copy_stuff(char * source, char * dest){
    int fdSource = open(source, O_RDONLY);
    if(fdSource == -1){
        printf("Can't open!\n");
    }
    int fdDest = open(dest, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    if(fdDest == -1){
        printf("Can't create/read file");
    }
}

void link_stuff(char * source, char * dest){
    
}

void traverse_folder(char * path, char * dest, int mode){
    DIR * d = opendir(path);
    if(d == NULL) return;
    int fdDest = open(dest, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    struct dirent * dir;

    while((dir = readdir(d)) != NULL){
        if(dir->d_type != DT_DIR){
            printf("\t%s\n", dir->d_name);
            char to_path[255];
            sprintf(to_path, "%s/%s", dest, dir->d_name);
            copy_stuff(path, to_path);
        }
        else
        {
            if(dir->d_type == DT_DIR && strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0){
                printf("%s\n", dir->d_name);
                char d_path[255];
                char to_path[255];
                sprintf(d_path, "%s/%s", path, dir->d_name);
                sprintf(to_path, "%s/%s", dest, dir->d_name);
                mkdir(to_path, S_IRWXU | S_IRWXG | S_IRWXO);
                traverse_folder(d_path, to_path, mode);
            }
        }
    }
    closedir(d);
}

int main(int argc, char **argv){

    char * source = argv[1];
    char * dest = argv[2];
    int mode = atoi(argv[3]);

    traverse_folder(source, dest, mode);

    return 0;
}