################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../archivoConfig.c \
../comunicacion.c \
../conexionesSocket.c \
../consola.c \
../handshake.c \
../protocolo.c \
../utils.c 

OBJS += \
./archivoConfig.o \
./comunicacion.o \
./conexionesSocket.o \
./consola.o \
./handshake.o \
./protocolo.o \
./utils.o 

C_DEPS += \
./archivoConfig.d \
./comunicacion.d \
./conexionesSocket.d \
./consola.d \
./handshake.d \
./protocolo.d \
./utils.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


