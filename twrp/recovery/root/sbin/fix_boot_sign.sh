#!/sbin/sh
# puppet13th@xda
# highwaystar_ru@xda
#dump current installed kernel
#get boot.img info
workdir=/tmp/mkboot
bootdevice=/dev/block/bootdevice/by-name/boot
bootimg=boot

tooldir=/tools/

busybox=$(command -v busybox)
od=$tooldir/od

chmod +x $od

C_OUT="\033[0;1m"
C_ERR="\033[31;1m"
C_CAUT="\033[33;1m"
C_CLEAR="\033[0;0m"

mkdir $workdir
cd $workdir
$busybox dd if=$bootdevice of=boot.img 2>/dev/null
echo $boot_magic_addr
bootimg=boot.img
echo calc
kernel_addr=0x$($od -A n -X -j 12 -N 4 $bootimg | $busybox sed 's/ //g' | $busybox sed 's/^0*//g')
ramdisk_addr=0x$($od -A n -X -j 20 -N 4 $bootimg | $busybox sed 's/ //g' | $busybox sed 's/^0*//g')
second_addr=0x$($od -A n -X -j 28 -N 4 $bootimg | $busybox sed 's/ //g' | $busybox sed 's/^0*//g')
tags_addr=0x$($od -A n -X -j 32 -N 4 $bootimg | $busybox sed 's/ //g' | $busybox sed 's/^0*//g')

kernel_size=$($od -A n -D -j 8 -N 4 $bootimg | $busybox sed 's/ //g')
#base_addr=0x$($od -A n -x -j 14 -N 2 $bootimg | $busybox sed 's/ //g')0000
ramdisk_size=$($od -A n -D -j 16 -N 4 $bootimg | $busybox sed 's/ //g')
second_size=$($od -A n -D -j 24 -N 4 $bootimg | $busybox sed 's/ //g')
page_size=$($od -A n -D -j 36 -N 4 $bootimg | $busybox sed 's/ //g')
dtb_size=$($od -A n -D -j 40 -N 4 $bootimg | $busybox sed 's/ //g')
#cmd_line=$($od -A n --strings -j 64 -N 512 $bootimg)
#board=$($od -A n --strings -j 48 -N 16 $bootimg)
cmd_line=$($od -A n -S1 -j 64 -N 512 $bootimg)
board=$($od -A n -S1 -j 48 -N 16 $bootimg)

base_addr=$((kernel_addr-0x00008000))
kernel_offset=$((kernel_addr-base_addr))
ramdisk_offset=$((ramdisk_addr-base_addr))
second_offset=$((second_addr-base_addr))
tags_offset=$((tags_addr-base_addr))
qcdt_offset=$((qcdt_addr-base_addr))

base_addr=$(printf "%08x" $base_addr)
kernel_offset=$(printf "%08x" $kernel_offset)
ramdisk_offset=$(printf "%08x" $ramdisk_offset)
second_offset=$(printf "%08x" $second_offset)
tags_offset=$(printf "%08x" $tags_offset)
qcdt_offset=$(printf "%08x" $qcdt_offset)

base_addr=0x${base_addr:0-8}
kernel_offset=0x${kernel_offset:0-8}
ramdisk_offset=0x${ramdisk_offset:0-8}
second_offset=0x${second_offset:0-8}
tags_offset=0x${tags_offset:0-8}
qcdt_offset=0x${qcdt_offset:0-8}

# ABOVE SECTION HANDLES NON STANDARD IMAGES
#########################################################

k_count=$(((kernel_size+page_size-1)/page_size))
r_count=$(((ramdisk_size+page_size-1)/page_size))
s_count=$(((second_size+page_size-1)/page_size))
d_count=$(((dtb_size+page_size-1)/page_size))
k_offset=1
r_offset=$((k_offset+k_count))
s_offset=$((r_offset+r_count))
d_offset=$((s_offset+s_count))

lenovo_sig_offset=$(($s_offset*$page_size))

#append boot signature to built image
echo s_offset=$s_offset
dd if=/boot.sig of=$bootdevice bs=$page_size seek=$s_offset
