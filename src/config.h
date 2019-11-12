/*

Copyright 2011-2018 Stratify Labs, Inc

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/

#ifndef CONFIG_H_
#define CONFIG_H_

#include <mcu/arch.h>

#include "board_config.h"

//openocd -f interface/stlink-v2-1.cfg -f target/stm32f4x_stlink.cfg
//openocd -f interface/stlink-v2-1.cfg -f target/stm32f4x_stlink.cfg -c "program ./Nucleo-F446ZE/build_debug_boot/Nucleo-F446ZE.bin 0x08000000; reset run; exit;"
#define SOS_BOARD_DEBUG_FLAGS (MCU_DEBUG_INFO | MCU_DEBUG_SYS)


#define SOS_BOARD_SYSTEM_CLOCK 168000000
#define SOS_BOARD_SYSTEM_MEMORY_SIZE (8192*3)

#define SOS_BOARD_USB_RX_BUFFER_SIZE 512
#define SOS_BOARD_STDIO_BUFFER_SIZE 512
#define SOS_BOARD_TMR 1

//Total number of tasks (threads) for the entire system
#define SOS_BOARD_TASK_TOTAL 10
#define SOS_BOARD_EVENT_HANDLER board_event_handler
#define SOS_BOARD_TRACE_EVENT board_trace_event

#define STM32_ARCH_O_FLAGS STM32_CONFIG_FLAG_IS_HSE_ON
#define STM32_ARCH_CLOCK_PLLM 4
#define STM32_ARCH_CLOCK_PLLN 168
#define STM32_ARCH_CLOCK_PLLP 2
#define STM32_ARCH_CLOCK_PLLQ 7
#define STM32_ARCH_CLOCK_PLLR 2
#define STM32_ARCH_CLOCK_AHB_CLOCK_DIVIDER 1
#define STM32_ARCH_CLOCK_APB1_CLOCK_DIVIDER 4
#define STM32_ARCH_CLOCK_APB2_CLOCK_DIVIDER 2
#define STM32_ARCH_CLOCK_VOLTAGE_SCALE 1
#define STM32_ARCH_CLOCK_FLASH_LATENCY 5

//--------------------------------------------Symbols-------------------------------------------------

/* By defining Ignore switches, functions can be omitted from the kernel
 * This means any applications that use these functions will fail
 * to install on the BSP.
 * 
 * If you are building a custom board, ignoring symbols is a good
 * way to squeeze down the kernel to only what is necessary. However,
 * if you plan on allowing your users to install applications, they
 * might find it challenging when some functions are missing (the
 * applications will compile but fail to install).
 * 
 * See [sos/symbols/defines.h](https://github.com/StratifyLabs/StratifyOS/blob/master/include/sos/symbols/defines.h) 
 * for all available switches.
 * 
 */ 

#define SYMBOLS_IGNORE_DCOMPLEX 1
#define SYMBOLS_IGNORE_POSIX_TRACE 1 //Always ignore -- deprecated
#define SYMBOLS_IGNORE_SG 1 //Stratify Graphics -- ignore if board will not support displays
#define SYMBOLS_IGNORE_SOCKET 1
#define SYMBOLS_IGNORE_LWIP 1

//other ignore switches
#if 0
#define SYMBOLS_IGNORE_MATH_F 1
#define SYMBOLS_IGNORE_DOUBLE 1
#define SYMBOLS_IGNORE_STDIO_FILE 1
#define SYMBOLS_IGNORE_SIGNAL 1
#define SYMBOLS_IGNORE_PTHREAD_MUTEX 1
#define SYMBOLS_IGNORE_PTHREAD_COND 1
#define SYMBOLS_IGNORE_AIO 1
#define SYMBOLS_IGNORE_WCTYPE 1
#define SYMBOLS_IGNORE_STR 1
#define SYMBOLS_IGNORE_SEM 1
#define SYMBOLS_IGNORE_MQ 1
#endif

/* Uncomment to add ARM CMSIS DSP libraries to the BSP
 * 
 * See [ARM CMSIS Declaration](https://github.com/StratifyLabs/StratifyOS-CMSIS/blob/master/arm_dsp_api_declaration.h)
 * for more detailed link configuration switches.
 * 
 * 
 */
#define SOS_BOARD_ARM_DSP_API_Q7 0
#define SOS_BOARD_ARM_DSP_API_Q15 0
#define SOS_BOARD_ARM_DSP_API_Q31 0
#define SOS_BOARD_ARM_DSP_API_F32 0
#define SOS_BOARD_ARM_DSP_CONVERSION_API 0

//--------------------------------------------Hardware Pins-------------------------------------------------

