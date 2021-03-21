C_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(notdir $(wildcard memory/*.c))))
CPP_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.cpp, %.o, $(notdir $(wildcard memory/*.cpp))))
ASM_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.s, %.o, $(notdir $(wildcard memory/*.s))))

$(BUILD_DIR)/%.o: memory/%.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: memory/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/%.o: memory/%.s
	$(CC) $(ASMFLAGS) $< -o $@

CFLAGS += -I./memory
CXXFLAGS += -I ./memory
