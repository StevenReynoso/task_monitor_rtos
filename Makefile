# === Paths ===
BUILD_DIR = build
SRC_DIR = src
INC_DIR = inc
STARTUP = startup_stm32f446.s
LINKER = linker.ld

# === Tools ===
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

CFLAGS = -mcpu=cortex-m4 -mthumb -Wall -O0 -g -ffreestanding -nostdlib -Isrc -I$(INC_DIR)
LDFLAGS = -T$(LINKER)

# === Sources & Objects ===
C_SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SOURCES))
OBJ_FILES += $(BUILD_DIR)/startup.o
BIN = $(BUILD_DIR)/main.bin

# === Default Target ===
all: $(BUILD_DIR)/main.elf

# === Create Build Directory ===
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# === Compile C Source Files ===
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# === Compile Startup Assembly ===
$(BUILD_DIR)/startup.o: $(STARTUP) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# === Link ELF ===
$(BUILD_DIR)/main.elf: $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) -o $@ $(LDFLAGS)

# === Create BIN from ELF ===
$(BIN): $(BUILD_DIR)/main.elf
	$(OBJCOPY) -O binary $< $@

# === Flash to Board ===
flash: $(BIN)
	st-flash write $< 0x08000000

# === Debug ===
debug: all
	st-util & arm-none-eabi-gdb $(BUILD_DIR)/main.elf

# === Clean ===
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all flash debug clean