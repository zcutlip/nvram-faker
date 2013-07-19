#!/bin/sh

export ARCH=mips
TARGET=$1

# Sets up toolchain environment variables for various mips toolchain

warn()
{
	echo "$1" >&2
}

if [ ! -z $(which mips-linux-gcc) ];
then
	export CC=$(which mips-linux-gcc)
else
	warn "Not setting CC: can't locate mips-linux-gcc."
fi

if [ ! -z $(which mips-linux-ld) ];
then
	export LD=$(which mips-linux-ld)
else
	warn "Not setting LD: can't locate mips-linux-ld."
fi

if [ ! -z $(which mips-linux-ar) ];
then
	export AR=$(which mips-linux-ar)
else
	warn "Not setting AR: can't locate mips-linux-ar."
fi


if [ ! -z $(which mips-linux-strip) ];
then
	export STRIP=$(which mips-linux-strip)
else
	warn "Not setting STRIP: can't locate mips-linux-strip."
fi

if [ ! -z $(which mips-linux-nm) ];
then
	export NM=$(which mips-linux-nm)
else
	warn "Not setting NM: can't lcoate mips-linux-nm."
fi


make $TARGET || exit $?


