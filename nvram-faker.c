#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <dlfcn.h>
#include "nvram-faker.h"
#include "ini.h"

#define RED_ON "\033[22;31m"
#define RED_OFF "\033[22;00m"
#define DEFAULT_BUFFER_LEN 1024

#ifndef INI_FILE_PATH
#define INI_FILE_PATH "/nvram.ini"
#endif

#ifndef LOG_FILE_PATH
#define LOG_FILE_PATH "/tmp/faker.log"
#endif

static int kv_count=0;
static int fn_count=0;
static int key_value_pair_len=DEFAULT_BUFFER_LEN;
static int fake_filenames_len=DEFAULT_BUFFER_LEN;
ini_info_t *ii=NULL;
static char **key_value_pairs=NULL;
static char **fake_filenames=NULL;
FILE* flog;

void initialize_ini(void) __attribute__((constructor));
void end(void) __attribute__((destructor));

int fname_check(const char *filename);

static int ini_handler(void *user, const char *section, const char *name,const char *value)
{

    int old_kv_len;
    ini_info_t *new_ii;
    char **kv;
    char **fn;
    char **new_kv;
    int i;

    if(NULL == user || NULL == section || NULL == name || NULL == value)
    {
        fprintf(flog,"bad parameter to ini_handler\n");
        return 0;
    }

    new_ii = *((ini_info_t **) user);

    if(NULL == new_ii)
    {
        fprintf(flog,"ini_info struct is NULL\n");
        return 0;
    }

    kv = new_ii->kv_pairs;
    fn = new_ii->fake_fns;

    if(kv_count >= key_value_pair_len)
    {
        old_kv_len=key_value_pair_len;
        key_value_pair_len=(key_value_pair_len * 2);
        new_kv=(char **)malloc(key_value_pair_len);
        if(NULL == new_kv)
        {
            fprintf(flog,"Failed to reallocate key value array.\n");
            return 0;
        }
        for(i=0;i<old_kv_len;i++)
        {
            new_kv[i]=kv[i];
        }

        free(new_ii->kv_pairs);
        kv=new_kv;
        new_ii->kv_pairs=kv;
    }

    if(fn_count >= fake_filenames_len)
    {
        fprintf(flog,"Sorry you are limited to 1024 filenames to fake\n");
        fprintf(flog,"Seriously it will probably make things painfully slow\n");
        fprintf(flog,"if you had more.\n");
        return 0;
    }

    fprintf(flog,"Got %s:%s\n",name,value);
    if(strstr(name,"fake_filename")){
        fn[fn_count++]=strdup(value);
    }else{
        kv[kv_count++]=strdup(name);
        kv[kv_count++]=strdup(value);
    }
    
    return 1;
}

void initialize_ini(void)
{
    int ret;
    char *flog_path=NULL;
    flog_path = getenv("FAKER_LOG");


    if(NULL == flog_path || '\0' == flog_path[0]){
        //if no log provided log to stderr
        flog = stderr;
        fprintf(flog,RED_ON"**************************************************\n");
        fprintf(flog,"**\t\tTO LOG TO A FILE\t\t**\n");
        fprintf(flog,"**\t   SET \"FAKER_LOG\" ENV VARIABLE\t\t**\n");
        fprintf(flog,"**************************************************\n"RED_OFF);
    }else{
        flog = fopen(flog_path,"w+");
        if( NULL == flog){
            fprintf(stderr,"Failed to open log file \"%s\"\n",flog_path);
            exit(1);
        }
    }

    setvbuf(flog,NULL,_IONBF,0);
    fprintf(flog,"Initializing.\n");
    if(NULL == ii){
        ii = malloc(sizeof(ini_info_t));
        if(NULL == ii){
            fprintf(flog,"Failed to allocate memory for update info struct. Terminating.\n");
            exit(1);
        }
            
    }

    if (NULL == key_value_pairs)
    {
        key_value_pairs=malloc(key_value_pair_len);
    }
    if(NULL == key_value_pairs)
    {
        fprintf(flog,"Failed to allocate memory for key value array. Terminating.\n");
        exit(1);
    }

    if(NULL == fake_filenames)
    {
        fake_filenames = malloc(fake_filenames_len);
    }
    if(NULL == fake_filenames)
    {
        fprintf(flog,"Failed to allocate memory for fake filenames array. Terminating.\n");
        exit(1);
    }

    ii->kv_pairs = key_value_pairs;
    ii->fake_fns = fake_filenames;
   
    ret = ini_parse(INI_FILE_PATH,ini_handler,(void *)&ii);
    fprintf(flog,"ret from ini_parse was: %d\n",ret);
    if (0 != ret)
    {
        fprintf(flog,"INI parse failed. Terminating\n");
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
        if(NULL != key_value_pairs[i])
            free(key_value_pairs[i]);
    }

    for (i=0;i<fn_count;i++)
    {
        if(NULL != fake_filenames[i])
            free(fake_filenames[i]);
    }
    free(key_value_pairs);
    free(fake_filenames);
    key_value_pairs=NULL;
    fake_filenames=NULL;
    free(ii);
    ii=NULL;

    fclose(flog);
    
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
            fprintf(flog,"%s=%s\n",key,key_value_pairs[i+1]);
            found = 1;
            value=key_value_pairs[i+1];
            break;
        }
    }

    ret = NULL;
    if(!found)
    {
            if(flog == stderr)
                fprintf(flog, RED_ON"%s=Unknown\n"RED_OFF,key);
            else
                fprintf(flog, "%s=Unknown (FAILED LOOKUP)\n",key);
    }else
    {

            ret=strdup(value);
    }
    return ret;
}

FILE* fopen(const char *filename, const char *mode){
	static FILE* (*my_fopen) (const char *filename, const char *mode) = NULL;
    char *base;
    char fakename[64];

	if(!my_fopen)
		my_fopen = dlsym(RTLD_NEXT,"fopen");
	FILE* p = my_fopen(filename,mode);
	if(p==0){
		if( fname_check(filename) ){
            base = basename(filename);
            if(strlen(base) > 54)
                base[53]='\0';
            snprintf(fakename,64,"/tmp/%s.fake",base);
                    
			p = my_fopen(fakename,"a+");
        }
	}
	return p;
}

int fname_check(const char *filename){
	int i=0;
	
    for(i=0; i<fn_count; i++){
		if(strstr(filename,fake_filenames[i])){
			fprintf(flog,"FILE %s not found providing false positive\n",filename);
			return 1;
		}
	}

	return 0;
}

int open(const char *fn, int flags){
    static int (*my_open) (const char *fn, int flags) = NULL;
    char *base;
    char fakename[64];


    if(!my_open)
        my_open = dlsym(RTLD_NEXT, "open");

    int p = my_open(fn,flags);
    if(p==-1){
        if( fname_check(fn) ){
            base = basename(fn);
            if(strlen(base)>54)
                base[53]='\0';
            snprintf(fakename,64,"/tmp/%s.fake",base);
            p = my_open(fakename,O_RDWR|O_CREAT|O_APPEND);
        }
    }

    return p;
} 
