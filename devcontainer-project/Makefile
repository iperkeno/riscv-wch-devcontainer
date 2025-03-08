######################################
# target ok
######################################
TARGET ?= program

FAMILY = ch32v30x

#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

TOOLCHAIN= riscv-none-elf-

OPENOCD_DIR= /opt/wch-openocd/bin

######################################
# source
######################################
# C sources
C_SOURCES += \
	User/ch32v30x_it.c \
	User/main.c 

#######################################
# binaries
#######################################
CC = $(TOOLCHAIN)gcc
AS = $(TOOLCHAIN)gcc -x assembler-with-cpp
CP = $(TOOLCHAIN)objcopy
SZ = $(TOOLCHAIN)size

HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

######################################
# building variables
######################################
# debug build?
DEBUG = 1

#######################################
# includes
#######################################

include /opt/wch-sdk/wch-sdk.mk

#include ./FreeRTOS/freertos.mk

#######################################
# CFLAGS
#######################################
# cpu
#CPU = -march=rv32imac -mabi=ilp32 -msmall-data-limit=8 

# For gcc v12 and above
#CPU = -march=rv32imac_zicsr -mabi=ilp32 -msmall-data-limit=8

CPU = 	-march=rv32imafc_zicsr \
		-msave-restore \
 		-mabi=ilp32f \
		-msmall-data-limit=8

# mcu
MCU = $(CPU) $(FPU) $(FLOAT-ABI)

# optimization for size
OPT = -Os

# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES += -IUser

# compiler/assembler gcc flags
ASFLAGS = $(MCU) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_INCLUDES)   $(OPT) -Wall -fdata-sections -ffunction-sections -fmessage-length=0 -fsigned-char -Wunused -Wuninitialized

ifeq ($(DEBUG), 1)
CFLAGS += -g #-gdwarf-2
endif


CFLAGS += -D__BUILD_FREERTOS__

# Generate dependency information
CFLAGS += -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)"

#		-std=gnu99
#		-Wno-comment
#		-Wno-pointer-sign
#		-fno-common

# FLAGS ?= 	-march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -Os \
# 			-fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized  -g
# ASFLAGS ?= 	$(FLAGS) -x assembler $(INC_FLAGS) -MMD -MP
# CPPFLAGS ?= $(FLAGS) $(INC_FLAGS) -std=gnu99 -MMD -MP
# LDFLAGS ?= 	$(FLAGS) -T ./vendor/Ld/Link.ld -nostartfiles -Xlinker --gc-sections -Wl,-Map,"$(BUILD_DIR)/CH32V307VCT6.map" --specs=nano.specs --specs=nosys.specs


# riscv-none-embed-gcc 
#	ok	-march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -msave-restore 
# 	ok	-Os 
# 		-fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common 
# 		-Wunused -Wuninitialized  
# 		-g 
# 		-std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	
#######################################
# LDFLAGS
#######################################
# libraries
LIBS = -lc -lm -lnosys
#LIBS = -lm 
LIBDIR = 

#LDFLAGS = $(MCU) -mno-save-restore -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized 
#               -T $(LDSCRIPT) -nostartfiles -Xlinker --gc-sections -Wl,-Map=$(BUILD_DIR)/$(TARGET).map --specs=nano.specs $(LIBS)
LDFLAGS = $(MCU) -msave-restore -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g \
				-T $(LDSCRIPT) -nostartfiles -Xlinker --gc-sections -Wl,-Map=$(BUILD_DIR)/$(TARGET).map --specs=nano.specs --specs=nosys.specs $(LIBS)

#--specs=nosys.specs # this generates wornings for some function like _lseek

#######################################
# default action: build all
#######################################
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

#######################################
# build the application
#######################################
# list of objects
# Create a list of dependency files with the desired output directory path.
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.S=.o)))
vpath %.S $(sort $(dir $(ASM_SOURCES)))

# Makefile is a target, changes to Makefile trigger a rebuild.
# $(BUILD_DIR) is an order-only prerequisite, changes to this doesn't trigger rebuild
$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	@echo -e 'Building target: $@ \t\t from $< '
	@$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.S Makefile | $(BUILD_DIR)
	@echo -e 'Building target: $@ 	\t\t from $< '
	@$(AS) -c $(CFLAGS) $< -o $@

#$(LUAOBJECTS) $(OBJECTS)

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	@echo 'Invoking: GNU RISC-V Cross C Linker'
	@$(CC) $(LDFLAGS) -o $@ $(OBJECTS) $(LIBS)
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@ $@/FreeRTOS $@/wch-sdk	

#######################################
# Program
#######################################

wlink-watch:$(BUILD_DIR)/$(TARGET).elf
	wlink flash --watch-serial build/${TARGET}.elf

wlink:$(BUILD_DIR)/$(TARGET).elf
	wlink flash  build/${TARGET}.elf

program: $(BUILD_DIR)/$(TARGET).elf 
	sudo ${OPENOCD_DIR}/openocd -f ${OPENOCD_DIR}/wch-riscv.cfg -c 'init; halt; program $(BUILD_DIR)/$(TARGET).elf; reset; wlink_reset_resume; exit;'

isp: $(BUILD_DIR)/$(TARGET).bin
	wchisp flash $(BUILD_DIR)/$(TARGET).bin

test:
	@echo $(C_SOURCES)

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
# -include $(wildcard $(BUILD_DIR)/*.d)

-include $(OBJECTS:.o=.d)

# *** EOF ***
