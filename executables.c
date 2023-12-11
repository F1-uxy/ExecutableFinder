#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

int printExecutables(char* path)
{

    struct stat sb;
    DIR *dir;
    dir = opendir(path);

    while (dir == NULL)
    {
        perror("Unable to open directory");
        return 1;
    }

    struct dirent *file; //Struct of type dirent which holds info about the current file

    while ((file = readdir(dir)) != NULL) //Iterates through files in directory until NULL
    {
        if(file->d_type == DT_REG) //Check if is a regular file, avoids . and ..
        {
            //Passes filepath into stat and checks if the stats return say user have execute permission from the file owner
            //st_mode = file permission, S_IXUSR = bit mask for the execute permissions for the owner
            //& Bitewise AND op to check specific bit in the st_mode field of the struct stat
            if (stat(("%s%s",path,file->d_name), &sb) == 0 && sb.st_mode & S_IXUSR)
            {
                printf("%s\n", file->d_name);
            }
        }
    }
}



int main(int argc, char *argv[])
{

    const char delim[1] = ":";
    char* token;
    char currdir[100];

    if(argc == 1) //If no args then use current directory
    {
        getcwd(currdir, sizeof(currdir));
        printExecutables(currdir);
        return 0;
    }

    else if(argc == 2  && strcmp(argv[1], "") != 0) //If 2nd arg isn't empty
    {

        token = strtok(argv[1], delim);

        while(token != NULL)
        {

            if (chdir(token) != 0)
            {
                perror("chdir error ");
                printf("%s\n", token);
                return 1;
            }else
            {
                getcwd(currdir, sizeof(currdir));
                printExecutables(currdir);
            }

            token = strtok(NULL, ":");

        }
    }


    return 0;
}
