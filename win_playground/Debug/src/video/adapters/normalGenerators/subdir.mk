################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/video/adapters/normalGenerators/NormalGenerator.cpp 

OBJS += \
./src/video/adapters/normalGenerators/NormalGenerator.o 

CPP_DEPS += \
./src/video/adapters/normalGenerators/NormalGenerator.d 


# Each subdirectory must supply rules for building sources it contributes
src/video/adapters/normalGenerators/%.o: ../src/video/adapters/normalGenerators/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


