TRUNK ?=../..
include $(TRUNK)/arch.mk

%.o:%.c
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

clean:
	-rm *.o