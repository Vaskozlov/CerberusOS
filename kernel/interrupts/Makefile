C_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(notdir $(wildcard interrupts/*.c))))
CPP_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.cpp, %.o, $(notdir $(wildcard interrupts/*.cpp))))
ASM_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.s, %.o, $(notdir $(wildcard interrupts/*.s))))

$(BUILD_DIR)/%.o: interrupts/%.c
	$(CC) -Wno-unused-command-line-argument -mgeneral-regs-only -mno-red-zone $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: interrupts/%.cpp
	$(CXX) -Wno-unused-command-line-argument -mgeneral-regs-only -mno-red-zone -mavx -mavx2 -mxsave $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/%.o: interrupts/%.s
	$(CC) -Wno-unused-command-line-argument -mgeneral-regs-only -mno-red-zone $(ASMFLAGS) $< -o $@
