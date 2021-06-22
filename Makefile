OSNAME := cerberusOS
LOOP_DEVICE := /dev/loop24
OVMFDIR = ovmf
BUILD_DIR := ./build
MUSL_DIR := ./musl

QEMU_FLAGS := \
			-usb -device usb-kbd -device usb-mouse \
			-d int \
			-M q35 \
			-m 4G \
			-drive file=$(OSNAME).img \
			-drive if=pflash,format=raw,unit=0,file="$(OVMFDIR)/OVMF.fd",readonly=on \
			-drive if=pflash,format=raw,unit=1,file="$(OVMFDIR)/OVMF_VARS.fd" \
			-net none \
			-rtc clock=host,base=localtime

all: compile install run

MUSL_OBJECT_FILES += $(wildcard $(MUSL_DIR)/obj/src/math/*.lo)

$(BUILD_DIR)/libmath.a: 
	ar rcs $@ $(MUSL_OBJECT_FILES)

compileModules: $(BUILD_DIR)/libmath.a
	./scripts/compileModules.sh

compile: $(BUILD_DIR)/libmath.a
	./scripts/compile.sh

createIMG:
	./scripts/install.sh

setup: createIMG compileModules

clean:
	rm -fr build
	mkdir -p build

mount:
	@mount $(LOOP_DEVICE) /mnt/os

umount:
	@umount $(LOOP_DEVICE)

los:
	@losetup --offset 1048576 --sizelimit 46934528 $(LOOP_DEVICE) $(OSNAME).img

install_font:
	sudo cp ./font/* /mnt/os/font
	sudo cp ./startup.nsh /mnt/os

install: mount
	@cp build/BOOTX64.EFI /mnt/os/EFI/BOOT
	@cp build/kernel.elf /mnt/os
	@make umount

run:
	@qemu-system-x86_64 \
	-cpu EPYC-v2 \
 	$(QEMU_FLAGS)

run-kvm:
	@qemu-system-x86_64 --enable-kvm \
	-cpu host \
	$(QEMU_FLAGS)
	
run-debug:
	screen -dmS qemu-debug-micronet qemu-system-x86_64 -s -S -cpu EPYC-v2 $(QEMU_FLAGS)