#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>

void copy_stuff(char * source, char * dest){
    char buffer[256];
    struct stat buf;
    stat(source,&buf);
    ssize_t readb;
    int fdSource = open(source, O_RDONLY);
    if(fdSource == -1){
        printf("Can't open!\n");
        return;
    }
    printf("%s\n", source);
    int fdDest = open(dest, O_WRONLY | O_CREAT, buf.st_mode );
    printf("%s\n", dest);
    if(fdDest == -1){
        printf("Can't create file");
        return;
    }
    
    do
    {    
      readb=read(fdSource,&buffer,256);
      write(fdDest, &buffer,readb);
    } while(readb==256);

    close(fdSource);
    close(fdDest);
}

void link_stuff(char * source, char * dest){

    int fdSource = open(source, O_RDONLY);
    if(fdSource == -1){
        printf("Can't open!\n");
        return;
    }
    printf("%s\n", source);
    
    int linked = symlink(source, dest);
    if(linked == -1){
        printf("Can't link file.");
    }

    close(fdSource);
}

void traverse_folder(char * path, char * dest, int mode){
    DIR * d = opendir(path);
    if(d == NULL) return;
    struct dirent * dir;
    struct stat buf;
    stat(path,&buf);

    while((dir = readdir(d)) != NULL){
        if(dir->d_type != DT_DIR){
            printf("\t%s\n", dir->d_name);
            char d_path[257];
            char to_path[257];
            sprintf(d_path, "%s/%s", path, dir->d_name);
            sprintf(to_path, "%s/%s", dest, dir->d_name);
            if(mode == 0){
                copy_stuff(d_path, to_path);
            }
            if(mode == 1){
                link_stuff(d_path, to_path);
            }
        }
        else
        {
            if(dir->d_type == DT_DIR && strcmp){
                printf("%s\n", dir->d_name);
                char d_path[257];
                char to_path[257];
                sprintf(d_path, "%s/%s", path, dir->d_name);
                sprintf(to_path, "%s/%s", dest, dir->d_name);
                mkdir(to_path, buf.st_mode);
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

    if(opendir(dest) == NULL){
        mkdir(dest, S_IRWXU | S_IRWXG | S_IRWXO);
    }

    traverse_folder(source, dest, mode);

    return 0;
}