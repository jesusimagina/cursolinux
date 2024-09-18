#!/bin/bash

# apt install net-tools uml-utilities nfs-kernel-server
# Hay que editar el /etc/exports como sudo
# Añadir la línea: /home/user/cursolinux/rootfs/compile/rootfs *(rw,sync,no_subtree_check,no_root_squash)
# Reiniciar el servicio:
# 	systemctl restart nfs-kernel-server



ROOTDIR=/home/user/cursolinux/rootfs/compile/rootfs
HOST_IP=192.168.1.1
TARGET_IP=192.168.1.101
NET_NUMBER=192.168.1.0
NET_MASK=255.255.255.0
tunctl -u $(whoami) -t tap0
ifconfig tap0 ${HOST_IP}
route add -net ${NET_NUMBER} netmask ${NET_MASK} dev tap0
sh -c "echo 1 > /proc/sys/net/ipv4/ip_forward"

KERNEL=zImage
DTB=versatile-pb.dtb
INITRAMFS=initramfs.cpio.gz

if [ ! -f ${KERNEL} ]; then
        echo "${KERNEL} does not exist"
        exit 1
fi
if [ ! -f ${DTB} ]; then
        echo "${DTB} does not exist"
        exit 1
fi

#QEMU_AUDIO_DRV=none \
#qemu-system-arm -m 256M -nographic -M versatilepb -kernel ${KERNEL} -dtb ${DTB} -append "console=ttyAMA0,115200  root=/dev/nfs rw nfsroot=${HOST_IP}:${ROOTDIR},v3 init=/sbin/init ip=${TARGET_IP}" -net nic -net tap,ifname=tap0,script=no

qemu-system-arm -m 256M -nographic -M versatilepb -kernel ${KERNEL} -append "console=ttyAMA0,115200 root=/dev/nfs rw nfsroot=${HOST_IP}:${ROOTDIR} ip=${TARGET_IP}" -dtb ${DTB} -net nic -net tap,ifname=tap0,script=no
