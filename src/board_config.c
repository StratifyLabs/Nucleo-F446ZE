/*

Copyright 2011-2016 Tyler Gilbert

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
#include <device/microchip/sst25vf.h>
#include <device/sys.h>
#include <device/uartfifo.h>
#include <device/usbfifo.h>
#include <device/fifo.h>
#include <device/mcfifo.h>
#include <device/sys.h>
#include <sos/link.h>
#include <sos/fs/sysfs.h>
#include <sos/fs/appfs.h>
#include <sos/fs/devfs.h>
#include <sos/fs/sffs.h>
#include <sos/sos.h>

#include "board_trace.h"
#include "link_transport.h"

#define SOS_BOARD_SYSTEM_CLOCK 120000000
#define SOS_BOARD_SYSTEM_MEMORY_SIZE (8192*2)
#define SOS_BOARD_TASK_TOTAL 10

static void board_event_handler(int event, void * args);

const mcu_board_config_t mcu_board_config = {
		.core_osc_freq = 12000000,
		.core_cpu_freq = SOS_BOARD_SYSTEM_CLOCK,
		.core_periph_freq = SOS_BOARD_SYSTEM_CLOCK,
		.usb_max_packet_zero = MCU_CORE_USB_MAX_PACKET_ZERO_VALUE,
		.debug_uart_port = 0,
		.debug_uart_attr = {
				.pin_assignment =
				{
						.rx = {0, 2},
						.tx = {0, 3},
						.cts = {0xff, 0xff},
						.rts = {0xff, 0xff}
				},
				.freq = 115200,
				.o_flags = UART_FLAG_IS_PARITY_NONE | UART_FLAG_IS_STOP1,
				.width = 8
		},
		.o_flags = 0,
		.event_handler = board_event_handler,
		.led.port = 1, .led.pin = 18
};


void board_event_handler(int event, void * args){
	switch(event){
	case MCU_BOARD_CONFIG_EVENT_ROOT_FATAL:
		//start the bootloader on a fatal event
		mcu_core_invokebootloader(0, 0);

		break;
	case MCU_BOARD_CONFIG_EVENT_START_LINK:
		mcu_debug_user_printf("Start LED\n");
		sos_led_startup();
		break;
	case MCU_BOARD_CONFIG_EVENT_START_FILESYSTEM:
		mcu_debug_user_printf("Started %ld apps\n", *((u32*)args));
		break;
	}
}


const sos_board_config_t sos_board_config = {
		.clk_usecond_tmr = 3,
		.task_total = SOS_BOARD_TASK_TOTAL,
		.stdin_dev = "/dev/stdio-in" ,
		.stdout_dev = "/dev/stdio-out",
		.stderr_dev = "/dev/stdio-out",
		.o_sys_flags = SYS_FLAG_IS_STDIO_FIFO | SYS_FLAG_IS_TRACE,
		.sys_name = "Stratify Alpha",
		.sys_version = "1.5",
		.sys_id = "-KZKdVwMXIj6vTVsbX56",
		.sys_memory_size = SOS_BOARD_SYSTEM_MEMORY_SIZE,
		//.start = sos_default_thread,
		.start = 0,
		//.start_args = &link_transport,
		.start_args = 0,
		.start_stack_size = SOS_DEFAULT_START_STACK_SIZE,
		.socket_api = 0,
		.request = 0,
		.trace_dev = "/dev/trace",
		.trace_event = board_trace_event
};

volatile sched_task_t sos_sched_table[SOS_BOARD_TASK_TOTAL] MCU_SYS_MEM;
task_t sos_task_table[SOS_BOARD_TASK_TOTAL] MCU_SYS_MEM;

#define USER_ROOT 0

/* This is the state information for the sst25vf flash IC driver.
 *
 */
sst25vf_state_t sst25vf_state MCU_SYS_MEM;

/* This is the configuration specific structure for the sst25vf
 * flash IC driver.
 */
//const sst25vf_cfg_t sst25vf_cfg = SST25VF_DEVICE_CFG(0, 6, -1, 0, -1, 0, 0, 8, 2*1024*1024, 10000000);
const sst25vf_config_t sst25vf_cfg = {
		.spi.attr = {
				.o_flags = SPI_FLAG_SET_MASTER | SPI_FLAG_IS_MODE0 | SPI_FLAG_IS_FORMAT_SPI,
				.freq = 16000000,
				.width = 8,
				.pin_assignment = {
						.miso = {0,8},
						.mosi = {0,9},
						.sck = {0,7},
						.cs = {0xff,0xff}
				},
		},
		.cs  = {0,6},
		.hold  = {0xff,0xff},
		.wp  = {0xff,0xff},
		.size = 2*1024*1024
};

