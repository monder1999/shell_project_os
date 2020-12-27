#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(int argc, char *argv[])
{
    char loop[1000] = "shell $>";
    while (1)
    {
        char comm[100] = {0};
        int fillindex = 0;
        char ch;
        printf("%s", loop);
        scanf("%c", &ch);
        while (ch != '\n')
        {
            comm[fillindex] = ch;
            fillindex++;
            scanf("%c", &ch);
        }
        char *progname = strtok(comm, " ");
        char *argvx[100];
        char *argtoken = progname;

        if (progname == NULL)
        {
            continue;
        }
        if (strcmp(progname, "pause") == 0)
        {
            char x = 0;
            printf("press enter to resume");
            while (x!= '\n')
            {
                scanf("%c", &x);
            }
            continue;
        }
        if (strcmp(progname, "quit") == 0)
        {
            exit(0);
        }
        int index = 0;
        while (argtoken != NULL)
        {
            argvx[index] = argtoken;
            index++;
            argtoken = strtok(NULL, " ");
        }
        argvx[index] = NULL;

        if (strcmp(progname, "cd") == 0)
        {
            char current[1000];
            getcwd(current, 1000);
            char newpath[1000];
            char *buffer;

            if (strcmp(argvx[1], "..") != 0 && strcmp(argvx[1], "/..") != 0)
            {
                if (argvx[1][0] != '/')
                {
                    strcpy(current, strcat(current, "/"));
                }
                strcpy(newpath, strcat(current, argvx[1]));
                if (chdir(newpath) != 0)
                {
                    perror("failed to get current directory\n");
                    continue;
                }
                char newloop[100] = "shell $>~";
                strcpy(loop, strcat(strcat(newloop, newpath), ">>"));
                continue;
            }
            else
            {
                chdir(strcat(current, "/.."));
                getcwd(newpath, 1000);
                char newloop[100] = "shell $>~";
                strcpy(loop, strcat(strcat(newloop, newpath), ">>"));
                continue;
            }
        }
        

        if (strcmp(progname, "dir") == 0)
        {
            strcpy(progname, "ls");
            strcpy(argvx[0], "ls");
        }
        if (strcmp(progname, "clr") == 0)
        {
            strcpy(progname, "clear");
            strcpy(argvx[0], "clear");
        }
        if (strcmp(progname, "environ") == 0)
        {
            strcpy(progname, "env");
            strcpy(argvx[0], "env");
        }
        if (strcmp(progname, "help") == 0)
        {
            strcpy(progname, "more");
            argvx[0]= "more";
            argvx[1]="help.txt";
            argvx[2]= NULL;
        }
        int gp=-1;
        int i=0;
        while(argvx[i]!=0){
            if(strcmp(argvx[i],">")==0){
                gp=i;
            }
            i++;
        }
        if (gp != -1)
        {char *argvx2[gp];
        for (int i = 0; i < gp; i++)
        {
          argvx2[i]=argvx[i]; 
        }
        argvx2[gp] = NULL;
        int x = open(argvx[gp+1], O_RDWR | O_CREAT, 0777);
        int m = fork();
        if (m == 0)
        {
            dup2(x, 1);
            execvp(argvx2[0],argvx2);
        }
        else
        {
            wait(0);

        }
        continue ;
       }
       int bool=0;
       int index2 = 0;
       while (argvx[index2] !=NULL)
       {
           if (strcmp(argvx[index2],"&")==0)
           {   
                bool = index2;
           }
           index2++;
       }
       if(bool>0)
       argvx[bool]=NULL;
        pid_t forks = fork();
        if (forks == 0)
        {   
            execvp(progname, argvx);
        }
        else
        {   
             if (bool==0){
                 wait(NULL);
             }
                
        }
    }
}
