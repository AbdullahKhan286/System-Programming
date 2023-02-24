#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>

int doesCommandExistInDir(char *, char *);

int main(int argc, char *argv[]) {
    // ------------ Get environmet variable "PATH"
    char *pathvar;
    pathvar = getenv("PATH");

    // ------------ Loop through all the environmet variables
    int i = 0, j = 0;
    char envi[1000];

    while (pathvar[i] != '\0') {
        if (pathvar[i] != ':') {    // Environment Variables Separted by :
            envi[j] = pathvar[i];
            j++;

        } else {
            // ------ Here we have a path and can peform search in it for our command.
            int result = doesCommandExistInDir(envi, argv[1]);

            if (result == 1) {    // If command is found in this path, print and break
                printf("%s\n", envi);
                exit(0);    // End the program
            }
            // ------- Clear the environemnt variable string for next iteration
            memset(envi, 0, sizeof envi);
            j = 0;  
        }
        i++;
    }

    printf("Command Not Found\n");
    return 0;
}

int doesCommandExistInDir(char * dirPath, char * commandName) {
    DIR *dirp = opendir(dirPath);

    if (dirp == NULL) {
        perror("ERROR!! Failed to open. ");
        return -1;    //Go out of the function
    }

    struct dirent *direntp;

    int result = 0;

    while(1) {
        direntp = readdir(dirp);

        if (direntp == NULL) {
            break;
        }

        char * fileName = direntp -> d_name;
        
        if (!strcmp(fileName, commandName)) {   // Returns 0 if both string match
            result = 1;       // 1 if the command is found in the path
            break;
        }
    }

    return result;
}