#define SOS_BOARD_USART2_TX_PORT 3 //PD5
#define SOS_BOARD_USART2_TX_PIN 5
#define SOS_BOARD_USART2_RX_PORT 3 //PD6
#define SOS_BOARD_USART2_RX_PIN 6

#define SOS_BOARD_USART3_TX_PORT 3 //PD8
#define SOS_BOARD_USART3_TX_PIN 8
#define SOS_BOARD_USART3_RX_PORT 3 //PD9
#define SOS_BOARD_USART3_RX_PIN 9

#define SOS_BOARD_USART6_TX_PORT 6 //PG5
#define SOS_BOARD_USART6_TX_PIN 9
#define SOS_BOARD_USART6_RX_PORT 6 //PG14
#define SOS_BOARD_USART6_RX_PIN 14

#define SOS_BOARD_I2C1_SDA_PORT 1 //PB9
#define SOS_BOARD_I2C1_SDA_PIN 9
#define SOS_BOARD_I2C1_SCL_PORT 1 //PB8
#define SOS_BOARD_I2C1_SCL_PIN 8

#define SOS_BOARD_I2C2_SDA_PORT 5 //PF0
#define SOS_BOARD_I2C2_SDA_PIN 0
#define SOS_BOARD_I2C2_SCL_PORT 5 //PF1
#define SOS_BOARD_I2C2_SCL_PIN 1

#define SOS_BOARD_SPI1_MISO_PORT 0 //PA6
#define SOS_BOARD_SPI1_MISO_PIN 6
#define SOS_BOARD_SPI1_MOSI_PORT 0 //PA7
#define SOS_BOARD_SPI1_MOSI_PIN 7
#define SOS_BOARD_SPI1_SCK_PORT 0 //PA5
#define SOS_BOARD_SPI1_SCK_PIN 5
#define SOS_BOARD_SPI1_CS_PORT 0xff //Not used
#define SOS_BOARD_SPI1_CS_PIN 0xff

#define SOS_BOARD_SPI3_MISO_PORT 1 //PB4
#define SOS_BOARD_SPI3_MISO_PIN 4
#define SOS_BOARD_SPI3_MOSI_PORT 1 //PB5
#define SOS_BOARD_SPI3_MOSI_PIN 5
#define SOS_BOARD_SPI3_SCK_PORT 1  //PB3
#define SOS_BOARD_SPI3_SCK_PIN 3
#define SOS_BOARD_SPI3_CS_PORT 0xff //Not used
#define SOS_BOARD_SPI3_CS_PIN 0xff

#define SOS_BOARD_USART2_RX_DMA STM32_DMA1
#define SOS_BOARD_USART2_RX_DMA_STREAM 5
#define SOS_BOARD_USART2_RX_DMA_CHANNEL 4
#define SOS_BOARD_USART2_TX_DMA STM32_DMA1
#define SOS_BOARD_USART2_TX_DMA_STREAM 6
#define SOS_BOARD_USART2_TX_DMA_CHANNEL 4

#define SOS_BOARD_SPI1_RX_DMA STM32_DMA2
#define SOS_BOARD_SPI1_RX_DMA_STREAM 0
#define SOS_BOARD_SPI1_RX_DMA_CHANNEL 3
#define SOS_BOARD_SPI1_TX_DMA STM32_DMA2
#define SOS_BOARD_SPI1_TX_DMA_STREAM 3
#define SOS_BOARD_SPI1_TX_DMA_CHANNEL 3

#define SOS_BOARD_SPI3_RX_DMA STM32_DMA1
#define SOS_BOARD_SPI3_RX_DMA_STREAM 0
#define SOS_BOARD_SPI3_RX_DMA_CHANNEL 0
#define SOS_BOARD_SPI3_TX_DMA STM32_DMA1
#define SOS_BOARD_SPI3_TX_DMA_STREAM 5
#define SOS_BOARD_SPI3_TX_DMA_CHANNEL 0

#define SOS_BOARD_SD_DATA0_PORT 2 //PC8
#define SOS_BOARD_SD_DATA0_PIN 8
#define SOS_BOARD_SD_DATA1_PORT 2 //PC9
#define SOS_BOARD_SD_DATA1_PIN 9
#define SOS_BOARD_SD_DATA2_PORT 2 //PC10
#define SOS_BOARD_SD_DATA2_PIN 10
#define SOS_BOARD_SD_DATA3_PORT 2 //PC11
#define SOS_BOARD_SD_DATA3_PIN 11
#define SOS_BOARD_SD_CLK_PORT 2 //PC12
#define SOS_BOARD_SD_CLK_PIN 12
#define SOS_BOARD_SD_CMD_PORT 3 //PD2
#define SOS_BOARD_SD_CMD_PIN 2

#endif /* CONFIG_H_ */
