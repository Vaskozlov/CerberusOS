CC	:= clang
CXX	:= clang++

CFLAGS := \
		-c \
		-g \
		-Wall \
		-Wextra \
		-std=c11 \
		-O0 \
		-fno-stack-protector \
		-fno-exceptions \
		-ffreestanding \
		-Wall -Wextra \
		-I ./include

CXXFLAGS := \
		-c \
		-g \
		-Wall \
		-Wextra \
		-std=c++20 \
		-O2 \
		-fno-stack-protector \
		-fno-exceptions \
		-ffreestanding \
		-Wall -Wextra \
		-I ./include \
		-mno-avx

BUILD_DIR := build

C_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(notdir $(wildcard code/*.c))))
CPP_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.cpp, %.o, $(notdir $(wildcard code/*.cpp))))

T_C_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.c, %.o, $(notdir $(wildcard tests/*.c))))
T_CPP_OBJECT_FILES += $(addprefix $(BUILD_DIR)/, $(patsubst %.cpp, %.o, $(notdir $(wildcard tests/*.cpp))))

$(BUILD_DIR)/libcerberus.a: $(C_OBJECT_FILES) $(CPP_OBJECT_FILES)
	ar rcs $@ $^

all: setup $(BUILD_DIR)/libcerberus.a test

test: $(T_C_OBJECT_FILES) $(T_CPP_OBJECT_FILES)
	$(CXX) $^ $(BUILD_DIR)/libcerberus.a -o build/CerberusBuilder

setup:
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: code/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/%.o: code/%.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: tests/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/%.o: tests/%.c
	$(CC) $(CFLAGS) $< -o $@
