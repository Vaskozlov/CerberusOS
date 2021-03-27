C_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(notdir $(wildcard structures/*.c))))
CPP_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.cpp, %.o, $(notdir $(wildcard structures/*.cpp))))
ASM_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.s, %.o, $(notdir $(wildcard structures/*.s))))

$(BUILD_DIR)/%.o: structures/%.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: structures/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/%.o: structures/%.s
	$(CC) $(ASMFLAGS) $< -o $@
