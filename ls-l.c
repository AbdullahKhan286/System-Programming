#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>    
#include<time.h>
#include<string.h>
#include<pwd.h>
#include<grp.h>

int main(int argc, char* argv[]) {
    DIR * dirp = opendir(argv[1]);

    if (dirp == NULL) {
        perror("Failed to open dir.");
        return -1;
    }

    struct dirent * direntp;
    struct stat statbuff;

    while (1){
        direntp = readdir(dirp);

        stat(direntp -> d_name, &statbuff);

        if (S_ISDIR(statbuff.st_mode)) {                 //Check if is directory
            printf("d");
        } else {
            printf("-");
        }

        //--------------- User Permissions -------------------------------
        if (S_IRUSR & statbuff.st_mode) {                 //Check if user has read permission (bit-wise AND)
            printf("r");
        } else {
            printf("-");
        }
        if (S_IWUSR & statbuff.st_mode) {                 //Check if user has write permission
            printf("w");
        } else {
            printf("-");
        }
        if (S_IXUSR & statbuff.st_mode) {                 //Check if user has execute permission
            printf("x");
        } else {
            printf("-");
        }

        //--------------- Group Permissions -------------------------------
        if (S_IRGRP & statbuff.st_mode) {                 //Check if group has read permission
            printf("r");
        } else {
            printf("-");
        }
        if (S_IWGRP & statbuff.st_mode) {                 //Check if group has write permission
            printf("w");
        } else {
            printf("-");
        }
        if (S_IXGRP & statbuff.st_mode) {                 //Check if group has execute permission
            printf("x");
        } else {
            printf("-");
        }

        //--------------- Other Permissions -------------------------------
        if (S_IROTH & statbuff.st_mode) {                 //Check Other has read permission
            printf("r");
        } else {
            printf("-");
        }
        if (S_IWOTH & statbuff.st_mode) {                 //Check Other has write permission
            printf("w");
        } else {
            printf("-");
        }
        if (S_IXOTH & statbuff.st_mode) {                 //Check Other has execute permission
            printf("x");
        } else {
            printf("-");
        }

        char *time = ctime(&statbuff.st_atimespec.tv_sec);   //Getting time string

        //----------- Removing new line from time string-------------
        int i, len;
        len = strlen(time);

        char *newTime;
        newTime = (char *) malloc(len-1);
        for(i = 0; i < strlen(time)-1; i++) {
            newTime[i] = time[i]; 
        }
        //-----------------------------------------------------------

        printf("\t%03d\t", statbuff.st_nlink);     //Number of links

        struct group *grp;
        struct passwd *pwd;
        grp = getgrgid(statbuff.st_gid);
        pwd = getpwuid(statbuff.st_uid);

        printf("%s\t", pwd -> pw_name);            //Group name
        printf("%s\t", grp -> gr_name);            //User name
        printf("%06lld\t", statbuff.st_size);    //Number of bytes
        printf("%s\t", newTime);                   //Time
        printf("%s\n", direntp -> d_name);         //Name

        if(direntp == NULL)
            break;
    }
    
    return 0;
}
