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
#define SOS_BOARD_DEBUG_FLAGS (MCU_DEBUG_INFO | MCU_DEBUG_SYS | MCU_DEBUG_DEVICE)


#define SOS_BOARD_SYSTEM_CLOCK 168000000
#define SOS_BOARD_SYSTEM_MEMORY_SIZE (8192*3)
#define SOS_BOARD_ID "-L2Tsz0P1cEDIywrY2qc"
#define SOS_BOARD_VERSION "0.11"
#define SOS_BOARD_NAME "Nucleo-F446ZE"

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
//#define SOS_BOARD_ARM_DSP_API_Q7 1
//#define SOS_BOARD_ARM_DSP_API_Q15 1
//#define SOS_BOARD_ARM_DSP_API_Q31 1
//#define SOS_BOARD_ARM_DSP_API_F32 1
//#define SOS_BOARD_ARM_DSP_CONVERSION_API 1


#endif /* CONFIG_H_ */
