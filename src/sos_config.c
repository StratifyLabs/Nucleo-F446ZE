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


#include <sys/lock.h>
#include <fcntl.h>
#include <errno.h>
#include <mcu/mcu.h>
#include <mcu/debug.h>
#include <mcu/periph.h>
#include <device/sys.h>
#include <device/uartfifo.h>
#include <device/usbfifo.h>
#include <device/fifo.h>
#include <device/cfifo.h>
#include <device/sys.h>
#include <sos/link.h>
#include <sos/fs/sysfs.h>
#include <sos/fs/appfs.h>
#include <sos/fs/devfs.h>
#include <sos/fs/sffs.h>
#include <sos/sos.h>

#include "config.h"
#include "sl_config.h"
#include "link_config.h"


//--------------------------------------------Stratify OS Configuration-------------------------------------------------
const sos_board_config_t sos_board_config = {
	.clk_usecond_tmr = SOS_BOARD_TMR, //TIM2 -- 32 bit timer
	.task_total = SOS_BOARD_TASK_TOTAL,
	.stdin_dev = "/dev/stdio-in" ,
	.stdout_dev = "/dev/stdio-out",
	.stderr_dev = "/dev/stdio-out",
	.o_sys_flags = SYS_FLAG_IS_STDIO_FIFO | SYS_FLAG_IS_TRACE,
	.sys_name = SL_CONFIG_NAME,
	.sys_version = SL_CONFIG_VERSION_STRING,
	.sys_id = SL_CONFIG_DOCUMENT_ID,
	.sys_memory_size = SOS_BOARD_SYSTEM_MEMORY_SIZE,
	.start = sos_default_thread,
	.start_args = &link_transport,
	.start_stack_size = SOS_DEFAULT_START_STACK_SIZE,
	.socket_api = 0,
	.request = 0,
	.trace_dev = "/dev/trace",
	.trace_event = SOS_BOARD_TRACE_EVENT,
	.git_hash = SOS_GIT_HASH
};

//This declares the task tables required by Stratify OS for applications and threads
SOS_DECLARE_TASK_TABLE(SOS_BOARD_TASK_TOTAL);

//--------------------------------------------Device Filesystem-------------------------------------------------


/*
 * Defaults configurations
 *
 * This provides the default pin assignments and settings for peripherals. If
 * the defaults are not provided, the application must specify them.
 *
 * Defaults should be added for peripherals that are dedicated for use on the
 * board. For example, if a UART has an external connection and label on the
 * board, the BSP should provide the default configuration.
 *
 *
 *
 */
//USART2
UARTFIFO_DECLARE_CONFIG_STATE(uart1_fifo, 1024,
										UART_FLAG_SET_LINE_CODING_DEFAULT, 8, 115200,
										SOS_BOARD_USART2_RX_PORT, SOS_BOARD_USART2_RX_PIN, //RX
										SOS_BOARD_USART2_TX_PORT, SOS_BOARD_USART2_TX_PIN, //TX
										0xff, 0xff,
										0xff, 0xff);
#if !defined __debug
//USART3
UARTFIFO_DECLARE_CONFIG_STATE(uart2_fifo, 1024,
										UART_FLAG_SET_LINE_CODING_DEFAULT, 8, 115200,
										SOS_BOARD_USART3_RX_PORT, SOS_BOARD_USART3_RX_PIN, //RX
										SOS_BOARD_USART3_TX_PORT, SOS_BOARD_USART3_TX_PIN, //TX
										0xff, 0xff,
										0xff, 0xff);
#endif

//USART6
UARTFIFO_DECLARE_CONFIG_STATE(uart5_fifo, 1024,
										UART_FLAG_SET_LINE_CODING_DEFAULT, 8, 115200,
										SOS_BOARD_USART6_RX_PORT, SOS_BOARD_USART6_RX_PIN, //RX
										SOS_BOARD_USART6_TX_PORT, SOS_BOARD_USART6_TX_PIN, //TX
										0xff, 0xff,
										0xff, 0xff);

