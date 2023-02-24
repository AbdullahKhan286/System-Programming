#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>    //File Details

void listAllFiles(char *);
void searchForFile(char *, char *);

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("Need more arguments!\n");
        return -1;

    } else if (argc == 2) {   // e.g: ./find /bin ---> List all the files within this folder 
        listAllFiles(argv[1]);

    } else if (argc == 3) {   //e.g: ./find . -name
        if (!strcmp(argv[2], "-name")) {
            printf("find: -name: requires additional arguments\n");
            return -1;
        }
        
    } else if (argc == 4) {   //e.g: ./find /bin -name ls
        if (!strcmp(argv[2], "-name")) {
            searchForFile(argv[1], argv[3]);
        }
    }

    return 0;
}

void listAllFiles(char * dirPath) {
    DIR *dirp = opendir(dirPath);

    if (dirp == NULL) {
        perror("ERROR!! Failed to open. ");
        printf("%s\n\n", dirPath);
        return;    //Go out of the function
    }

    struct dirent *direntp;
    struct stat statbuff;

    while(1) {
        direntp = readdir(dirp);

        if (direntp == NULL) {
            break;
        }

        char * name = direntp -> d_name;

        if (name[0] == '.') {    //Filter the hidden files
            continue;
        }
        
        char path[100];
        strcat(path, dirPath);
        strcat(path, "/");
        strcat(path, name);

        int statRet = stat(path, &statbuff);
        if (statRet == -1) {
            perror("Stat Failed");
        }

        printf("%s\n", path);

        if (S_ISDIR(statbuff.st_mode)) {     //If it's a directory call the listAllFiles() function again
            listAllFiles(path);
        } 
        memset(path, 0, sizeof path);   //Clear the path string
    }
}

void searchForFile(char * dirPath, char * fileName) {
    DIR *dirp = opendir(dirPath);

    if (dirp == NULL) {
        perror("ERROR!! Failed to open. ");
        printf("%s\n\n", dirPath);
        return;    //Go out of the function
    }

    struct dirent *direntp;
    struct stat statbuff;

    while(1) {
        direntp = readdir(dirp);

        if (direntp == NULL) {
            break;
        }

        char * name = direntp -> d_name;

        if (name[0] == '.') {    //Filter the hidden files
            continue;
        }
        
        char path[100];
        strcat(path, dirPath);
        strcat(path, "/");
        strcat(path, name);

        if (strstr(name, fileName)) {     // If filename contains in name
            printf("%s\n", path);
        }

        int statRet = stat(path, &statbuff);
        if (statRet == -1) {
            perror("Stat Failed");
        }

        if (S_ISDIR(statbuff.st_mode)) {     //If it's a directory call the listAllFiles() function again
            searchForFile(path, fileName);
        } 
        memset(path, 0, sizeof path);   //Clear the path string
    }
}