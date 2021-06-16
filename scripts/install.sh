mkdir -p build
sudo mkdir -p /mnt/os

if [ ! -d cerberusOS.img ] ; then
    dd if=/dev/zero of=cerberusOS.img bs=512 count=93750;
    echo ""
    echo "AGRUMENTS!!!! First agument: o, Second: n, 1, 2048, 93716, ef00, w"
    echo ""
    echo ""
    sudo gdisk cerberusOS.img;
    sudo losetup --offset 1048576 --sizelimit 46934528 /dev/loop24 cerberusOS.img
	sudo mkdosfs -F 32 /dev/loop24
fi

sudo mount /dev/loop24 /mnt/os
sudo mkdir /mnt/os/EFI
sudo mkdir /mnt/os/EFI/BOOT
sudo mkdir /mnt/os/font
sudo cp ./font/* /mnt/os/font
sudo cp ./startup.nsh /mnt/os
sudo umount /dev/loop24
