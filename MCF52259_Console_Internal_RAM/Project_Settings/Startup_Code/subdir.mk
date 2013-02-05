################################################################################
# 自动生成的文件。不要编辑！
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Project_Settings/Startup_Code/startcf.c" \

C_SRCS += \
../Project_Settings/Startup_Code/startcf.c \

OBJS += \
./Project_Settings/Startup_Code/startcf_c.obj \

OBJS_QUOTED += \
"./Project_Settings/Startup_Code/startcf_c.obj" \

C_DEPS += \
./Project_Settings/Startup_Code/startcf_c.d \

OBJS_OS_FORMAT += \
./Project_Settings/Startup_Code/startcf_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Project_Settings/Startup_Code/startcf_c.obj: ../Project_Settings/Startup_Code/startcf.c
	@echo '正在构建文件： $<'
	@echo '正在调用： ColdFire Compiler'
	"$(CF_ToolsDirEnv)/mwccmcf" @@"Project_Settings/Startup_Code/startcf.args" -o "Project_Settings/Startup_Code/startcf_c.obj" "$<" -MD -gccdep
	@echo '已结束构建： $<'
	@echo ' '

Project_Settings/Startup_Code/%.d: ../Project_Settings/Startup_Code/%.c
	@echo '正在生成依赖文件： $@'
	
	@echo ' '


