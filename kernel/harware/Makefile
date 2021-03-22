C_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(notdir $(wildcard harware/*.c))))
CPP_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.cpp, %.o, $(notdir $(wildcard harware/*.cpp))))
ASM_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.s, %.o, $(notdir $(wildcard harware/*.s))))

$(BUILD_DIR)/%.o: harware/%.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: harware/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/%.o: harware/%.s
	$(CC) $(ASMFLAGS) $< -o $@

CFLAGS += -I./harware
CXXFLAGS += -I ./harware
