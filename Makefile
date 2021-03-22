OSNAME := cerberusOS
LOOP_DEVICE := /dev/loop20
OVMFDIR = /usr/bin/OVMFbin

all: compile install run

compile:
	./compile.sh

setup:
	./install.sh

clean:
	rm -fr build
	mkdir -p build

mount:
	sudo mount $(LOOP_DEVICE) /mnt/os

umount:
	sudo umount $(LOOP_DEVICE)

los:
	losetup --offset 1048576 --sizelimit 46934528 $(LOOP_DEVICE) $(OSNAME).img

install_font:
	sudo cp ./font/* /mnt/os/font
	sudo cp ./startup.nsh /mnt/os

install: mount
	sudo cp build/BOOTX64.EFI /mnt/os/EFI/BOOT
	sudo cp build/kernel.elf /mnt/os
	make umount

run:
	qemu-system-x86_64 -enable-kvm -cpu host -M q35 -drive file=$(OSNAME).img,if=virtio -m 512M -drive if=pflash,format=raw,unit=0,file="$(OVMFDIR)/OVMF_CODE-pure-efi.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="$(OVMFDIR)/OVMF_VARS-pure-efi.fd" -net none
# -d guest_errors