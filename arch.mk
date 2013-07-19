ifeq ($(ARCH),mips)
	export HOST?="$(ARCH)-linux"
	#export OLDPATH=$(PATH)
	#MIPS_TOOLCHAIN_PATH=/opt/gcc/$(ARCH)/host/usr/bin
	#export PATH:=$(MIPS_TOOLCHAIN_PATH):$(OLDPATH) export
	#LD?=$(MIPS_TOOLCHAIN_PATH)/$(HOST)-ld
endif

ifeq ($(ARCH),mipsel)
	export HOST?="$(ARCH)-linux"
endif

