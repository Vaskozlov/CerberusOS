VPATH := .
SRCDIR := .
MUSL := ../musl
BUILD_DIR := ../build
GNU_EFI_DIR := ../gnu-efi
KERNEL_DIR := ../kernel

include $(GNU_EFI_DIR)/Make.defaults

override CC := clang
override TOPDIR := $(GNU_EFI_DIR)

HEADERS := /usr/src/sys/build/include

CFLAGS += 	-Wall \
			-Wextra \
			-std=c11 \
			-I $(HEADERS) \
			-D__KERNEL__ \
			-I $(KERNEL_DIR) \
			-I ../CerberusLib/include \
			-I ./arch/$(ARCH)

ASMFLAGS := -O3\
			-Wall -Wextra \
			-m64 -c -g

override CPPFLAGS := $(filter-out $(CPPFLAGS), -maccumulate-outgoing-args)

ifeq ($(ARCH), x86_64)
	SHORT_ARCH := X64
else

endif

CRT_OBJS := $(GNU_EFI_DIR)/$(ARCH)/gnuefi/crt0-efi-$(ARCH).o
TARGET := BOOT$(SHORT_ARCH)
TARGET_BSDRIVERS :=
TARGET_RTDRIVERS :=

ifeq (FreeBSD, $(OS))
LDSCRIPT := $(GNU_EFI_DIR)/gnuefi/elf_$(ARCH)_fbsd_efi.lds
else
LDSCRIPT := $(GNU_EFI_DIR)/gnuefi/elf_$(ARCH)_efi.lds
endif

LDFLAGS += \
			-L $(GNU_EFI_DIR)/$(ARCH)/gnuefi \
			-L $(GNU_EFI_DIR)/$(ARCH)/lib \
			-shared -Bsymbolic $(CRT_OBJS)

LOADLIBES += -lefi
LOADLIBES += -lgnuefi
LOADLIBES += ../CerberusLib/build/libcerberus.a
LOADLIBES += -T $(LDSCRIPT)

ifneq ($(HAVE_EFI_OBJCOPY),)

FORMAT		:= --target efi-app-$(ARCH)
$(TARGET_BSDRIVERS): FORMAT=--target efi-bsdrv-$(ARCH)
$(TARGET_RTDRIVERS): FORMAT=--target efi-rtdrv-$(ARCH)

else

SUBSYSTEM	:= 0xa
$(TARGET_BSDRIVERS): SUBSYSTEM = 0xb
$(TARGET_RTDRIVERS): SUBSYSTEM = 0xc

FORMAT		:= -O binary
LDFLAGS		+= --defsym=EFI_SUBSYSTEM=$(SUBSYSTEM)

endif

TARGETS = $(BUILD_DIR)/$(TARGET).EFI $(TARGET_BSDRIVERS) $(TARGET_RTDRIVERS)

C_OBJECT_FILES = $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(notdir $(wildcard *.c))))
CPP_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.cpp, %.o, $(notdir $(wildcard *.cpp))))

all: $(TARGETS)

$(BUILD_DIR)/$(TARGET).EFI: $(BUILD_DIR)/$(TARGET).so
	$(OBJCOPY) -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel \
		-j .rela -j .rel.* -j .rela.* -j .rel* -j .rela* \
		-j .reloc $(FORMAT) $< $@

$(BUILD_DIR)/$(TARGET).so: $(C_OBJECT_FILES) $(CXX_OBJECT_FILES) $(ASM_OBJECT_FILES)
	$(LD) $(LDFLAGS) $^ -o $@ $(LOADLIBES)
	
$(BUILD_DIR)/%.o: %.c
	$(CC) $(INCDIR) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(INCDIR) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

.PHONY: install
