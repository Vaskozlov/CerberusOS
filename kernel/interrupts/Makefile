C_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(notdir $(wildcard interrupts/*.c))))
CPP_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.cpp, %.o, $(notdir $(wildcard interrupts/*.cpp))))
ASM_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.s, %.o, $(notdir $(wildcard interrupts/*.s))))
ADDITIONAL_FILES += CreateInterrupts

INTERRUPTS 				:= 	DevisionByZeroINT \
							DebugINT \
							BreakpointINT \
							OverflowINT \
							BoundRangeINT \
							InvalidOpcodeINT \
							DeviceNotAvailableINT \
							EmptyIqrINT \
							PitINT \
							ERROR \
							DoubleFaultINT \
							InavlidTSSINT \
							SegmentNotPresentINT \
							GeneralProtectionINT \
							PageFaultINT \
							AlignmentCheckINT \
							MachineCheckINT \
							SIMDINT \
							VirtualizationINT \
							StackSegmentINT \


$(BUILD_DIR)/%.o: interrupts/%.c
	$(CC) -Wno-unused-command-line-argument -mgeneral-regs-only -mno-red-zone $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: interrupts/%.cpp
	$(CXX) -Wno-unused-command-line-argument -mgeneral-regs-only -mno-red-zone $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/%.o: interrupts/%.s
	$(CC) -Wno-unused-command-line-argument -mgeneral-regs-only -mno-red-zone $(ASMFLAGS) $< -o $@

CreateInterrupts:
	$(PY) ./interrupts/InterruptCreator.py $(INTERRUPTS)