#define UART0_DEVFIFO_BUFFER_SIZE 1024
char uart0_fifo_buffer[UART0_DEVFIFO_BUFFER_SIZE];

const uartfifo_config_t uart0_fifo_cfg = {
		.uart.attr = {
				.o_flags = UART_FLAG_IS_PARITY_NONE | UART_FLAG_IS_STOP1 | UART_FLAG_SET_LINE_CODING,
				.width = 8,
				.freq = 115200,
				.pin_assignment = {
						.tx = {0, 2},
						.rx = {0, 3},
						.rts = {0xff, 0xff},
						.cts = {0xff, 0xff}
				}
		},
		.fifo = { .size = UART0_DEVFIFO_BUFFER_SIZE, .buffer = uart0_fifo_buffer }
};

uartfifo_state_t uart0_fifo_state MCU_SYS_MEM;
//uartfifo_state_t uart0_fifo_state MCU_SYS_MEM;

#define UART1_DEVFIFO_BUFFER_SIZE 64
char uart1_fifo_buffer[UART1_DEVFIFO_BUFFER_SIZE];
const uartfifo_config_t uart1_fifo_cfg = {
		.fifo = { .size = UART1_DEVFIFO_BUFFER_SIZE, .buffer = uart1_fifo_buffer }
};
uartfifo_state_t uart1_fifo_state MCU_SYS_MEM;

#define UART3_DEVFIFO_BUFFER_SIZE 64
char uart3_fifo_buffer[UART3_DEVFIFO_BUFFER_SIZE];
const uartfifo_config_t uart3_fifo_cfg = {
		.fifo = { .size = UART3_DEVFIFO_BUFFER_SIZE, .buffer = uart3_fifo_buffer }
};
uartfifo_state_t uart3_fifo_state MCU_SYS_MEM;


#define STDIO_BUFFER_SIZE 128

char stdio_out_buffer[STDIO_BUFFER_SIZE];
char stdio_in_buffer[STDIO_BUFFER_SIZE];

fifo_config_t stdio_in_cfg = { .buffer = stdio_in_buffer, .size = STDIO_BUFFER_SIZE };
fifo_config_t stdio_out_cfg = { .buffer = stdio_out_buffer, .size = STDIO_BUFFER_SIZE };
fifo_state_t stdio_out_state = { .head = 0, .tail = 0, .rop = NULL, .rop_len = 0, .wop = NULL, .wop_len = 0 };
fifo_state_t stdio_in_state = { .head = 0, .tail = 0, .rop = NULL, .rop_len = 0, .wop = NULL, .wop_len = 0 };

const i2c_config_t i2c1_config = {
		.attr = {
				.o_flags = I2C_FLAG_SET_MASTER,
				.freq = 100000,
				.pin_assignment = {
						.sda = {0, 0},
						.scl = {0, 1}
				}
		}
};

const i2c_config_t i2c2_config = {
		.attr = {
				.o_flags = I2C_FLAG_SET_MASTER,
				.freq = 100000,
				.pin_assignment = {
						.sda = {0, 10},
						.scl = {0, 11}
				}
		}
};

#define STREAM_COUNT 16
#define STREAM_SIZE 128
#define STREAM_BUFFER_SIZE (STREAM_SIZE*STREAM_COUNT)
static char stream_buffer[STREAM_COUNT][STREAM_SIZE];

const fifo_config_t board_stream_config_fifo_array[STREAM_COUNT] = {
		{ .size = STREAM_SIZE, .buffer = stream_buffer[0] },
		{ .size = STREAM_SIZE, .buffer = stream_buffer[1] },
		{ .size = STREAM_SIZE, .buffer = stream_buffer[2] },
		{ .size = STREAM_SIZE, .buffer = stream_buffer[3] },
		{ .size = STREAM_SIZE, .buffer = stream_buffer[4] },
		{ .size = STREAM_SIZE, .buffer = stream_buffer[5] },
		{ .size = STREAM_SIZE, .buffer = stream_buffer[6] },
		{ .size = STREAM_SIZE, .buffer = stream_buffer[7] },
		{ .size = STREAM_SIZE, .buffer = stream_buffer[8] },
		{ .size = STREAM_SIZE, .buffer = stream_buffer[9] },
		{ .size = STREAM_SIZE, .buffer = stream_buffer[10] },
		{ .size = STREAM_SIZE, .buffer = stream_buffer[11] },
		{ .size = STREAM_SIZE, .buffer = stream_buffer[12] },
		{ .size = STREAM_SIZE, .buffer = stream_buffer[13] },
		{ .size = STREAM_SIZE, .buffer = stream_buffer[14] },
		{ .size = STREAM_SIZE, .buffer = stream_buffer[15] },
};

