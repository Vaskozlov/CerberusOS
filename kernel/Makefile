LD 	:= ld
CC 	:= clang
CXX := clang++
CCV := $(shell $(CC) -dumpversion)

PREFIX       := /usr/local
HOSTCC       := $(prefix)clang
OS           := $(shell uname -s)
HOSTARCH     ?= $(shell $(HOSTCC) -dumpmachine | cut -f1 -d- | sed -e s,i[3456789]86,ia32, -e 's,armv[67].*,arm,' )
ARCH         ?= $(shell $(HOSTCC) -dumpmachine | cut -f1 -d- | sed -e s,i[3456789]86,ia32, -e 's,armv[67].*,arm,' )

MUSL_DIR 		:= ../musl
BUILD_DIR 		:= ../build
GNU_EFI_DIR 	:= ../gnu-efi
TARGET 			:= kernel.elf
CERBERUS_DIR 	:= ../CerberusLib
LINUX_HEADERS 	:= /usr/src/sys/build

MAJOR_VERSION := 0
MINOR_VERSION := 1
PATCH_VERSION := 0

ifeq (FreeBSD, $(OS))
LDSCRIPT := linkers/elf_$(ARCH)_fbsd_efi.lds
else
LDSCRIPT := linkers/elf_$(ARCH)_efi.lds
endif

OS_DEBGUG := 1

CFLAGS :=  \
			-O0 \
			-g \
			-c \
			-msse4.1 \
			-msse4.2 \
			-Wall\
			-Wextra \
			-nostdlib \
			-ffreestanding \
			-fno-exceptions \
			-fno-stack-protector \
			-std=c11 \
			-I . \
			-I ./arch/$(ARCH) \
			-I $(MUSL_DIR)/arch \
			-I $(LINUX_HEADERS)/include \
			-I $(CERBERUS_DIR)/include \
			-D MAJORE_VERSION=$(MAJORE_VERSION) \
			-D MINORE_VERSION=$(MINORE_VERSION) \
			-D PATCH_VERSION=$(PATCH_VERSION) \
			-D OS_DEBGUG=$(OS_DEBGUG)

CXXFLAGS :=  \
			-O0 \
			-g \
			-c \
			-msse4.1 \
			-msse4.2 \
			-Wall\
			-Wextra \
			-nostdlib \
			-nostdlib++ \
			-ffreestanding \
			-fno-exceptions \
			-fno-stack-protector \
			-std=c++20 \
			-I . \
			-I ./arch/$(ARCH) \
			-I $(MUSL_DIR)/arch \
			-I $(CERBERUS_DIR)/include \
			-I $(LINUX_HEADERS)/include \
			-D MAJOR_VERSION=$(MAJOR_VERSION) \
			-D MINOR_VERSION=$(MINOR_VERSION) \
			-D PATCH_VERSION=$(PATCH_VERSION) \
			-D OS_DEBUG=$(OS_DEBGUG)

ASMFLAGS := -O3\
			-Wall -Wextra \
			-m64 -c -g

LDFLAGS :=	-static \
			-Bsymbolic \
			-nostdlib \
			-T $(LDSCRIPT) 

LOADLIBES := \
			$(CERBERUS_DIR)/build/libcerberus.a \
			$(BUILD_DIR)/libmath.a

C_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(notdir $(wildcard *.c))))
CPP_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.cpp, %.o, $(notdir $(wildcard *.cpp))))
ASM_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.s, %.o, $(notdir $(wildcard *.s))))

include render/Makefile
include memory/Makefile
include gdt/Makefile
include interrupts/Makefile
include hardware/Makefile
include scheduling/Makefile
include arch/Makefile

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.s
	$(CC) $(ASMFLAGS) $< -o $@

all: $(C_OBJECT_FILES) $(CPP_OBJECT_FILES) $(ASM_OBJECT_FILES)
	$(LD) $(LDFLAGS) -o $(BUILD_DIR)/$(TARGET) $^ $(LOADLIBES)
