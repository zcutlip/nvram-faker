#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "nvram-faker.h"
#include "ini.h"

#define RED_ON "\033[22;31m"
#define RED_OFF "\033[22;00m"
#define DEFAULT_KV_PAIR_LEN 1024

#ifndef INI_FILE_PATH
#define INI_FILE_PATH "/nvram.ini"
#endif

static int kv_count=0;
static int key_value_pair_len=DEFAULT_KV_PAIR_LEN;
static char **key_value_pairs=NULL;

void initialize_ini(void) __attribute__((constructor));
void end(void) __attribute__((destructor));

static int ini_handler(void *user, const char *section, const char *name,const char *value)
{

    int old_kv_len;
    char **kv;
    char **new_kv;
    int i;
    
    if(NULL == user || NULL == section || NULL == name || NULL == value)
    {
        printf("bad parameter to ini_handler\n");
        return 0;
    }
    kv = *((char ***)user);
    if(NULL == kv)
    {
        printf("kv is NULL\n");
        return 0;
    }
    
    if(kv_count >= key_value_pair_len)
    {
        old_kv_len=key_value_pair_len;
        key_value_pair_len=(key_value_pair_len * 2);
        new_kv=(char **)malloc(key_value_pair_len);
        if(NULL == kv)
        {
            printf("Failed to reallocate key value array.\n");
            return 0;
        }
        for(i=0;i<old_kv_len;i++)
        {
            new_kv[i]=kv[i];
        }
        free(*(char ***)user);
        kv=new_kv;
        *(char ***)user=kv;
    }
    printf("Got %s:%s\n",name,value);
    kv[kv_count++]=strdup(name);
    kv[kv_count++]=strdup(value);
    
    return 1;
}

void initialize_ini(void)
{
    int ret;
    printf("Initializing.\n");
    if (NULL == key_value_pairs)
    {
        key_value_pairs=malloc(key_value_pair_len);
    }
    if(NULL == key_value_pairs)
    {
        printf("Failed to allocate memory for key value array. Terminating.\n");
        exit(1);
    }
    
    ret = ini_parse(INI_FILE_PATH,ini_handler,(void *)&key_value_pairs);
    printf("ret from ini_parse was: %d\n",ret);
    if (0 != ret)
    {
        printf("INI parse failed. Terminating\n");
        free(key_value_pairs);
        key_value_pairs=NULL;
        exit(1);
    }
    
    return;
    
}

void end(void)
{
    int i;
    for (i=0;i<kv_count;i++)
    {
        free(key_value_pairs[i]);
    }
    free(key_value_pairs);
    key_value_pairs=NULL;
    
    return;
}

char *nvram_get(const char *key)
{
    int i;
    int found=0;
    char *value;
    char *ret;
    for(i=0;i<kv_count;i+=2)
    {
        if(strcmp(key,key_value_pairs[i]) == 0)
        {
            printf("%s=%s\n",key,key_value_pairs[i+1]);
            found = 1;
            value=key_value_pairs[i+1];
            break;
        }
    }

    ret = NULL;
    if(!found)
    {
            printf( RED_ON"%s=Unknown\n"RED_OFF,key);
    }else
    {

            ret=strdup(value);
    }
    return ret;
}