const mcfifo_config_t board_stream_config = {
		.count = STREAM_COUNT,
		.size = STREAM_SIZE,
		.fifo_config_array = board_stream_config_fifo_array
};

fifo_state_t board_stream_state_fifo_array[STREAM_COUNT];
mcfifo_state_t board_stream_state = {
		.fifo_state_array = board_stream_state_fifo_array
};

/* This is the list of devices that will show up in the /dev folder
 * automatically.  By default, the peripheral devices for the MCU are available
 * plus some devices on the board.
 */
const devfs_device_t devfs_list[] = {
		//mcu peripherals
		DEVFS_DEVICE("trace", ffifo, 0, &board_trace_config, &board_trace_state, 0666, USER_ROOT, S_IFCHR),
		DEVFS_DEVICE("multistream", mcfifo, 0, &board_stream_config, &board_stream_state, 0666, USER_ROOT, S_IFCHR),
		/*
		DEVFS_DEVICE("core", mcu_core, 0, 0, 0, 0666, USER_ROOT, S_IFCHR),
		DEVFS_DEVICE("core0", mcu_core, 0, 0, 0, 0666, USER_ROOT, S_IFCHR),
		DEVFS_DEVICE("adc0", mcu_adc, 0, 0, 0, 0666, USER_ROOT, S_IFCHR),
		DEVFS_DEVICE("dac0", mcu_dac, 0, 0, 0, 0666, USER_ROOT, S_IFCHR),
		DEVFS_DEVICE("eint0", mcu_eint, 0, 0, 0, 0666, USER_ROOT, S_IFCHR),
		DEVFS_DEVICE("eint1", mcu_eint, 1, 0, 0, 0666, USER_ROOT, S_IFCHR),
		DEVFS_DEVICE("eint2", mcu_eint, 2, 0, 0, 0666, USER_ROOT, S_IFCHR),
		DEVFS_DEVICE("eint3", mcu_eint, 3, 0, 0, 0666, USER_ROOT, S_IFCHR),
		DEVFS_DEVICE("pio0", mcu_pio, 0, 0, 0, 0666, USER_ROOT, S_IFCHR),
		DEVFS_DEVICE("pio1", mcu_pio, 1, 0, 0, 0666, USER_ROOT, S_IFCHR),
		DEVFS_DEVICE("pio2", mcu_pio, 2, 0, 0, 0666, USER_ROOT, S_IFCHR),
		DEVFS_DEVICE("pio3", mcu_pio, 3, 0, 0, 0666, USER_ROOT, S_IFCHR),
		DEVFS_DEVICE("pio4", mcu_pio, 4, 0, 0, 0666, USER_ROOT, S_IFCHR),
		DEVFS_DEVICE("i2c0", mcu_i2c, 0, 0, 0, 0666, USER_ROOT, S_IFCHR),
		DEVFS_DEVICE("i2c1", mcu_i2c, 1, &i2c1_config, 0, 0666, USER_ROOT, S_IFCHR),
		DEVFS_DEVICE("i2c2", mcu_i2c, 2, &i2c2_config, 0, 0666, USER_ROOT, S_IFCHR),
		DEVFS_DEVICE("pwm1", mcu_pwm, 1, 0, 0, 0666, USER_ROOT, S_IFCHR),
		DEVFS_DEVICE("qei0", mcu_qei, 0, 0, 0, 0666, USER_ROOT, S_IFCHR),
		DEVFS_DEVICE("rtc", mcu_rtc, 0, 0, 0, 0666, USER_ROOT, S_IFCHR),
		DEVFS_DEVICE("spi0", mcu_ssp, 0, 0, 0, 0666, USER_ROOT, S_IFCHR),
		DEVFS_DEVICE("spi1", mcu_ssp, 1, 0, 0, 0666, USER_ROOT, S_IFCHR),
		DEVFS_DEVICE("spi2", mcu_ssp, 2, 0, 0, 0666, USER_ROOT, S_IFCHR),
		DEVFS_DEVICE("tmr0", mcu_tmr, 0, 0, 0, 0666, USER_ROOT, S_IFCHR),
		DEVFS_DEVICE("tmr1", mcu_tmr, 1, 0, 0, 0666, USER_ROOT, S_IFCHR),
		DEVFS_DEVICE("tmr2", mcu_tmr, 2, 0, 0, 0666, USER_ROOT, S_IFCHR),
		//DEVFS_DEVICE("uart0", mcu_uart, 0, 0, 0, 0666, USER_ROOT, S_IFCHR),
		DEVFS_DEVICE("uart0", uartfifo, 0, &uart0_fifo_cfg, &uart0_fifo_state, 0666, USER_ROOT, S_IFCHR),
		DEVFS_DEVICE("uart1", mcu_uart, 1, 0, 0, 0666, USER_ROOT, S_IFCHR),
		//DEVFS_DEVICE("uart1", uartfifo, 1, &uart1_fifo_cfg, &uart1_fifo_state, 0666, USER_ROOT, S_IFCHR),
		DEVFS_DEVICE("uart2", mcu_uart, 2, 0, 0, 0666, USER_ROOT, S_IFCHR),
		//DEVFS_DEVICE("uart3", uartfifo, 3, &uart3_fifo_cfg, &uart3_fifo_state, 0666, USER_ROOT, S_IFCHR),
		DEVFS_DEVICE("usb0", mcu_usb, 0, 0, 0, 0666, USER_ROOT, S_IFCHR),

		//board devices
		DEVFS_DEVICE("drive0", sst25vf_ssp, 1, &sst25vf_cfg, &sst25vf_state, 0666, USER_ROOT, S_IFBLK),

		//FIFO buffers used for std in and std out
		DEVFS_DEVICE("stdio-out", fifo, 0, &stdio_out_cfg, &stdio_out_state, 0666, USER_ROOT, S_IFCHR),
		DEVFS_DEVICE("stdio-in", fifo, 0, &stdio_in_cfg, &stdio_in_state, 0666, USER_ROOT, S_IFCHR),

		//system devices
		DEVFS_DEVICE("link-phy-usb", usbfifo, 0, &sos_link_transport_usb_fifo_cfg, &sos_link_transport_usb_fifo_state, 0666, USER_ROOT, S_IFCHR),

*/
		DEVFS_DEVICE("sys", sys, 0, 0, 0, 0666, USER_ROOT, S_IFCHR),


		DEVFS_TERMINATOR
};

