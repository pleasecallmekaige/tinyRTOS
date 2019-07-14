######################################
# source
######################################
# C sources
ARCH_SRC =  \
        ${wildcard rtos/core/*.c} \
		${wildcard rtos/platform/*.c}
        C_SOURCES += $(ARCH_SRC)

# ASM sources
ASM_SOURCES +=  \
rtos/platform/os_cpu.s

# C includes
ARCH_INC = \
		-Irtos/core \
		-Irtos/platform
        C_INCLUDES += $(ARCH_INC)
