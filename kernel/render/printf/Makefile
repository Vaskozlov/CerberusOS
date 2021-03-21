C_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(notdir $(wildcard render/printf/*.c))))
CPP_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.cpp, %.o, $(notdir $(wildcard render/printf/*.cpp))))
ASM_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.s, %.o, $(notdir $(wildcard render/printf/*.s))))

$(BUILD_DIR)/%.o: render/printf/%.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: render/printf/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/%.o: render/printf/%.s
	$(CC) $(ASMFLAGS) $< -o $@
