cmake_minimum_required (VERSION 3.6)

#Add sources to the project
set(SOURCES_PREFIX ${CMAKE_SOURCE_DIR}/src)
add_subdirectory(src)
list(APPEND KERNEL_SOURCELIST ${SOURCES} ${CMAKE_SOURCE_DIR}/README.md)

#Change to boot sources
set(SOURCES "")
set(SOURCES_PREFIX ${CMAKE_SOURCE_DIR}/boot)
add_subdirectory(boot)
list(APPEND BOOT_SOURCELIST ${SOURCES})

if( ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin" )
  set(SOS_TOOLCHAIN_CMAKE_PATH /Applications/StratifyLabs-SDK/Tools/gcc/arm-none-eabi/cmake)
endif()
if( ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows" )
	set(SOS_TOOLCHAIN_CMAKE_PATH C:/StratifyLabs-SDK/Tools/gcc/arm-none-eabi/cmake)
endif()
set(CMAKE_TOOLCHAIN_FILE ${SOS_TOOLCHAIN_CMAKE_PATH}/sos-bsp-toolchain.cmake)
get_filename_component(SOS_BSP_NAME ${CMAKE_SOURCE_DIR} NAME)
project(${SOS_BSP_NAME} CXX C)

set(SOS_BSP_ARCH v7em_f4sh)
set(SOS_BSP_DEVICE stm32f446ze)
set(SOS_BSP_DEVICE_FAMILY stm32f446xx)
set(SOS_BSP_HARDWARD_ID 0x00000008)
set(SOS_BSP_LIBRARIES son_kernel)

set(SOS_BSP_STD_KERNEL_START_ADDRESS 0x08040000)
set(SOS_BSP_STD_KERNEL_SOURCELIST ${KERNEL_SOURCELIST})
set(SOS_BSP_STD_BOOT_START_ADDRESS 0x08000000)
set(SOS_BSP_STD_BOOT_SOURCELIST ${BOOT_SOURCELIST})
include(${SOS_TOOLCHAIN_CMAKE_PATH}/sos-bsp-std.cmake)
