#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/tynvos.kernel isodir/boot/tynvos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "tynvos" {
	multiboot /boot/tynvos.kernel
}
EOF
grub-mkrescue -o tynvos.iso isodir

objcopy -O binary isodir/boot/tynvos.kernel kernel.bin

. ./clean.sh
