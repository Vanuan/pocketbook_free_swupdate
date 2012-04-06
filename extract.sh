#!/bin/bash

# this script extracts root filesystem from PocketBook SWUPDATE file (using C++ extracting program)
# and places it to $ROOTFS_EXTRACTED folder (using mount and cp commands).
# On some systems root priveleges are required to mount an image.

export SWUPDATE_FILE="%1"
export SWUPDATE_EXTRACTED=swupdate_extracted
export ROOTFS_FILE=rootfs.img
export APPFS_FILE=app.img
export ROOTFS_EXTRACTED=rootfs_extracted
export APPFS_EXTRACTED=appfs_extracted
export TEMP_MOUNT=tempfs

mkdir -p $TEMP_MOUNT

mkdir -p $SWUPDATE_EXTRACTED
make
./parseUpdate $SWUPDATE_FILE $SWUPDATE_EXTRACTED

mkdir -p $ROOTFS_EXTRACTED
sudo mount -t ext2 -o loop $SWUPDATE_EXTRACTED/$ROOTFS_FILE $TEMP_MOUNT/
cp -dpR $TEMP_MOUNT/* $ROOTFS_EXTRACTED/
sudo umount -d $TEMP_MOUNT

mkdir -p $APPFS_EXTRACTED
sudo mount -t ext2 -o loop $SWUPDATE_EXTRACTED/$APPFS_FILE $TEMP_MOUNT/
cp -dpR $TEMP_MOUNT/* $APPFS_EXTRACTED/
sudo umount -d $TEMP_MOUNT

rmdir $TEMP_MOUNT
