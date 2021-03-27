C_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(notdir $(wildcard render/*.c))))
CPP_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.cpp, %.o, $(notdir $(wildcard render/*.cpp))))
ASM_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.s, %.o, $(notdir $(wildcard render/*.s))))

$(BUILD_DIR)/%.o: render/%.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: render/%.cpp
	$(CXX) $(CXXFLAGS) -O1 $< -o $@

$(BUILD_DIR)/%.o: render/%.s
	$(CC) $(ASMFLAGS) $< -o $@

include render/printf/Makefile

CFLAGS += -I./render
CXXFLAGS += -I ./render
