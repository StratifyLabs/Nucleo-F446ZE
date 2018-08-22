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

#ifndef BOARD_CONFIG_H_
#define BOARD_CONFIG_H_

#include <device/ffifo.h>
#include <mcu/debug.h>

void board_trace_event(void * event);

extern const ffifo_config_t board_trace_config;
extern ffifo_state_t board_trace_state;

void board_event_handler(int event, void * args); //optional event handler


#endif /* BOARD_CONFIG_H_ */
