#!/bin/sh

export ARCH=mipsel
export PATH=/opt/gcc/mipsel/host/usr/bin:$PATH

TARGET=$1

# Sets up toolchain environment variables for various mips toolchain

warn()
{
	echo "$1" >&2
}

if [ ! -z $(which mipsel-linux-gcc) ];
then
	export CC=$(which mipsel-linux-gcc)
else
	warn "Not setting CC: can't locate mipsel-linux-gcc."
fi

if [ ! -z $(which mipsel-linux-ld) ];
then
	export LD=$(which mipsel-linux-ld)
else
	warn "Not setting LD: can't locate mipsel-linux-ld."
fi

if [ ! -z $(which mipsel-linux-ar) ];
then
	export AR=$(which mipsel-linux-ar)
else
	warn "Not setting AR: can't locate mipsel-linux-ar."
fi


if [ ! -z $(which mipsel-linux-strip) ];
then
	export STRIP=$(which mipsel-linux-strip)
else
	warn "Not setting STRIP: can't locate mipsel-linux-strip."
fi

if [ ! -z $(which mipsel-linux-nm) ];
then
	export NM=$(which mipsel-linux-nm)
else
	warn "Not setting NM: can't lcoate mipsel-linux-nm."
fi


make $TARGET || exit $?


