#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "../include/cmp.h"
#include "../include/hash1.1.h"
#include "../include/write.h"
int main(int argc,char *argv[])
{
    char buf[MAX] = {0};
    char comm[256] = {0};
    char *pid;
    char SHA1[41];
    FILE *file;
    char buf1[128][512];
    char *path;
    char rootpath[512];
    int len = 0;
    int num;
    int pathlen =64;//缓存项中文件名长度
    // const int filePosition = 53;//在
    // const int typePosition = 26;//
    if (argc < 3)
    {
        printf("argument num error\n");
        return -1;
    }
    path = argv[1];//路径
    pid = argv[2];

    if(ModifyStr(sepc,path,argv[2]) == -1)
    {
        return -1;
    }
    
    
    /****** 查找缓存 *****/
    wfile = fopen("Cache", "r+");
    fgets(filenum, 512, wfile);
    printf("%s",filenum);
    sum = atoi(filenum);
    sumlen = strlen(filenum);
    if(pathfind(sepc,buf,wfile) == 0)
    {
        //cache中没找到相应的文件
        fseek(wfile, sumlen, SEEK_SET);
        num = myread(filebuf,wfile);
        /****计算HASH值*****/
        //printf("%s\n",path);
        getRootpath(rootpath,path,pid);
        sprintf(SHA1,"%08x%08x%08x%08x%08x",state[0],state[1],state[2],state[3],state[4]);
        sprintf(sepc+pathlen,"%08x%08x%08x%08x%08x \n",state[0],state[1],state[2],state[3],state[4]);
        strcpy(filebuf[num-1],sepc);
        mywrite(filebuf,num,wfile);
        //printf("SHA1:%s\nsepc:%s",SHA1,sepc);
    }
    else{
        //printf("buf:%s",buf);
        //printf("len:%ld\n",strlen(buf));
        sprintf(SHA1,"%s",buf+pathlen);
    }


    /****查找缓存失败，计算HASH值*****/
    // 排除/proc/self
    // if(strlen(path) >= 11)
    // {
    //     memcpy(inroot,path,11);
    //     if(!strcmp(inroot,"/proc/self/"))
    //     {
    //         return 0;
    //     }
    // }
    printf("SHA1:%s\n",SHA1);
    /****** 特征匹配 *****/
    file = fopen("hash.txt", "r");
    fgets(buf, 17, file);
    sum = atoi(buf);
    // printf("%s\n",argv[1]);
    sumlen = strlen(buf);
    //printf("sum:%d sumlen:%d\n",sum,sumlen);
    
    for (int i = 0; i < sumlen - 1; i++)
    {
        if (buf[i] < 48 || buf[i] > 57)
        {
            printf("sum error");
            return -1;
        }
    }


    // printf("%s\n",argv[1]);
    // fgets(buf,42,file);
    if (find(SHA1, buf, file))
    {
        printf("find and killed %s %s\n", buf, pid);
        // sprintf(comm, "kill %s", pid);
        // system(comm);
    }
    fclose(file);
    
    return 0;
}