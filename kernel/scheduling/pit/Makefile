C_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(notdir $(wildcard scheduling/pit/*.c))))
CPP_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.cpp, %.o, $(notdir $(wildcard scheduling/pit/*.cpp))))
ASM_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.s, %.o, $(notdir $(wildcard scheduling/pit/*.s))))

$(BUILD_DIR)/%.o: scheduling/pit/%.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: scheduling/pit/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/%.o: scheduling/pit/%.s
	$(CC) $(ASMFLAGS) $< -o $@
