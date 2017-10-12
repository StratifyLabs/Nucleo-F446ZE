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

const u32 mcu_crp_value __attribute__ ((section(".crp_section"))) = 0x87654321;

#define STFY_SYSTEM_CLOCK 60000000
#define STFY_SYSTEM_OSC 12000000

const mcu_board_config_t mcu_board_config = {
		.core_osc_freq = STFY_SYSTEM_OSC,
		.core_cpu_freq = STFY_SYSTEM_CLOCK,
		.core_periph_freq = STFY_SYSTEM_CLOCK,
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
		.led.port = 1, .led.pin = 18,
		.event_handler = 0
};

const bootloader_board_config_t boot_board_config = {
		.sw_req_loc = 0x10002000,
		.sw_req_value = 0x55AA55AA,
		.program_start_addr = 0x40000,
		.hw_req.port = 0, .hw_req.pin = 1,
		.o_flags = 0,
		.link_transport_driver = &link_transport,
		.id = __HARDWARE_ID,
};


extern void boot_main();

void _main(){
	boot_main();
}
