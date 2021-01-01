#!/bin/sh
set -e
. ./iso.sh

i3-msg workspace 1
qemu-system-$(./target-triplet-to-arch.sh $HOST) -s -S -cdrom tynvos.iso -m 2G &
sleep 1
urxvt -e gdb &

wait