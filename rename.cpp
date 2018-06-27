#include <stdio.h>  
#include <unistd.h>  
#include <dirent.h>  
#include <stdlib.h>  
#include <sys/stat.h>  
#include <string.h>  
#include <assert.h>
#include <iostream>
#include <ostream>
#include <sstream> 
using namespace std;

static unsigned int countRegPics = 0;
string SaveName;
char oldImageName[500];
char oldImageNameTmp[500];
char mvCommand[1000];
const char *saveNameTmp;
char dirnameToSave[500];
char dataHeader[50];
string dataHeaderString;

void listAllImageFilesToReg(const char *dirname)  
{  
    char fileNameTmp[500];
    char readPicName[500];

    static char dirNameTmp[500];
    int i, size;

    assert(dirname != NULL);  
      
    char path[512];
    struct dirent *filename; //readdir 的返回类型
    DIR *dir;  
      
    dir = opendir(dirname);  
    if(dir == NULL)
    {  
        printf("open dir %s error!\n",dirname);  
        exit(1);  
    }  
      
    while((filename = readdir(dir)) != NULL)  
    {  
        //目录结构下面问什么会有两个.和..的目录？ 跳过着两个目录
        if(!strcmp(filename->d_name,".")||!strcmp(filename->d_name,".."))
            continue; 
        size = strlen(filename->d_name);
        for(i = 0; i < size; i++)
        {
            fileNameTmp[i] = *(filename->d_name + i);
        }
        fileNameTmp[i] = '\0';
        sprintf(path,"%s/%s",dirname,filename->d_name);

        struct stat s;
        lstat(path,&s);
        
        if(S_ISDIR(s.st_mode))
        {
            memcpy(dirNameTmp, filename->d_name, strlen(filename->d_name));
            dirNameTmp[strlen(filename->d_name)] = '\0';

            printf("dir_name: %s\n", dirNameTmp);
            listAllImageFilesToReg(path);//递归调用
        }
        else
        {
            printf("fileNameTmp: %s\n", fileNameTmp);

            SaveName = "./outputImages/" + dataHeaderString + "_";
            ostringstream oss;
            oss << SaveName << countRegPics++;
            SaveName = oss.str();
            SaveName += ".jpg";
            cout << SaveName << endl;
            saveNameTmp = SaveName.data();

            // printf("saveNameTmp: %s\n", saveNameTmp);
            sprintf(oldImageName, "%s/%s/%s", dirnameToSave, dirNameTmp, fileNameTmp);
            // printf("oldImageName:%s\n", oldImageName);
            sprintf(oldImageNameTmp,"%s%s%s","\"",oldImageName,"\"");
            printf("oldImageNameTmp: %s\n", oldImageNameTmp);
            sprintf(mvCommand, "mv %s %s", oldImageNameTmp , saveNameTmp);
            printf("mvCommand :%s\n", mvCommand);
            system(mvCommand);
        }
    }
    closedir(dir);
}

int main(int argc, char const *argv[])
{
    system("mkdir outputImages");
    printf("%d\n", argc);
    if(argc < 3)
    {
        printf("execute like : bin dirname date(eg,2018062604);\n");
        return 0;
    }

    strcpy(dirnameToSave, argv[1]);
    strcpy(dataHeader, argv[2]);
    dataHeaderString = dataHeader;
    listAllImageFilesToReg(argv[1]);

    printf("number files: %d\n", countRegPics);
    return 0;
}