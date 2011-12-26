#!/bin/bash

cp $(which qemu-arm-static) ./rootfs_extracted/usr/bin/
sudo chroot rootfs_extracted/ /bin/ash
