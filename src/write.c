#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include "../include/write.h"
#define MAX 9999
const int ALLLEN = 106;
int PATHLEN = 64;
const int SHA1LEN = 40;
int sum;
int sumlen;
/***
 * 从文件file当前光标位置中读取每一行的内容放入数组buf中
 * 每一行内容不超过512个字节
 * 返回文件行数
 * ***/
int myread(char buf[][512],FILE *file)
{
    int num = 0;
    int ret;
    while ((ret = (size_t)fgets(buf[num++], 512, file)) != 0)
        ;
    return num;
}
/*快速排序*/
void W(char *p, int left, int right) //先去重
{
    //printf("left:%d right:%d\n",left,right);
    char flag[512];
    memcpy(flag,p+left*512,512);
    
    int l = left;
    int t = right;
    // int ret = right - left;
    //printf("%s12\n",flag);
    while (left < right)
    {
        if ( memcmp(flag,p+right*512,512) > 0)
        {
            memcpy(p+left*512,p+right*512,512);
            //*(p + left) = *(p + right);
            left++;
            while (left < right)
            {
                if (memcmp(flag,p+left*512,512) < 0)
                {
                    //*(p + right) = *(p + left);
                    memcpy(p+right*512,p+left*512,512);
                    right--;
                    break; 
                }
                else
                {
                    left++;
                }
            }
        }
        else
        {
            right--;
        }
    }
    if(left > right)
    {
        return;
    }
    //*(p + left) = flag;
    //printf("left:%d right:%d\n",left,right);
    //strcpy(p[left],flag);
    memcpy(p+left*512,flag,512);
    W(p, l, left - 1);
    W(p, right + 1, t);

}
/* 
    排序并将buf中的内容写入wfile文件中
    filesum是wfile文件的第一行
 * */
int mywrite(char buf[MAX][512],int num,FILE *wfile)
{
    int ret;
    char filenum[64];     //文件中总项数
    sprintf(filenum,"%d\n",num);
    //快速排序
   // printf("num:%d",num);
    // for(int i = 0 ; i < num; i++)
    // {
    //     printf("buf%s",buf[i]);
    //     //fwrite(buf[i],sizeof(char),strlen(buf[i]),wfile);
    //     //fwrite("\n",sizeof(char),1,wfile);
    // }
    W(buf,0,num-1);

    fseek(wfile,0,SEEK_SET);
    fwrite(filenum,sizeof(char),strlen(filenum),wfile);
    for(int i = 0 ; i < num; i++)
    {
        //printf("buf%s",buf[i]);
        fwrite(buf[i],sizeof(char),strlen(buf[i]),wfile);
        //fwrite("\n",sizeof(char),1,wfile);
    }
    
    return 0;
}

int pathfind(char *sepc, char *buf, FILE *file)
{
    size_t ret;
    int up = sum - 1;
    int down = 0;
    int mid = (up + down) / 2;
    int lastmid = -1;
    fseek(file, sumlen + mid * ALLLEN, SEEK_SET);
    //移动文件指针到mid处
    while ((ret = (size_t)fgets(buf, ALLLEN + 1, file)) != 0)
    //fgets 最后返回的字符串，后面要再加一个‘0’，所以读取想要的长度还需要加一个结束符的长度
    {
        if (memcmp(sepc, buf, PATHLEN) == 0)
        {
            //printf("ok\n");
            return 1;
        }
        else if (memcmp(sepc, buf, PATHLEN) > 0)
        {
            down = mid + 1;
            mid = (up + down) / 2;
            fseek(file, sumlen + mid * ALLLEN, SEEK_SET);
        }
        else
        {
            up = mid - 1;
            mid = (up + down) / 2;
            fseek(file, sumlen + mid * ALLLEN, SEEK_SET);
        }
        //printf("up: %d down: %dmid : %d hash:%s\n", up, down, mid, buf);
        if (up <= down + 1)
        {
            break;
        }
    }
    for (int i = down; i <= up; i++)
    {
        fseek(file, sumlen + i * ALLLEN, SEEK_SET);
        fgets(buf, ALLLEN+1, file);
        if (memcmp(sepc, buf, PATHLEN) == 0)
        {   

            //printf("ok\n");
            return 1;
        }
    }
    return 0;
}
/*
    retstr为最终写入的字符串
    path为传入的路径
    SHA1为传入的HASH值
*/
int ModifyStr(char *retstr,char* path,char *SHA1)
{
    if(strlen(path) > PATHLEN)
    {
        printf("string too long\n");
        return -1;
    }
    // if(strlen(SHA1) != SHA1LEN)
    // {
    //     printf("SHA1 length error\n");
    //     return -1;
    // }
    sprintf(retstr,"%s",path);
    int stringlen = strlen(retstr);
    for(int i = stringlen; i < PATHLEN; i++)
    {
        retstr[i] = ' ';
    }
    //sprintf(retstr+PATHLEN,"%s \n",SHA1);
}

// int main(int argc, char *argv[])
// {
//     char buf[512];
//     int num;
    
//     if(argc < 3)
//     {
//         printf("arguments too few\n");
//         return -1;
//     }
//     //限制传入参数大于等于三个
//     if(ModifyStr(sepc,argv[1],argv[2]) == -1)
//     {
//         return -1;
//     }
    
//     wfile = fopen("/home/wyx/update/Cache", "r+");
//     fgets(filenum, 512, wfile);
//     //printf("%s",filenum);
//     sum = atoi(filenum);
//     //printf("sum:%d\n",sum);
//     sumlen = strlen(filenum);
//     //read(buf,wfile);
//     if(find(sepc,buf,wfile) == 0)
//     {
//         fseek(wfile, sumlen, SEEK_SET);
//         num = read(filebuf,wfile);
//         strcpy(filebuf[num-1],sepc);
//         printf("%d\n",num);
//         write(filebuf,num,wfile);
//     }

//     //write();
//     fclose(wfile);
//     return 0;
// }