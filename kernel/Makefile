LD 	:= ld
CC 	:= clang
CXX := clang++

PREFIX       := /usr/local
HOSTCC       := $(prefix)clang
OS           := $(shell uname -s)
HOSTARCH     ?= $(shell $(HOSTCC) -dumpmachine | cut -f1 -d- | sed -e s,i[3456789]86,ia32, -e 's,armv[67].*,arm,' )
ARCH         ?= $(shell $(HOSTCC) -dumpmachine | cut -f1 -d- | sed -e s,i[3456789]86,ia32, -e 's,armv[67].*,arm,' )

MUSL_DIR 		:= ../musl
BUILD_DIR 		:= ../build
GNU_EFI_DIR 	:= ../gnu-efi
TARGET 			:= kernel.elf
LINUX_HEADERS 	:= /usr/src/sys/build

ifeq (FreeBSD, $(OS))
LDSCRIPT := $(GNU_EFI_DIR)/gnuefi/elf_$(ARCH)_fbsd_efi.lds
else
LDSCRIPT := $(GNU_EFI_DIR)/gnuefi/elf_$(ARCH)_efi.lds
endif

CFLAGS := 	-O0 -g \
			-c \
			-fno-stack-protector \
			-ffreestanding \
			-fshort-wchar \
			-Wall -Wextra \
			-I $(LINUX_HEADERS)/include \
			-I . \
			-std=c11

CXXFLAGS :=	-O0 -g \
			-c \
			-fno-stack-protector \
			-ffreestanding \
			-fshort-wchar \
			-Wall -Wextra \
			-I $(LINUX_HEADERS)/include \
			-I . \
			-std=c++17

ASMFLAGS := -S -O2 -g

LDFLAGS :=	-static \
			-Bsymbolic \
			-nostdlib \
			-T $(LDSCRIPT) 

LOADLIBES := 	$(MUSL_DIR)/lib/libc.a \
				$(MUSL_DIR)/lib/libm.a

C_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(notdir $(wildcard *.c))))
CPP_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.cpp, %.o, $(notdir $(wildcard *.cpp))))
ASM_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.s, %.o, $(notdir $(wildcard *.s))))

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.s
	$(CC) $(ASMFLAGS) $< -o $@

all: $(C_OBJECT_FILES) $(CPP_OBJECT_FILES) $(ASM_OBJECT_FILES)
	$(LD) $(LDFLAGS) -o $(BUILD_DIR)/$(TARGET) $^ $(LOADLIBES)
