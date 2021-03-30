C_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(notdir $(wildcard hardware/*.c))))
CPP_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.cpp, %.o, $(notdir $(wildcard hardware/*.cpp))))
ASM_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.s, %.o, $(notdir $(wildcard hardware/*.s))))

$(BUILD_DIR)/%.o: hardware/%.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: hardware/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/%.o: hardware/%.s
	$(CC) $(ASMFLAGS) $< -o $@

include hardware/ahci/Makefile
