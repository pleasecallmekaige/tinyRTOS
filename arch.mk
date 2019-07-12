######################################
# source
######################################
# C sources
ARCH_SRC =  \
        ${wildcard Middlewares/LiteOS/arch/arm/arm-m/src/*.c}
        C_SOURCES += $(ARCH_SRC)

# ASM sources
ASM_SOURCES_S +=  \
Middlewares/LiteOS/arch/arm/arm-m/cortex-m3/gcc/los_dispatch_gcc.S

# C includes
ARCH_INC = \
        -IMiddlewares/LiteOS/arch/arm/arm-m/include \
        -IMiddlewares/LiteOS/arch/arm/common/cmsis
        C_INCLUDES += $(ARCH_INC)