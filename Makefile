TRUNK ?=.
include $(TRUNK)/arch.mk

TEST=test-write-pid

AR?=ar

CFLAGS+=-g 
OBJS=nvram-faker.o 
INI_OBJ=ini.o
INI_PATH=$(TRUNK)/contrib/inih
INCLUDES=-I$(INI_PATH)

LIB=libnvram-faker.so

all:$(LIB)


$(INI_OBJ):
	make -C $(INI_PATH) $@
	cp $(INI_PATH)/$@ .

%.o:%.c
	$(CC) $(INCLUDES) $(CFLAGS) -fPIC -c -o $@ $<

$(LIB): $(OBJS) $(INI_OBJ)
	$(CC) -shared -o $@ $^ -Wl,-nostdlib -ldl

clean:
	-rm *.o
	-rm *.so
	make -C $(INI_PATH) $@


