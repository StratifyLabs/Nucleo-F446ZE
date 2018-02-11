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


#include <mcu/types.h>
#include <mcu/core.h>
#include <mcu/bootloader.h>

#include "link_transport.h"
#include "board_config.h"

extern u32 _flash_start;

const struct __sFILE_fake __sf_fake_stdin;
const struct __sFILE_fake __sf_fake_stdout;
const struct __sFILE_fake __sf_fake_stderr;

#define STFY_SYSTEM_CLOCK 168000000

#define USB_RX_BUFFER_SIZE 512
char usb_rx_buffer[USB_RX_BUFFER_SIZE] MCU_SYS_MEM;

const mcu_board_config_t mcu_board_config = {
		.core_osc_freq = 8000000,
		.core_cpu_freq = STFY_SYSTEM_CLOCK,
		.core_periph_freq = STFY_SYSTEM_CLOCK,
		.usb_max_packet_zero = MCU_CORE_USB_MAX_PACKET_ZERO_VALUE,
        .debug_uart_port = 2, //USART3 - is connected to mbed console
		.debug_uart_attr = {
				.pin_assignment =
				{
						.rx = {3, 9},
						.tx = {3, 8},
						.cts = {0xff, 0xff},
						.rts = {0xff, 0xff}
				},
				.freq = 115200,
				.o_flags = UART_FLAG_SET_LINE_CODING | UART_FLAG_IS_PARITY_NONE | UART_FLAG_IS_STOP1,
				.width = 8
		},
		.o_flags = MCU_BOARD_CONFIG_FLAG_LED_ACTIVE_HIGH,
		.led = {1, 7},
		.event_handler = 0,
		.usb_rx_buffer = usb_rx_buffer,
		.usb_rx_buffer_size = USB_RX_BUFFER_SIZE
};

const bootloader_board_config_t boot_board_config = {
		.sw_req_loc = 0x20004000,
		.sw_req_value = 0x55AA55AA,
        .program_start_addr = 0x40000 + (u32)&_flash_start, //flash start + 256KB
		.hw_req = {2, 13}, //PC13
		.o_flags = BOOT_BOARD_CONFIG_FLAG_HW_REQ_ACTIVE_HIGH,
		.link_transport_driver = &link_transport,
		.id = __HARDWARE_ID,
};

extern void boot_main();

void _main(){
	boot_main();
}
