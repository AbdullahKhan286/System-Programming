#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int fd = open("filesfifo", O_RDWR);
    if (fd == -1) {
        perror("Failed to open!");
        return -1;
    }
    
    // Send Request to receive the contents of the directory
    char req[] = "req_ls";
    write(fd, req, sizeof(req));
    sleep(2);
    //------------------

    while (1) {
        // Wait for the response
        char res[1000];
        read(fd, res, 1000);
        sleep(1);
        printf("%s", res);
        memset(res, 0, sizeof res);
        //------------------

        // Ask the name of file from the user
        char name[255];
        printf("Enter the File/Dir Name: ");
        scanf("%s", name);
        //------------------

        // Send File/Dir Name to server
        char reqCont[255] = "";
        strcat(reqCont, "*");    // * removed at the server end
        strcat(reqCont, name);
        write(fd, reqCont, sizeof(reqCont));
        sleep(1);
        //------------------

        // Wait for the response
        char typeRes[1000] = "";
        read(fd, typeRes, 1000);
        sleep(1);

        printf("%s\n", typeRes);

        if (strstr(typeRes, "Sending File") != NULL) {      // Receive the File Content
            // Read the file Content
            read(fd, res, 1000);
            sleep(1);
            printf("File Content: %s\n", res);
            
            break;

        }
        memset(res, 0, sizeof res);
    }

    return 0;
}