#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/tyn.kernel isodir/boot/tyn.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "tyn" {
	insmod all_video
	multiboot /boot/tyn.kernel
}
EOF
grub-mkrescue -o tyn.iso isodir

objcopy -O binary isodir/boot/tyn.kernel kernel.bin

. ./clean.sh
