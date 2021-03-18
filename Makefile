IMAGE := cerberusOS.img
LOOP_DEVICE := /dev/loop20
OVMFDIR = /usr/bin/OVMFbin

all: compile install run

compile:
	./compile.sh

setup:
	./install.sh

mount:
	sudo mount $(LOOP_DEVICE) /mnt/os

umount:
	sudo umount $(LOOP_DEVICE)

los:
	losetup --offset 1048576 --sizelimit 46934528 $(LOOP_DEVICE) $(IMAGE)

install_font:
	sudo cp ./font/* /mnt/os/font
	sudo cp ./startup.nsh /mnt/os

install: mount
	sudo cp build/BOOTX64.EFI /mnt/os/EFI/BOOT
	sudo cp build/kernel.elf /mnt/os
	make umount

run:
	qemu-system-x86_64 -smp 1 -cpu qemu64 -drive file=$(IMAGE) -m 512M -cpu qemu64 -drive if=pflash,format=raw,unit=0,file="$(OVMFDIR)/OVMF_CODE-pure-efi.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="$(OVMFDIR)/OVMF_VARS-pure-efi.fd" -net none
