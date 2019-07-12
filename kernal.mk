######################################
# C sources
KERNEL_SRC =  \
        ${wildcard Middlewares/LiteOS/kernel/*.c} \
        ${wildcard Middlewares/LiteOS/kernel/base/core/*.c} \
        ${wildcard Middlewares/LiteOS/kernel/base/ipc/*.c} \
        ${wildcard Middlewares/LiteOS/kernel/base/mem/bestfit_little/*.c} \
        ${wildcard Middlewares/LiteOS/kernel/base/mem/membox/*.c} \
        ${wildcard Middlewares/LiteOS/kernel/base/mem/common/*.c} \
        ${wildcard Middlewares/LiteOS/kernel/base/misc/*.c} \
        ${wildcard Middlewares/LiteOS/kernel/base/om/*.c} \
        ${wildcard Middlewares/LiteOS/kernel/extended/tickless/*.c}
        C_SOURCES += $(KERNEL_SRC)

# C includes
KERNEL_INC = \
        -IMiddlewares/LiteOS/kernel/base/include  \
        -IMiddlewares/LiteOS/kernel/extended/include  \
        -IMiddlewares/LiteOS/kernel/include
        C_INCLUDES += $(KERNEL_INC)
