C_OBJECT_FILES 		+= $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(notdir $(wildcard hardware/usb/*.c))))
CPP_OBJECT_FILES 	+= $(addprefix $(BUILD_DIR)/, $(patsubst %.cpp, %.o, $(notdir $(wildcard hardware/usb/*.cpp))))
ASM_OBJECT_FILES 	+= $(addprefix $(BUILD_DIR)/, $(patsubst %.s, %.o, $(notdir $(wildcard hardware/usb/*.s))))

$(BUILD_DIR)/%.o: hardware/usb/%.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: hardware/usb/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/%.o: hardware/usb/%.s
	$(CC) $(ASMFLAGS) $< -o $@
