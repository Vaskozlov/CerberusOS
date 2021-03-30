C_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(notdir $(wildcard hardware/ahci/*.c))))
CPP_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.cpp, %.o, $(notdir $(wildcard hardware/ahci/*.cpp))))
ASM_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.s, %.o, $(notdir $(wildcard hardware/ahci/*.s))))

$(BUILD_DIR)/%.o: hardware/ahci/%.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: hardware/ahci/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/%.o: hardware/ahci/%.s
	$(CC) $(ASMFLAGS) $< -o $@
