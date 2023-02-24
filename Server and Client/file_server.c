#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h> 

char buff[255];
char response[2000];
void listAllFiles(char *);
int isDir();

int main(int argc, char *argv[]) {
    int r = mkfifo("filesfifo", S_IRUSR | S_IWUSR);
    if (r == -1 && errno != EEXIST) {
        perror("File Creation Failed!");
        return -1;
    }

    int fd = open("filesfifo", O_RDWR);
    if (fd == -1) {
        perror("Failed to open!");
        return -1;
    }
    
    int dirLevel = 0;

    while (1) {
        fd_set set;
        struct timeval timeout;
        
        FD_ZERO(&set);
        FD_SET(fd, &set); /* add our file descriptor to the set */

        timeout.tv_sec = 20;    // 20 seconds
        timeout.tv_usec = 0;

        int rv = select(fd + 1, &set, NULL, NULL, &timeout);
        if (rv == -1) {
            perror("Select Error: ");

        } else if (rv == 0) {
            printf("Timed Out!\n");
            break;

        } else {
            int br = read(fd, buff, 1000);
            sleep(1);
        }

        if (strstr(buff, "req_ls") != NULL) {
            listAllFiles(".");

            write(fd, response, sizeof(response));
            sleep(1);
            memset(response, 0, sizeof response);

        } else if (strstr(buff, "*") != NULL) {
            memmove(buff, buff+1, strlen(buff));        // Remove the Star

            if (!isDir()) {
                write(fd, "Sending File", 12);

                int fileFd = open(buff, O_RDONLY);
                if (fileFd == -1) {
                    perror("Failed to open File.");
                }
                char fileCon[255] = "";
                read(fileFd, fileCon, 1000);
                
                sleep(5);

                write(fd, fileCon, sizeof(fileCon));
                sleep(1);

                // Go back to default dir
                for (int i = 0; i < dirLevel; i++) {
                    chdir("..");
                }
                dirLevel = 0;

            } else {
                write(fd, "Listing Dir", 11);
                sleep(5);

                listAllFiles(buff);
                write(fd, response, sizeof(response));
                sleep(1);
                memset(response, 0, sizeof response);

                chdir(buff);
                dirLevel++;
            }
        }
    }
    
    return 0;
}

void listAllFiles(char * dirPath) {
    DIR *dirp = opendir(dirPath);
    if (dirp == NULL) {
        printf("dirPath%s\n", dirPath);
        perror("ERROR!! Failed to open. ");
        return;
    }

    struct dirent *direntp;
    struct stat statbuff;

    while(1) {
        direntp = readdir(dirp);
        if (direntp == NULL) {
            break;
        }

        char * name = direntp -> d_name;
        if (name[0] == '.') {
            continue;
        }
        strcat(response, name);
        strcat(response, "\n");
    }
}

int isDir() {
    struct dirent *direntp;
    struct stat statbuff;

    DIR *dirp = opendir(".");
    if (dirp == NULL) {
        perror("ERROR!! Failed to open. ");
    }
    direntp = readdir(dirp);
    if (direntp == NULL) {
        perror("Direntp is Null");
    }

    int statRet = stat(buff, &statbuff);
    if (statRet == -1) {
        perror("Stat Failed: ");
    }

    if (S_ISDIR(statbuff.st_mode)) {
        return 1;
    } else {
        return 0;
    }
}