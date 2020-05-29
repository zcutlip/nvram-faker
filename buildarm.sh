#!/bin/sh

export ARCH=arm
TARGET=$1

# Sets up toolchain environment variables for arm toolchain
# Tested against armv7-eabihf, glibc from toolchains.free-electrons.com

warn()
{
	echo "$1" >&2
}

if [ ! -z $(which arm-linux-gcc) ];
then
	export CC=$(which arm-linux-gcc)
else
	warn "Not setting CC: can't locate arm-linux-gcc."
fi

if [ ! -z $(which arm-linux-ld) ];
then
	export LD=$(which arm-linux-ld)
else
	warn "Not setting LD: can't locate arm-linux-ld."
fi

if [ ! -z $(which arm-linux-ar) ];
then
	export AR=$(which arm-linux-ar)
else
	warn "Not setting AR: can't locate arm-linux-ar."
fi


if [ ! -z $(which arm-linux-strip) ];
then
	export STRIP=$(which arm-linux-strip)
else
	warn "Not setting STRIP: can't locate arm-linux-strip."
fi

if [ ! -z $(which arm-linux-nm) ];
then
	export NM=$(which arm-linux-nm)
else
	warn "Not setting NM: can't lcoate arm-linux-nm."
fi


make $TARGET || exit $?
