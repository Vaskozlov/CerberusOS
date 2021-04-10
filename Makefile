OSNAME := cerberusOS
LOOP_DEVICE := /dev/loop24
OVMFDIR = /usr/share/edk2-ovmf/x64

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
	qemu-system-x86_64 \
	-d int \
	-cpu EPYC-v2 \
	-no-shutdown \
	-M q35 \
	-drive file=$(OSNAME).img -m 4G \
	-drive file=blank.img \
	-drive if=pflash,format=raw,unit=0,file="$(OVMFDIR)/OVMF.fd",\
	readonly=on -drive if=pflash,format=raw,unit=1,file="$(OVMFDIR)/OVMF_VARS.fd" -net none \
	-rtc clock=host,base=localtime

run-test:
	qemu-system-x86_64 -M q35 -drive file=$(OSNAME).img -m 1G -cpu qemu64 -drive if=pflash,format=raw,unit=0,file="$(OVMFDIR)/OVMF.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="$(OVMFDIR)/OVMF_VARS.fd" -net none

run-kvm:
	qemu-system-x86_64 --enable-kvm \
	-cpu host \
	-no-shutdown \
	-M q35 \
	-drive file=$(OSNAME).img -m 1G \
	-drive if=pflash,format=raw,unit=0,file="$(OVMFDIR)/OVMF.fd",\
	readonly=on \
	-drive if=pflash,format=raw,unit=1,file="$(OVMFDIR)/OVMF_VARS.fd" -net none \
	-rtc clock=host,base=localtime
