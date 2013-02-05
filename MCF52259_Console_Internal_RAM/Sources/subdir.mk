################################################################################
# 自动生成的文件。不要编辑！
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/MCF52259_sysinit.c" \
"../Sources/console_io_cf.c" \
"../Sources/exceptions.c" \
"../Sources/main.c" \
"../Sources/mcp2515.c" \
"../Sources/qspi.c" \
"../Sources/uart_support.c" \

C_SRCS += \
../Sources/MCF52259_sysinit.c \
../Sources/console_io_cf.c \
../Sources/exceptions.c \
../Sources/main.c \
../Sources/mcp2515.c \
../Sources/qspi.c \
../Sources/uart_support.c \

OBJS += \
./Sources/MCF52259_sysinit_c.obj \
./Sources/console_io_cf_c.obj \
./Sources/exceptions_c.obj \
./Sources/main_c.obj \
./Sources/mcp2515_c.obj \
./Sources/qspi_c.obj \
./Sources/uart_support_c.obj \

OBJS_QUOTED += \
"./Sources/MCF52259_sysinit_c.obj" \
"./Sources/console_io_cf_c.obj" \
"./Sources/exceptions_c.obj" \
"./Sources/main_c.obj" \
"./Sources/mcp2515_c.obj" \
"./Sources/qspi_c.obj" \
"./Sources/uart_support_c.obj" \

C_DEPS += \
./Sources/MCF52259_sysinit_c.d \
./Sources/console_io_cf_c.d \
./Sources/exceptions_c.d \
./Sources/main_c.d \
./Sources/mcp2515_c.d \
./Sources/qspi_c.d \
./Sources/uart_support_c.d \

OBJS_OS_FORMAT += \
./Sources/MCF52259_sysinit_c.obj \
./Sources/console_io_cf_c.obj \
./Sources/exceptions_c.obj \
./Sources/main_c.obj \
./Sources/mcp2515_c.obj \
./Sources/qspi_c.obj \
./Sources/uart_support_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/MCF52259_sysinit_c.obj: ../Sources/MCF52259_sysinit.c
	@echo '正在构建文件： $<'
	@echo '正在调用： ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"Sources/MCF52259_sysinit.args" -o "Sources/MCF52259_sysinit_c.obj" "$<" -MD -gccdep
	@echo '已结束构建： $<'
	@echo ' '

Sources/%.d: ../Sources/%.c
	@echo '正在生成依赖文件： $@'
	
	@echo ' '

Sources/console_io_cf_c.obj: ../Sources/console_io_cf.c
	@echo '正在构建文件： $<'
	@echo '正在调用： ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"Sources/console_io_cf.args" -o "Sources/console_io_cf_c.obj" "$<" -MD -gccdep
	@echo '已结束构建： $<'
	@echo ' '

Sources/exceptions_c.obj: ../Sources/exceptions.c
	@echo '正在构建文件： $<'
	@echo '正在调用： ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"Sources/exceptions.args" -o "Sources/exceptions_c.obj" "$<" -MD -gccdep
	@echo '已结束构建： $<'
	@echo ' '

Sources/main_c.obj: ../Sources/main.c
	@echo '正在构建文件： $<'
	@echo '正在调用： ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"Sources/main.args" -o "Sources/main_c.obj" "$<" -MD -gccdep
	@echo '已结束构建： $<'
	@echo ' '

Sources/mcp2515_c.obj: ../Sources/mcp2515.c
	@echo '正在构建文件： $<'
	@echo '正在调用： ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"Sources/mcp2515.args" -o "Sources/mcp2515_c.obj" "$<" -MD -gccdep
	@echo '已结束构建： $<'
	@echo ' '

Sources/qspi_c.obj: ../Sources/qspi.c
	@echo '正在构建文件： $<'
	@echo '正在调用： ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"Sources/qspi.args" -o "Sources/qspi_c.obj" "$<" -MD -gccdep
	@echo '已结束构建： $<'
	@echo ' '

Sources/uart_support_c.obj: ../Sources/uart_support.c
	@echo '正在构建文件： $<'
	@echo '正在调用： ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"Sources/uart_support.args" -o "Sources/uart_support_c.obj" "$<" -MD -gccdep
	@echo '已结束构建： $<'
	@echo ' '


