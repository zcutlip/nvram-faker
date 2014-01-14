#ifndef __NVRAM_FAKER_H__
#define __NVRAM_FAKER_H__

char *nvram_get(const char *key);

#endif /* __NVRAM_FAKER_H__ */

typedef struct {
   char **kv_pairs;
   char **fake_fns; 
}ini_info_t;

#ifndef O_CREAT
#define O_CREAT     00000100
#endif

#ifndef O_RDWR
#define O_RDWR      00000002
#endif

#ifndef O_APPEND
#define O_APPEND    00002000
#endif

