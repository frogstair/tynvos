#!/bin/sh
set -e
. ./iso.sh

if [ "$1" = "release" ]
    then
        qemu-system-$(./target-triplet-to-arch.sh $HOST) -cdrom tynvos.iso
    else
        qemu-system-$(./target-triplet-to-arch.sh $HOST) -s -S -cdrom tynvos.iso
fi

