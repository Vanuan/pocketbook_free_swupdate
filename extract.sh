#!/bin/bash

# this script extracts root filesystem from PocketBook SWUPDATE file (using C++ extracting program)
# and places it to $ROOTFS_EXTRACTED folder (using mount and cp commands).
# On some systems root priveleges are required to mount an image.

export SWUPDATE_FILE="%1"
export SWUPDATE_EXTRACTED=swupdate_extracted
export ROOTFS_FILE=rootfs.img
export ROOTFS_EXTRACTED=rootfs_extracted
export ROOTFS_MOUNT=rootfs

mkdir -p $SWUPDATE_EXTRACTED
mkdir -p $ROOTFS_MOUNT
mkdir -p $ROOTFS_EXTRACTED

make
./parseUpdate $SWUPDATE_FILE $SWUPDATE_EXTRACTED

sudo mount -t ext2 -o loop $SWUPDATE_EXTRACTED/$ROOTFS_FILE $ROOTFS_MOUNT/
cp -dpR $ROOTFS_MOUNT/* $ROOTFS_EXTRACTED/
sudo umount $ROOTFS_MOUNT
rmdir $ROOTFS_MOUNT
