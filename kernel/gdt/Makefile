C_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(notdir $(wildcard gdt/*.c))))
CPP_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.cpp, %.o, $(notdir $(wildcard gdt/*.cpp))))
ASM_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.s, %.o, $(notdir $(wildcard gdt/*.s))))

$(BUILD_DIR)/%.o: gdt/%.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: gdt/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/%.o: gdt/%.s
	$(CC) $(ASMFLAGS) $< -o $@
