TRUNK ?=.
include $(TRUNK)/arch.mk

TEST=test-write-pid

AR?=ar

CFLAGS+=-g 
OBJS=nvram-faker.o ini.o

LIB=libnvram-faker.so
#CC=mipsel-linux-gcc
all:$(LIB)

%.o:%.c
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

$(LIB): $(OBJS)
	$(CC) -shared -o $@ $^ -Wl,-nostdlib

clean:
	-rm *.o
	-rm *.so


