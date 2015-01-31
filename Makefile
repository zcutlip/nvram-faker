TRUNK ?=.
include $(TRUNK)/arch.mk


AR?=ar

CFLAGS+=-ggdb
CFLAGS+= -DINI_MAX_LINE=2000
CFLAGS+= -DINI_USE_STACK=0
export CFLAGS
EXE=nvram_faker_exe
EXE_OBJ=nvram_faker_main.o
OBJS=nvram-faker.o 
INI_OBJ=ini.o
INI_PATH=$(TRUNK)/contrib/inih
INCLUDES=-I$(INI_PATH)

LIB=libnvram-faker.so

all:$(LIB)



exe: export CFLAGS+=-DNVRAM_EXE -DDEBUG
exe: export CFLAGS+=-DINI_FILE_PATH=\"./nvram.ini\"
exe: $(EXE)



$(INI_OBJ):
	make -C $(INI_PATH) $@
	cp $(INI_PATH)/$@ .

%.o:%.c
	$(CC) -Wall $(INCLUDES) $(CFLAGS) -fPIC -c -o $@ $<

$(LIB): $(OBJS) $(INI_OBJ)
	$(CC) -shared -o $@ $^ -Wl,-nostdlib

nvram_faker_exe:$(EXE_OBJ) $(OBJS) $(INI_OBJ)
	$(CC) -Wall -o $@ $^ 

clean:
	-rm *.o
	-rm *.so
	-rm $(EXE)
	make -C $(INI_PATH) $@