//I2C1
I2C_DECLARE_CONFIG_MASTER(i2c0,
								  I2C_FLAG_SET_MASTER,
								  100000,
								  SOS_BOARD_I2C1_SDA_PORT, SOS_BOARD_I2C1_SDA_PIN, //SDA
								  SOS_BOARD_I2C1_SCL_PORT, SOS_BOARD_I2C1_SCL_PIN); //SCL

//I2C2
I2C_DECLARE_CONFIG_MASTER(i2c1,
								  I2C_FLAG_SET_MASTER,
								  100000,
								  SOS_BOARD_I2C2_SDA_PORT, SOS_BOARD_I2C2_SDA_PIN, //SDA
								  SOS_BOARD_I2C2_SCL_PORT, SOS_BOARD_I2C2_SCL_PIN); //SCL


#define SPI_DMA_FLAGS STM32_DMA_FLAG_IS_NORMAL | \
	STM32_DMA_FLAG_IS_MEMORY_SINGLE | \
	STM32_DMA_FLAG_IS_PERIPH_SINGLE | \
	STM32_DMA_FLAG_IS_PERIPH_BYTE | \
	STM32_DMA_FLAG_IS_MEMORY_BYTE

const stm32_spi_dma_config_t spi0_dma_config = {
	.spi_config = {
		.attr = {
			.o_flags = SPI_FLAG_SET_MASTER | SPI_FLAG_IS_FORMAT_SPI | SPI_FLAG_IS_MODE0 | SPI_FLAG_IS_FULL_DUPLEX,
			.width = 8,
			.freq = 1000000UL,
			.pin_assignment = {
				.miso = {SOS_BOARD_SPI1_MISO_PORT, SOS_BOARD_SPI1_MISO_PIN},
				.mosi = {SOS_BOARD_SPI1_MOSI_PORT, SOS_BOARD_SPI1_MOSI_PIN},
				.sck = {SOS_BOARD_SPI1_SCK_PORT, SOS_BOARD_SPI1_SCK_PIN},
				.cs = {SOS_BOARD_SPI1_CS_PORT, SOS_BOARD_SPI1_CS_PIN}
			}
		}
	},
	.dma_config = {
		.rx = {
			.dma_number = SOS_BOARD_SPI1_RX_DMA,
			.stream_number = SOS_BOARD_SPI1_RX_DMA_STREAM,
			.channel_number = SOS_BOARD_SPI1_RX_DMA_CHANNEL,
			.priority = STM32_DMA_PRIORITY_LOW,
			.o_flags = STM32_DMA_FLAG_IS_PERIPH_TO_MEMORY |
			SPI_DMA_FLAGS,
		},
		.tx = {
			.dma_number = SOS_BOARD_SPI1_TX_DMA,
			.stream_number = SOS_BOARD_SPI1_TX_DMA_STREAM,
			.channel_number = SOS_BOARD_SPI1_TX_DMA_CHANNEL,
			.priority = STM32_DMA_PRIORITY_LOW,
			.o_flags = STM32_DMA_FLAG_IS_MEMORY_TO_PERIPH |
			SPI_DMA_FLAGS
		}
	}
};

const stm32_spi_dma_config_t spi2_dma_config = {
	.spi_config = {
		.attr = {
			.o_flags = SPI_FLAG_SET_MASTER | SPI_FLAG_IS_FORMAT_SPI | SPI_FLAG_IS_MODE0 | SPI_FLAG_IS_FULL_DUPLEX,
			.width = 8,
			.freq = 1000000UL,
			.pin_assignment = {
				.miso = {SOS_BOARD_SPI3_MISO_PORT, SOS_BOARD_SPI3_MISO_PIN},
				.mosi = {SOS_BOARD_SPI3_MOSI_PORT, SOS_BOARD_SPI3_MOSI_PIN},
				.sck = {SOS_BOARD_SPI3_SCK_PORT, SOS_BOARD_SPI3_SCK_PIN},
				.cs = {SOS_BOARD_SPI3_CS_PORT, SOS_BOARD_SPI3_CS_PIN}
			}
		}
	},
	.dma_config = {
		.rx = {
			.dma_number = SOS_BOARD_SPI3_RX_DMA,
			.stream_number = SOS_BOARD_SPI3_RX_DMA_STREAM,
			.channel_number = SOS_BOARD_SPI3_RX_DMA_CHANNEL,
			.priority = STM32_DMA_PRIORITY_LOW,
			.o_flags = STM32_DMA_FLAG_IS_PERIPH_TO_MEMORY |
			SPI_DMA_FLAGS,
		},
		.tx = {
			.dma_number = SOS_BOARD_SPI3_TX_DMA,
			.stream_number = SOS_BOARD_SPI3_TX_DMA_STREAM,
			.channel_number = SOS_BOARD_SPI3_TX_DMA_CHANNEL,
			.priority = STM32_DMA_PRIORITY_LOW,
			.o_flags = STM32_DMA_FLAG_IS_MEMORY_TO_PERIPH |
			SPI_DMA_FLAGS
		}
	}
};

