C_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(notdir $(wildcard arch/$(ARCH)/*.c))))
CPP_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.cpp, %.o, $(notdir $(wildcard arch/$(ARCH)/*.cpp))))
ASM_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.s, %.o, $(notdir $(wildcard arch/$(ARCH)/*.s))))

$(BUILD_DIR)/%.o: arch/$(ARCH)/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@

$(BUILD_DIR)/%.o: arch/$(ARCH)/%.s
	$(CC) $(ASMFLAGS) $< -o $@