//this is the data needed for the stratify flash file system (wear-aware file system)
extern const sffs_config_t sffs_cfg;
sffs_state_t sffs_state;
open_file_t sffs_open_file; // Cannot be in MCU_SYS_MEM because it is accessed in unpriv mode

const sffs_config_t sffs_cfg = {
		.open_file = &sffs_open_file,
		.devfs = &(sysfs_list[1]),
		.name = "drive0",
		.state = &sffs_state
};


#if 0
//This is the setup code if you want to use a FAT filesystem (like on an SD card)

fatfs_state_t fatfs_state;
open_file_t fatfs_open_file; // Cannot be in MCU_SYS_MEM because it is accessed in unpriv mode

const fatfs_cfg_t fatfs_cfg = {
		.open_file = &fatfs_open_file,
		.devfs = &(sysfs_list[1]),
		.name = "drive0",
		.state = &fatfs_state,
		.vol_id = 0
};
#endif

//const devfs_device_t mem0 = DEVFS_DEVICE("mem0", mcu_mem, 0, 0, 0, 0666, USER_ROOT, S_IFBLK);


const sysfs_t const sysfs_list[] = {
		//APPFS_MOUNT("/app", &mem0, SYSFS_ALL_ACCESS), //the folder for ram/flash applications
		DEVFS_MOUNT("/dev", devfs_list, SYSFS_READONLY_ACCESS), //the list of devices
		//SFFS_MOUNT("/home", &sffs_cfg, SYSFS_ALL_ACCESS), //the stratify file system on external RAM
		//FATFS("/home", &fatfs_cfg, SYSFS_ALL_ACCESS), //fat filesystem with external SD card
		SYSFS_MOUNT("/", sysfs_list, SYSFS_READONLY_ACCESS), //the root filesystem (must be last)
		SYSFS_TERMINATOR
};