//Coming Soon
//SD Card as DMA
//I2S2
//I2S3
//SAI1A/B


FIFO_DECLARE_CONFIG_STATE(stdio_in, SOS_BOARD_STDIO_BUFFER_SIZE);
FIFO_DECLARE_CONFIG_STATE(stdio_out, SOS_BOARD_STDIO_BUFFER_SIZE);
CFIFO_DECLARE_CONFIG_STATE_4(board_fifo, 256);

#if !defined SOS_BOARD_USB_PORT
#define SOS_BOARD_USB_PORT 0
#endif

/* This is the list of devices that will show up in the /dev folder.
 */
const devfs_device_t devfs_list[] = {
	//System devices
	DEVFS_DEVICE("trace", ffifo, 0, &board_trace_config, &board_trace_state, 0666, SOS_USER_ROOT, S_IFCHR),
	DEVFS_DEVICE("fifo", cfifo, 0, &board_fifo_config, &board_fifo_state, 0666, SOS_USER_ROOT, S_IFCHR),
	DEVFS_DEVICE("stdio-out", fifo, 0, &stdio_out_config, &stdio_out_state, 0666, SOS_USER_ROOT, S_IFCHR),
	DEVFS_DEVICE("stdio-in", fifo, 0, &stdio_in_config, &stdio_in_state, 0666, SOS_USER_ROOT, S_IFCHR),
	DEVFS_DEVICE("link-phy-usb", usbfifo, SOS_BOARD_USB_PORT, &sos_link_transport_usb_fifo_cfg, &sos_link_transport_usb_fifo_state, 0666, SOS_USER_ROOT, S_IFCHR),
	DEVFS_DEVICE("sys", sys, 0, 0, 0, 0666, SOS_USER_ROOT, S_IFCHR),
	//DEVFS_DEVICE("rtc", mcu_rtc, 0, 0, 0, 0666, SOS_USER_ROOT, S_IFCHR),

	//MCU peripherals
	DEVFS_DEVICE("core", mcu_core, 0, 0, 0, 0666, SOS_USER_ROOT, S_IFCHR),
	DEVFS_DEVICE("core0", mcu_core, 0, 0, 0, 0666, SOS_USER_ROOT, S_IFCHR),

	DEVFS_DEVICE("i2c0", mcu_i2c, 0, 0, 0, 0666, SOS_USER_ROOT, S_IFCHR),
	DEVFS_DEVICE("i2c1", mcu_i2c, 1, 0, 0, 0666, SOS_USER_ROOT, S_IFCHR),
	DEVFS_DEVICE("i2c2", mcu_i2c, 2, 0, 0, 0666, SOS_USER_ROOT, S_IFCHR),
	DEVFS_DEVICE("i2c3", mcu_i2c, 3, 0, 0, 0666, SOS_USER_ROOT, S_IFCHR),

	DEVFS_DEVICE("pio0", mcu_pio, 0, 0, 0, 0666, SOS_USER_ROOT, S_IFCHR), //GPIOA
	DEVFS_DEVICE("pio1", mcu_pio, 1, 0, 0, 0666, SOS_USER_ROOT, S_IFCHR), //GPIOB
	DEVFS_DEVICE("pio2", mcu_pio, 2, 0, 0, 0666, SOS_USER_ROOT, S_IFCHR), //GPIOC
	DEVFS_DEVICE("pio3", mcu_pio, 3, 0, 0, 0666, SOS_USER_ROOT, S_IFCHR), //GPIOD
	DEVFS_DEVICE("pio4", mcu_pio, 4, 0, 0, 0666, SOS_USER_ROOT, S_IFCHR), //GPIOE
	DEVFS_DEVICE("pio5", mcu_pio, 5, 0, 0, 0666, SOS_USER_ROOT, S_IFCHR), //GPIOF
	DEVFS_DEVICE("pio6", mcu_pio, 6, 0, 0, 0666, SOS_USER_ROOT, S_IFCHR), //GPIOG
	DEVFS_DEVICE("pio7", mcu_pio, 7, 0, 0, 0666, SOS_USER_ROOT, S_IFCHR), //GPIOH

	DEVFS_DEVICE("spi0", mcu_spi, 0, 0, 0, 0666, SOS_USER_ROOT, S_IFCHR),
	DEVFS_DEVICE("spi1", mcu_spi, 1, 0, 0, 0666, SOS_USER_ROOT, S_IFCHR),
	DEVFS_DEVICE("spi2", mcu_spi, 2, 0, 0, 0666, SOS_USER_ROOT, S_IFCHR),
	DEVFS_DEVICE("spi3", mcu_spi, 3, 0, 0, 0666, SOS_USER_ROOT, S_IFCHR),

	DEVFS_DEVICE("tmr0", mcu_tmr, 0, 0, 0, 0666, SOS_USER_ROOT, S_IFCHR), //TIM1
	DEVFS_DEVICE("tmr1", mcu_tmr, 1, 0, 0, 0666, SOS_USER_ROOT, S_IFCHR), //TIM2
	DEVFS_DEVICE("tmr2", mcu_tmr, 2, 0, 0, 0666, SOS_USER_ROOT, S_IFCHR),
	DEVFS_DEVICE("tmr3", mcu_tmr, 3, 0, 0, 0666, SOS_USER_ROOT, S_IFCHR),
	DEVFS_DEVICE("tmr4", mcu_tmr, 4, 0, 0, 0666, SOS_USER_ROOT, S_IFCHR),
	DEVFS_DEVICE("tmr5", mcu_tmr, 5, 0, 0, 0666, SOS_USER_ROOT, S_IFCHR),
	DEVFS_DEVICE("tmr6", mcu_tmr, 6, 0, 0, 0666, SOS_USER_ROOT, S_IFCHR),
	DEVFS_DEVICE("tmr7", mcu_tmr, 7, 0, 0, 0666, SOS_USER_ROOT, S_IFCHR), //TIM8
	//Does this chip have more timers?

	DEVFS_DEVICE("uart1", uartfifo, 0, &uart1_fifo_config, &uart1_fifo_state, 0666, SOS_USER_ROOT, S_IFCHR),
	#if !defined __debug
	DEVFS_DEVICE("uart2", uartfifo, 0, &uart2_fifo_config, &uart2_fifo_state, 0666, SOS_USER_ROOT, S_IFCHR),
	#endif
	DEVFS_DEVICE("uart5", uartfifo, 0, &uart5_fifo_config, &uart5_fifo_state, 0666, SOS_USER_ROOT, S_IFCHR),
	DEVFS_TERMINATOR
};


//--------------------------------------------Root Filesystem---------------------------------------------------

/*
 * This is the root filesystem that determines what is mounted at /.
 *
 * The default is /app (for installing and running applciations in RAM and flash) and /dev which
 * provides the device tree defined above.
 *
 * Additional filesystems (such as FatFs) can be added if the hardware and drivers
 * are provided by the board.
 *
 */

const devfs_device_t mem0 = DEVFS_DEVICE("mem0", mcu_mem, 0, 0, 0, 0666, SOS_USER_ROOT, S_IFBLK);
const sysfs_t sysfs_list[] = {
	APPFS_MOUNT("/app", &mem0, SYSFS_ALL_ACCESS), //the folder for ram/flash applications
	DEVFS_MOUNT("/dev", devfs_list, SYSFS_READONLY_ACCESS), //the list of devices
	SYSFS_MOUNT("/", sysfs_list, SYSFS_READONLY_ACCESS), //the root filesystem (must be last)
	SYSFS_TERMINATOR
};


