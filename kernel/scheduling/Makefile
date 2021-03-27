C_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(notdir $(wildcard scheduling/*.c))))
CPP_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.cpp, %.o, $(notdir $(wildcard scheduling/*.cpp))))
ASM_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.s, %.o, $(notdir $(wildcard scheduling/*.s))))

$(BUILD_DIR)/%.o: scheduling/%.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: scheduling/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/%.o: scheduling/%.s
	$(CC) $(ASMFLAGS) $< -o $@

include scheduling/pit/Makefile
